#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\ChatacterWidget.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\LifeGauge\LifeGauge.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\BackGauge\BackGauge.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\SpecialAbilityGauge\SpecialAbilityGauge.h"
#include "..\..\..\..\Common\Effect\EffectManager.h"
#include "..\..\Item\EffectTimer\EffectTimer.h"

#include "..\..\..\..\Utility\BitFlagManager\BitFlagManager.h"
#include "..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"
#include "..\...\..\..\..\..\XAudio2\SoundManager.h"

CPlayer::CPlayer()
	: m_pCamera						( nullptr )
	, m_pSPCamera					( nullptr )
	, m_pAttackCollManager			( nullptr )
	, m_pEffects					()
	, m_pEffectTimers				( player::EEffectTimerNo_Max )
	, m_pWidget						()
	, m_AttackComboCount			( player::EAttackNo_None )
	, m_AttackDataQueue				()
	, m_AttackPosition				( FOR_INIT_ZERO_VEC3 )
	, m_GirlPosition				( FOR_INIT_ZERO_VEC3 )
	, m_AttackVector				( FOR_INIT_ZERO_VEC3 )
	, m_AvoidVector					( FOR_INIT_ZERO_VEC3 )
	, m_HitVector					( FOR_INIT_ZERO_VEC3 )
	, m_TargetVector				( FOR_INIT_ZERO_VEC3 )
	, m_AnimAdjDrggingParam			()
	, m_AttackAdjParam				()
	, m_Parameter					()
	, m_LifePoint					( FOR_INIT_ZERO_FLOAT )
	, m_SpecialAbility				( FOR_INIT_ZERO_FLOAT )
	, m_StatusFlag					( player::EStatusFlag_None )
	, m_AttackRangeLenght			( FOR_INIT_ZERO_FLOAT )
	, m_SpecialAbilityValue			( FOR_INIT_ZERO_FLOAT )
	, m_ItemSpecialAbilityValue		( FOR_INIT_ZERO_FLOAT )
	, m_AttackPower					( FOR_INIT_ZERO_FLOAT )
	, m_MoveSpeed					( FOR_INIT_ZERO_FLOAT )
	, m_MoveSpeedMulValue			( FOR_INIT_ZERO_FLOAT )
//	, m_HitStopCameraPosition		( FOR_INIT_ZERO_VEC3 )
	, m_CameraPosition				( FOR_INIT_ZERO_VEC3 )
	, m_CameraLookPosition			( FOR_INIT_ZERO_VEC3 )
	, m_CameraCount					( CAMERA_COUNT_MAX )
	, m_CameraLerp					( FOR_INIT_ZERO_FLOAT )
	, m_NowSPCameraStete			( player::ESPCameraState_Start )
	, m_ParalysisCameraShakeCount	( FOR_INIT_ZERO_INT )
	, m_OneStepCmaeraSeting			( false )
{
	m_ObjectTag = EObjectTag::Player;	// プレイヤータグを設定.
	m_NowAnimNo = player::EAnimNo_Wait;	// 現在のアニメーションを待機に設定.
	m_OldAnimNo = player::EAnimNo_None;	// 過去のアニメーションは無し.
	m_pCamera	= std::make_shared<CRotLookAtCenter>( true );
	m_pSPCamera = std::make_shared<CCamera>();
	m_AnimFrameList.resize( player::EAnimNo_Max );
	for( auto& e : m_pEffectTimers ) e = std::make_shared<CEffectTimer>();
}

CPlayer::~CPlayer()
{
}

// 初期化関数.
bool CPlayer::Init()
{
	// パラメータの取得.
	if( ParameterSetting( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
	if( GetModel( MODEL_NAME )		== false ) return false;	// モデルの取得.
	if( SetAnimFrameList()			== false ) return false;	// アニメーションフレームリストの設定.
	if( FootStepCollisionSetting()	== false ) return false;	// 足音用当たり判定の設定.
	if( ColliderSetting()			== false ) return false;	// 当たり判定の設定.
	if( WidgetSetting()				== false ) return false;	// UIの設定.
	if( EffectSetting()				== false ) return false;	// エフェクトの設定.
	if( SoundSetting()				== false ) return false;	// サウンドの設定.
	m_MoveSpeed		= m_Parameter.MoveSpeed;	// 移動速度の設定.
	m_AttackPower	= m_Parameter.AttackPower;	// 攻撃力の設定.
	m_LifePoint		= m_Parameter.LifeMax;		// 体力の設定.
	m_SpecialAbilityValue	= m_Parameter.SpecialAbilityValue;	// 特殊能力回復値の設定.
	m_CameraLookPosition	= { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_CameraLerp			= m_Parameter.CameraLerpValue;

	m_ParalysisCameraShakeCount = FPS*2;

	// 待機アニメーションに変更.
	m_pSkinMesh->ChangeAnimSet_StartPos( player::EAnimNo_Wait, 0.0 );

	// プレイヤーの回転ベクトルを取得.
	m_AttackVector = { sinf(m_vRotation.y), FOR_INIT_ZERO_FLOAT, cosf(m_vRotation.y), };

	return true;
}

// 更新関数.
void CPlayer::Update()
{
	// アニメーションフレームの更新.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
//	if( m_IsHitStop == false ){
	// 麻痺タイマーが動作してなければ.
	if( m_pEffectTimers[player::EEffectTimerNo_Paralysis]->IsUpdate() == false ){
		Controller();			// 操作.
		AttackController();		// 攻撃操作.
		SPController();			// 特殊能力操作.
		AvoidController();		// 回避操作.
		AttackAnimation();		// 攻撃アニメーション.
		Move();					// 移動.
		AttackMove();			// 攻撃移動.
		AvoidMove();			// 回避動作.
		KnockBack();			// ノックバック動作関数.
		Dead();					// 死亡関数.
		m_HitStopCameraPosition = m_pCamera->GetPosition();
	} else {
		ParalysisUpdate();		// 麻痺時の更新.
	}
	CameraController();			// カメラ操作.
	SPCameraUpdate();			// 特殊能力時のカメラ動作.
	SpecialAbilityUpdate();		// 特殊能力回復更新.
	AttackUpUpdate();			// 攻撃力UP更新.
	MoveSpeedUpUpdate();		// 移動速度UP更新.

	CameraUpdate();				// カメラの更新.
//	} else {
//		HitStopUpdate();			// ヒットストップの更新.
//	}


	// 攻撃範囲のフラグを下す.
	bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_AttackRange );
	m_AttackRangeLenght = FOR_INIT_ZERO_FLOAT;	// 攻撃範囲の長さを初期化.

	// 体力が1/4になったらSEを鳴らす.
	if (m_LifePoint <= m_Parameter.LifeMax * PLAY_WARNING_SE_LIFE_ONE_THIRD)
	{
		if (CSoundManager::GetIsPlaySE("HP", 0) == false) {
			CSoundManager::PlaySE("HP");
		}
	}
	// 足音.
	FootStep(RIGHT_FOOT_BONE_NAME, LEFT_FOOT_BONE_NAME);
}

// 描画関数.
void CPlayer::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render();

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_AttackDataQueue.empty() == true ) return;
	if( m_AnimFrameList[m_NowAnimNo].NowFrame >= m_AttackDataQueue.front().AttackCollEndFrame ) return;
	if( m_pAttackCollManager == nullptr ) return;
	m_pAttackCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CPlayer::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	AttackCollision( pActor );		// 攻撃時の当たり判定.
	AttackRangeDecision( pActor );	// 攻撃範囲の判定.
}

// 相手座標の設定関数.
void CPlayer::SetTargetPos( CActor& actor )
{
	if( actor.GetObjectTag() != EObjectTag::Girl ) return;
	m_GirlPosition = actor.GetPosition();
}

// スプライトの描画.
void CPlayer::SpriteRender()
{
	// ゲージ.
	CCharacterWidget::SCharacterParam param;
	// ライフ.
	param.Life = m_LifePoint;
	param.LifeMax = m_Parameter.LifeMax;
	// 特殊能力.
	param.SpecialAbility = m_SpecialAbility;
	param.SpecialAbilityMax = m_Parameter.SpecialAbilityMax;
	for (const auto& s : m_pWidget)
	{
		s->SetParameter(param);
		s->Update();
		s->Render();
	}

#if _DEBUG
	// エディット用の描画関数をエディットレンダラーに追加.
	CEditRenderer::PushRenderProc( [&](){ EditRender(); } );
#endif	// #if _DEBUG.
}

// 特殊能力を使っているか.
bool CPlayer::IsSpecialAbility()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_UsableSP )  == false ) return false;
	// 特殊能力が使えるなら.
	bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_UsableSP );
	return true;			// trueを返す.
}

// カメラの方向.
float CPlayer::GetCameraRadianX()
{
	return m_pCamera->GetRadianX();
}

// アニメーションを再開する.
void CPlayer::ResumeAnimation()
{
	// 設定画面に行った後カメラの設定が変わっているかもしれないので.
	// 再度読み込む.
	if( m_pCamera->LoadParameter( true ) == false ){}
	m_AnimSpeed = m_OldAnimSpeed;
}

// カメラを揺らす.
bool CPlayer::CameraShake()
{
	const float SHAKE_VALUE = 
		cosf(static_cast<float>(D3DX_PI) * TWO / 15.0f * m_ParalysisCameraShakeCount) * (m_ParalysisCameraShakeCount * 0.01f);
	const D3DXVECTOR3 lookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_pCamera->SetAddPositionValue( { SHAKE_VALUE, FOR_INIT_ZERO_FLOAT, SHAKE_VALUE } );
	m_ParalysisCameraShakeCount--;
	if( m_ParalysisCameraShakeCount < 0 ){
		m_ParalysisCameraShakeCount = FPS*2;
		return false;
	}
	return true;
}

// パラメータの設定(editなどで使用).
void CPlayer::SetParameter( const player::SPlayerParam& param, const bool& isEdit )
{
	m_Parameter				= param;
	m_MoveSpeed				= m_Parameter.MoveSpeed;			// 移動速度の設定.
	m_AttackPower			= m_Parameter.AttackPower;			// 攻撃力の設定.
	m_LifePoint				= m_Parameter.LifeMax;				// 体力の設定.
	m_SpecialAbilityValue	= m_Parameter.SpecialAbilityValue;	// 特殊能力回復値の設定.
	m_CameraLookPosition	= { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_CameraLerp			= m_Parameter.CameraLerpValue;
	if( isEdit == true ) m_SpecialAbility = m_Parameter.SpecialAbilityMax;
	ColliderSetting();
}

// 操作関数.
void CPlayer::Controller()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== true ) return;	// ノックバック中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true ) return;	// 死亡中は終了.

	// コントローラーのLスティックの傾きを取得.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	if( GetAsyncKeyState('W') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('S') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState('D') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('A') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MIN;
}

// カメラ操作.
void CPlayer::CameraController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove )	== true ) return;	// SPカメラの動作中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// 死亡中は終了.
	// カメラの回転移動.
	// 横方向.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalRightMove();	// 右方向.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalLeftMove();	// 左方向.

	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 ) 
		m_pCamera->DegreeHorizontalRightMove();	// 右方向.
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )
		m_pCamera->DegreeHorizontalLeftMove();	// 左方向.
}

// 攻撃操作関数.
void CPlayer::AttackController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// 回避中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// ノックバック中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove )	== true ) return;	// SPカメラの動作中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// 死亡中は終了.

	// Xボタンを押した瞬間になれば.
	if( CInput::IsMomentPress( EKeyBind::Attack ) == true ){
		// 攻撃カウントが最大以上なら終了.
		if( m_AttackComboCount >= m_Parameter.AttackComboMax ) return;
		m_AttackComboCount++;	// 攻撃カウントを加算.
		// 攻撃データがキューに追加されたら終了.
		if( IsPushAttack() == true ){
			// プレイヤーの索敵範囲外なら終了.
			if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_AttackRange ) == true ){
				m_vRotation.y = atan2f( m_AttackVector.x, m_AttackVector.z );
			} else {
				// プレイヤーの回転ベクトルを取得.
				m_AttackVector = { sinf(m_vRotation.y), FOR_INIT_ZERO_FLOAT, cosf(m_vRotation.y), };
			}
			return;
		}
		m_AttackComboCount--;	// 攻撃カウントを減算.
	}
}

// 特殊能力操作関数.
void CPlayer::SPController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack ) == true ) return;
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid ) == true ) return;
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;
	if( m_AttackComboCount > player::EAttackNo_None ) return;
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead ) == true ) return;

	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	// Yボタンが押された瞬間になれば.
	if( CInput::IsMomentPress( EKeyBind::SpecialAbility ) == true ){
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_EndSPCameraMove );
		m_CameraPosition	= m_pCamera->GetPosition();
		m_SpecialAbility	= FOR_INIT_ZERO_FLOAT;

		SetAnimationBlend( player::EAnimNo_Wait );	// 待機アニメーションを設定.
		CSoundManager::PlaySE("PlayerVoiceSpecial");
	}
}

// 回避操作関数.
void CPlayer::AvoidController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// 回避中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// ノックバック中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;	// SPカメラ中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// 死亡中は終了.
// 攻撃中に回避ができないと、敵からの攻撃が避けにくくなり,
//	死亡しやすくなるので回避できるように以下の分をコメントアウト.
//	if( m_AttackComboCount > player::EAttackNo_None ) return;	// 攻撃中は発動しない.

	// 既に回避アニメーションだったら終了.
	if( m_NowAnimNo == player::EAnimNo_Avoid ) return;

	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// Aボタンが押された瞬間になれば.
	if( CInput::IsMomentPress( EKeyBind::Avoidance ) == true ){
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_DuringAvoid );	// 回避フラグを立てる.
		m_AvoidVector = m_MoveVector;	// 移動ベクトルを設定.
		m_pEffects[player::EEffectNo_Avoidance]->Play( { m_vPosition.x, m_vPosition.y+AVOID_EFFECT_RENDER_HEIGHT, m_vPosition.z } );
		// 回避アニメーションの設定.
		SetAnimationBlend( player::EAnimNo_Avoid );
		// 攻撃キューを全部取り出す.
		for( int i = 0; i < static_cast<int>(m_AttackDataQueue.size()); i++ ) m_AttackDataQueue.pop();
		m_AttackComboCount = player::EAttackNo_None;
		CSoundManager::PlaySE("PlayerAvoidMove");
		CSoundManager::PlaySE("PlayerVoiceAvoidMove");
	}
}

// 移動関数.
void CPlayer::Move()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// ノックバック中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// 回避中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;	// SPカメラの動作中は終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// 死亡中は終了.
	if( m_AttackComboCount > player::EAttackNo_None ) return;	// 攻撃中は終了.

	// 各値が有効範囲外なら終了.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
		m_MoveSpeedMulValue = FOR_INIT_ZERO_FLOAT;	// 初期化.
		if( m_NowAnimNo == player::EAnimNo_Attack1 )	return;	// アニメーションが攻撃1,2,3の時は.
		if( m_NowAnimNo == player::EAnimNo_Attack2 )	return;	//　待機モーションに設定できないようにする.
		if( m_NowAnimNo == player::EAnimNo_Attack3 )	return;	// 
		SetAnimationBlend( player::EAnimNo_Wait );	// 待機アニメーションを設定.
		return;
	}

	// 掛け合わせる移動量の加算.
	m_MoveSpeedMulValue += m_Parameter.MoveSpeedMulAddValue;
	// 一定値を超えないようにする.
	if( m_MoveSpeedMulValue >= m_Parameter.MoveSpeedMulMaxValue ) m_MoveSpeedMulValue = m_Parameter.MoveSpeedMulMaxValue;

	// ターゲットのベクトルを用意 カメラのラジアン値を足して調整.
	const float targetRot = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
	D3DXVECTOR3 targetVec = FOR_INIT_ZERO_VEC3;
	targetVec.x = sinf( targetRot );
	targetVec.z = cosf( targetRot );

	if( TargetRotation( targetVec, m_Parameter.RotationSpeed, m_Parameter.ToleranceRadian ) == true ){
		// ターゲットの回転を取得.
		m_vRotation.y = targetRot;

		// 回転軸で移動.
		m_vPosition.x -= targetVec.x * m_MoveSpeed * m_MoveSpeedMulValue;
		m_vPosition.z -= targetVec.z * m_MoveSpeed * m_MoveSpeedMulValue;
		// 見えない壁との当たり判定.
		if( CActor::IsCrashedWallX() == true ) m_vPosition.x += targetVec.x * m_MoveSpeed * m_MoveSpeedMulValue;
		if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += targetVec.z * m_MoveSpeed * m_MoveSpeedMulValue;

		if( m_NowAnimNo == player::EAnimNo_Attack1 )	return;	// アニメーションが攻撃1,2,3の場合は、
		if( m_NowAnimNo == player::EAnimNo_Attack2 )	return;	//　移動しないので、
		if( m_NowAnimNo == player::EAnimNo_Attack3 )	return;	//	アニメーションを設定せずに終了.
		SetAnimationBlend( player::EAnimNo_Walk );
	}
}

// 攻撃時の移動.
void CPlayer::AttackMove()
{
	if( m_AttackComboCount <= player::EAttackNo_None ) return;	// 攻撃カウントが無ければ終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid ) == true ) return;	// 回避中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== true ) return;	// ノックバック中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true ) return;	// 死亡中なら終了.

	int attackNo = FOR_INIT_ZERO_INT;
	switch( m_NowAnimNo )
	{
	case player::EAnimNo_Attack1:	attackNo = player::EAttackNo_One-1;		break;
	case player::EAnimNo_Attack3:	attackNo = player::EAttackNo_Two-1;		break;
	case player::EAnimNo_Attack2:	attackNo = player::EAttackNo_Three-1;	break;
	default: break;
	}
	// 攻撃アニメーションのずれ(引きずり)を調整.
	AnimationDraggingAdj( attackNo );
}

// 回避動作関数.
void CPlayer::AvoidMove()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )	== false ) return;	// 回避してなければ終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true ) return;	// 死亡していれば終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== true ) return;	// ノックバックしていれば終了.

	// スティックの傾いた方向に向く.
	m_vRotation.y = atan2f( m_AvoidVector.x, m_AvoidVector.z );
	// カメラの角度と足し合わせる.
	m_vRotation.y += m_pCamera->GetRadianX();

	// 回転軸で移動.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	// 見えない壁との当たり判定.
	if( CActor::IsCrashedWallX() == true ) m_vPosition.x += sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;

	if( m_AnimFrameList[player::EAnimNo_Avoid].IsNowFrameOver() == false ) return;
	// 回避アニメーションの経過フレームが終了フレームを超えていたら.
	// 回避中じゃなくする.
	bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_DuringAvoid );
}

// ノックバック動作関数.
void CPlayer::KnockBack()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== false )	return;	// ノックバックしてなければ終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true )	return;	// 死亡中なら終了.

	m_vPosition.x -= m_MoveVector.x*m_Parameter.HitKnocBackMoveSpeed;
	m_vPosition.z -= m_MoveVector.z*m_Parameter.HitKnocBackMoveSpeed;

	// 見えない壁との当たり判定.
	if( CActor::IsCrashedWallX() == true ) m_vPosition.x += m_MoveVector.x*m_Parameter.HitKnocBackMoveSpeed;
	if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += m_MoveVector.z*m_Parameter.HitKnocBackMoveSpeed;

	if( m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver() == true ){
		bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_KnockBack );	// ノックバックを止める.
	}
}

// 死亡動作関数.
void CPlayer::Dead()
{
	// 死亡してなければ終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead ) == false ) return;

	// アニメーションのずれを調整している場合、アニメーション速度をゆっくりにする.
	if( AnimationDraggingAdj( player::EDraggingAdjList_Dead ) == true ) m_AnimSpeed = DEFAULT_ANIM_HALF_SPEED;

	// アニメーションを再生させないようにする.
	if( m_AnimFrameList[player::EAnimNo_Dead].IsNowFrameOver() == true ) STOP_ANIM_SPEED;
}

// カメラの更新.
void CPlayer::CameraUpdate()
{
	// 死亡中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead ) == true ) return;

	if( m_OneStepCmaeraSeting == false ){
		m_pCamera->SetPosition( CCameraManager::GetPosition() );
		m_OneStepCmaeraSeting = true;
	}


	// プレイヤーを注視して回転.
	m_pCamera->RotationLookAtObject( { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z }, m_CameraLerp );
	m_pCamera->SetLength( m_Parameter.CameraDistance );	// 中心との距離を設定.
	m_pCamera->SetHeight( m_Parameter.CameraHeight );	// 高さの設定.

	// Yボタン(特殊能力が使われていなければ).
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == false ){
		m_CameraLerp = m_Parameter.CameraLerpValue;
		// メインカメラをマネージャーに設定.
		CCameraManager::SetCamera( m_pCamera );
	} else {
		// 特殊能力用のカメラをマネージャーに設定.
		CCameraManager::SetCamera( m_pSPCamera );
	}
}

// エフェクト描画関数.
void CPlayer::EffectRender()
{
	// 攻撃カウントが一定値より多ければ.
	if( m_AttackComboCount > player::EAttackNo_None ){
		// 攻撃エフェクトの座標を設定する.
		m_pEffects[m_AttackComboCount-1]->SetLocation( m_vPosition );
	}
	// エフェクトの描画.
	for( auto& e : m_pEffects ){
		e->SetLocation( m_vPosition );
		e->SetRotation( { FOR_INIT_ZERO_FLOAT, static_cast<float>(D3DX_PI)+m_vRotation.y, FOR_INIT_ZERO_FLOAT } );
		e->Render();
	}
}

// 攻撃の当たり判定.
void CPlayer::AttackCollision( CActor* pActor )
{
	// 攻撃用当たり判定座標をゲーム範囲外に設定.
	m_AttackPosition = m_AttackAdjParam.CollInvalidPosition;
	// 攻撃してない場合なら終了.
	if( m_AttackComboCount <= player::EAttackNo_None ) return;
	// 現在のアニメーションフレームが当たり判定の有効範囲外なら終了.
	if( m_AnimFrameList[m_NowAnimNo].NowFrame >= m_AttackDataQueue.front().AttackCollEndFrame ) return;
	if( pActor->IsPossibleToHit() == false ) return;

	D3DXVECTOR3 littleFingerPos	= FOR_INIT_ZERO_VEC3;	// 小指ボーンの座標.
	D3DXVECTOR3 ringFingerPos	= FOR_INIT_ZERO_VEC3;	// 薬指のボーン座標.
	// 指のボーン座標を取得.
	m_pSkinMesh->GetPosFromBone( LITTLE_FINGER_BONE_NAME, &littleFingerPos );
	m_pSkinMesh->GetPosFromBone( RING_FINGER_BONE_NAME, &ringFingerPos );
	// 指のベクトルを取得して当たり判定の座標を計算.
	D3DXVECTOR3 fingerVec = ringFingerPos - littleFingerPos;
	D3DXVec3Normalize( &fingerVec, &fingerVec );
	m_AttackPosition = littleFingerPos + fingerVec * m_AttackAdjParam.CollisionDistance;

	// 球体の当たり判定.
	if( m_pAttackCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// ノックバック用のベクトルを計算.
	D3DXVECTOR3 vec =
	{
		m_AttackPosition.x - pActor->GetPosition().x,
		FOR_INIT_ZERO_FLOAT,
		m_AttackPosition.z - pActor->GetPosition().z
	};
	D3DXVec3Normalize( &vec, &vec );
	vec.x *= m_AttackComboCount*m_Parameter.EnemyKnockBackPower;
	vec.z *= m_AttackComboCount*m_Parameter.EnemyKnockBackPower;
	pActor->SetVector( vec );

	// 攻撃関数.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{ 
		life	-= m_AttackPower;
		isAttack = true;
	});

//	if( m_IsHitStop == false ){
//		m_IsHitStop		= true;
//		m_HitStopTime	= m_AttackComboCount*2;
//		m_AnimSpeed		= 0.0;
//		m_HitStopCameraPosition = m_pCamera->GetPosition();
//	}
//	pActor->SetHitStopTime( m_HitStopTime );
	
	// 音声を鳴らしてない状態なら.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_AttackSE ) == false ){
		CSoundManager::PlaySE("PlayerAttackHit");
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_AttackSE );
	}
}

// 攻撃範囲との判定.
void CPlayer::AttackRangeDecision( CActor* pActor )
{
	// プレイヤーと宇宙人とのベクトルを取得.
	D3DXVECTOR3 vec =
	{
		m_vPosition.x - pActor->GetPosition().x,
		FOR_INIT_ZERO_FLOAT,
		m_vPosition.z - pActor->GetPosition().z,
	};
	// ベクトルの長さ算出.
	const float vecLength = sqrtf((vec.x * vec.x) + (vec.z * vec.z));
	float rot = m_vRotation.y;
	// 各値が有効範囲内なら移動ベクトルから回転値を算出.
	if( m_MoveVector.x >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.x ||
		m_MoveVector.z >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.z ){
		rot = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
	}
	// プレイヤーの回転ベクトルを取得.
	const D3DXVECTOR3 playerVec = { sinf(rot), FOR_INIT_ZERO_FLOAT, cosf(rot), };

	// 宇宙人との距離が一定範囲外なので終了.
	if( vecLength > m_Parameter.AttackSearcLenght ) return;

	// 単位ベクトルに変換.
	D3DXVec3Normalize( &vec, &vec );

	// 内積を取得.
	const float dot = vec.x * playerVec.x + vec.z * playerVec.z;

	// 点が範囲外であれば終了.
	if( dot < cos(m_Parameter.AttackFanRadian) ) return;

	// 長さが初期化状態なら長さを入れる.
	if( m_AttackRangeLenght <= 0.0f ) m_AttackRangeLenght = dot;
	// 前回の長さが今回の長さより大きければ終了.
	if( m_AttackRangeLenght < dot ) return;

	m_AttackRangeLenght	= dot;
	m_AttackVector		= vec;
	bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_AttackRange );

}

// 特殊能力時のカメラ動作.
void CPlayer::SPCameraUpdate()
{
	// 特殊能力が使えないなら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == false ){
		m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		return;
	}

	m_CameraLookPosition = m_GirlPosition;	// 女の子を視点にする.
	// 回転値を設定する.
	const float targetRot = atan2f( 
		m_vPosition.x - m_CameraLookPosition.x,
		m_vPosition.z - m_CameraLookPosition.z );

	switch( m_NowSPCameraStete )
	{
	case player::ESPCameraState_TargetRotation:
	{
		//-------------------------------------.
		// プレイヤーを女の子のほうへ向ける.
		//-------------------------------------.
		// 目的のベクトルを用意.
		m_TargetVector.x = sinf( targetRot );
		m_TargetVector.z = cosf( targetRot );
		
		// 目的の座標に向けて回転.
		if( TargetRotation( m_TargetVector, m_Parameter.RotationSpeed, m_Parameter.ToleranceRadian ) == true ){
			m_vRotation.y		= targetRot;							// ベクトルの回転を取得.
			m_NowSPCameraStete	= player::ESPCameraState_PlayerBack;	// 次の状態へ移動.
		} else {
			// 回転中はプレイヤーを視点に設定.
			m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		}
		break;
	}
	case player::ESPCameraState_PlayerBack:
	{
		//-------------------------------------.
		// カメラをプレイヤーの後ろに移動させる.
		//-------------------------------------.
		m_vRotation.y = targetRot;	// ベクトルの回転を取得.
		// カメラの座標を設定する.
		m_CameraNextPosition = m_vPosition;
		m_CameraNextPosition.x += m_TargetVector.x * CAMERA_BACK_DIRECTION_X;
		m_CameraNextPosition.z += m_TargetVector.z * CAMERA_BACK_DIRECTION_Z;
		m_CameraNextPosition.y = CAMERA_BACK_HEIGHT;
		// プレイヤーの後ろに移動.
		D3DXVec3Lerp( &m_CameraPosition, &m_CameraPosition, &m_CameraNextPosition, CAMERA_BACK_LERP_VALUE );
		if( fabsf(D3DXVec3Length(&m_CameraPosition) - D3DXVec3Length(&m_CameraNextPosition)) < 0.01f ){
			m_NowSPCameraStete = player::ESPCameraState_CameraShake;	// 次の状態へいどう.
			SetAnimationBlend( player::EAnimNo_SP );
		}
	}
		break;
	case player::ESPCameraState_CameraShake:
	{
		//-------------------------------------.
		// カメラを揺らす.
		//-------------------------------------.

		if( m_AnimFrameList[player::EAnimNo_SP].NowFrame >= m_AnimFrameList[player::EAnimNo_SP].EndFrame-0.5 ){
			bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_UsableSP );	// SPを使えるようにする.
			m_AnimSpeed = STOP_ANIM_SPEED;
			m_AnimFrameList[player::EAnimNo_SP].UpdateFrame( DEFAULT_ANIM_SPEED );
		}
		if( m_AnimFrameList[player::EAnimNo_SP].IsNowFrameOver() == true ){
			m_CameraCount--;	// カウントの減算.
			// カメラの揺れ.
			const float SHAKE_VALUE = sinf(static_cast<float>(D3DX_PI) * TWO / CAMERA_FREQUENCY_LOOKPOS * m_CameraCount) * (m_CameraCount * CAMERA_AMPLITUDE_LOOKPOS);
			m_CameraLookPosition.x += SHAKE_VALUE;
			m_CameraLookPosition.y += SHAKE_VALUE;
		}
		if( m_CameraCount <= 20 ) m_AnimSpeed = DEFAULT_ANIM_SPEED;
		
		// カメラカウントが0以下になったら.
		if( m_CameraCount <= 0 ){
			m_CameraCount			= CAMERA_COUNT_MAX;			// カウントを初期化.
			m_CameraNextPosition	= m_pCamera->GetPosition();	// メインカメラの座標を設定.
			m_CameraReturnCount		= FOR_INIT_ZERO_FLOAT;		// カメラを戻すカウントを初期化.
			m_NowSPCameraStete		= player::ESPCameraState_CameraReturn;	// 次の状態へ移動.
			m_AnimSpeed				= DEFAULT_ANIM_SPEED;
			SetAnimationBlend( player::EAnimNo_Wait );
		}
	}
		break;
	case player::ESPCameraState_CameraReturn:
	{
		//-------------------------------------.
		// カメラをもとの位置に戻す.
		//-------------------------------------.
		m_CameraReturnCount		+= CAMERA_RETURN_COUNT_ADD;	// カメラを戻すカウントを加算.
		// 一定値こ超えないようにする.
		if( m_CameraReturnCount >= CAMERA_RETURN_COUNT_MAX ) m_CameraReturnCount = CAMERA_RETURN_COUNT_MAX;
		// 視点をプレイヤーに設定.
		m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		// プレイヤーの後ろに移動.
		D3DXVec3Lerp( &m_CameraPosition, &m_CameraPosition, &m_CameraNextPosition, m_CameraReturnCount );
		// カメラを戻すカウントが一定値以上なら.
		if( m_CameraReturnCount >= CAMERA_RETURN_COUNT_MAX ){
			m_pCamera->SetLookPosition( m_CameraLookPosition );	// メインカメラの視点座標を設定.
			m_pCamera->SetPosition( m_CameraPosition );			// メインカメラの座標を設定.
			bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_EndSPCameraMove );	// SPカメラのフラグを下す.
			m_CameraReturnCount = FOR_INIT_ZERO_FLOAT;			// カメラを戻すカウントを初期化.
			m_CameraLerp		= FOR_INIT_ZERO_FLOAT;			// カメラの補完値を初期化.
			m_NowSPCameraStete	= player::ESPCameraState_Start;	// 初めの状態へ戻す.

			return;	// 特殊カメラは設定しなくてもよいのでここで終了.
		}
	}
		break;
	default:
		break;
	}
	// 特殊能力カメラ用の座標と視点座標を設定.
	m_pSPCamera->SetLookPosition( m_CameraLookPosition );
	m_pSPCamera->SetPosition( m_CameraPosition );
}

// ヒットストップの更新.
void CPlayer::HitStopUpdate()
{
	//if( m_IsHitStop == false ) return;

	//m_HitStopCount++;
	//const float shakeValue = 
	//	sinf( static_cast<float>(D3DX_PI)*TWO / static_cast<float>(m_HitStopCount)*TWO ) *
	//	static_cast<float>(m_HitStopTime*m_HitStopTime)/TWO * 0.08f;

	//const D3DXVECTOR3 lookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	//D3DXVECTOR3 vec = lookPosition - m_pCamera->GetPosition();
	//D3DXVec3Normalize( &vec, &vec );
	//m_HitStopCameraPosition.x += vec.x * shakeValue;
	//m_HitStopCameraPosition.z += vec.z * shakeValue;

	//m_pSPCamera->SetLookPosition( lookPosition );
	//m_pSPCamera->SetPosition( m_HitStopCameraPosition );
	//// 特殊能力用のカメラをマネージャーに設定.
	//CCameraManager::SetCamera( m_pSPCamera );

	//if( m_HitStopCount < m_HitStopTime ) return;

	//m_IsHitStop		= false;
	//m_HitStopCount	= 0;
	//m_AnimSpeed		= DEFAULT_ANIM_SPEED;
}

// 特殊能力回復更新関数.
void CPlayer::SpecialAbilityUpdate()
{							
	// アイテムでの回復状態なら.
	if( m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->Update() == true ){
		m_SpecialAbilityValue = m_Parameter.SpecialAbilityValue;	// 回復値をもとに戻す.
	}

	// 特殊能力値が最大以上なら終了.
	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_SpecialAbility += m_SpecialAbilityValue;	// 特殊能力値を加算.

	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	m_SpecialAbility = m_Parameter.SpecialAbilityMax;	// 最大値を超えないようにする.
}

// 攻撃力UP更新関数.
void CPlayer::AttackUpUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_Attack]->Update() == false ) return;
	// タイマーが終了したら元の値に戻す.
	m_AttackPower = m_Parameter.AttackPower;
}

// 移動速度UP更新関数.
void CPlayer::MoveSpeedUpUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->Update() == false ) return;
	// タイマーが終了したら元の値に戻す.
	m_MoveSpeed	= m_Parameter.MoveSpeed;
}

// 麻痺中の更新関数.
void CPlayer::ParalysisUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_Paralysis]->Update() == false ) return;
	m_AnimSpeed = DEFAULT_ANIM_SPEED;
	m_pEffects[player::EEffectNo_Paralysis]->Stop();
}

// 攻撃アニメーション.
void CPlayer::AttackAnimation()
{
	// キューが空なら終了.
	if( m_AttackDataQueue.empty() == true ) return;
	if( m_AttackDataQueue.front().Frame >= m_AttackDataQueue.front().EndFrame ){
		m_AttackDataQueue.pop();	// キューの先頭を取り出す.
		// キューが空なら終了.
		if( m_AttackDataQueue.empty() == true ){
			// これが最後の攻撃なので、攻撃カウントを0にする.
			m_AttackComboCount = player::EAttackNo_None;	
			// 待機モーションか、移動モーションにするかの比較.
			// 各値が有効範囲外なら終了.
			if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
				m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
				// アニメーションを待機に設定.
				SetAnimationBlend( player::EAnimNo_Wait );
			} else {
				// アニメーションを移動に設定.
				SetAnimationBlend( player::EAnimNo_Walk );
			}
			return;
		}
		// エフェクトを再生.
		m_pEffects[m_AttackComboCount-1]->Play( {m_vPosition.x, m_vPosition.y+ATTACK_EFFECT_RENDER_HEIGHT, m_vPosition.z } );
		// 攻撃SEを鳴らす.
		CSoundManager::PlaySE("PlayerAttack");
		if(m_AttackComboCount == player::EAttackNo_Two){
			CSoundManager::PlaySE("PlayerVoiceAttack2");
		}
		if(m_AttackComboCount == player::EAttackNo_Three){
			CSoundManager::PlaySE("PlayerVoiceAttack3");
		}
		// 攻撃の当たり判定のサイズを変更する.
		if( FAILED( m_pAttackCollManager->InitSphere(
			&m_AttackPosition,
			&m_vRotation,
			&m_vScale.x,
			FOR_INIT_ZERO_VEC3,
			m_AttackAdjParam.CollisionRadius[m_AttackComboCount-1] ) )) return;
		// 各値が有効範囲内ならベクトルから回転値を入れる.
		if( m_MoveVector.x >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.x ||
			m_MoveVector.z >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.z ){
			// ターゲットのベクトルを用意 カメラのラジアン値を足して調整.
			m_vRotation.y = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
		}
		// 新しくアニメーションをセットする.
		SetAnimation( m_AttackDataQueue.front().AnimNo );
		bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_AttackSE );
	}
	m_AttackDataQueue.front().Frame += m_AnimSpeed;	// フレームの更新.
}

// 攻撃の追加ができたか.
bool CPlayer::IsPushAttack()
{
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackComboMax ) return false;
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackQueueMax ) return false;
	if( m_AttackDataQueue.empty() == false ){
		// キューにデータが入っていたら.
		// 経過フレームが攻撃有効フレームより大きければ終了.
		if( m_AttackDataQueue.front().Frame >= 
			m_AttackDataQueue.front().EnabledEndFrame ) return false;
	}

	player::SAttackData tmpAttackData;	// 仮データを用意.
	// 仮データの設定.
	const auto setAttackData = [&]( const player::EAnimNo& animNo, const int& atkNo )
	{
		tmpAttackData.AnimNo			= animNo;
		tmpAttackData.EnabledEndFrame	= m_AnimFrameList[animNo].EndFrame - m_AttackAdjParam.EnabledEndFrame[atkNo];
		tmpAttackData.EndFrame			= m_AnimFrameList[animNo].EndFrame;
		tmpAttackData.AttackCollEndFrame= m_AnimFrameList[animNo].EndFrame - m_AttackAdjParam.CollEnabledEndFrame[atkNo];
	};

	// "player::EAttackNo_One"が 数値"1"なので,
	//	-1して配列の添え字を調整する.
	const int attackAdjNo = m_AttackComboCount-1;

	switch( m_AttackComboCount )
	{
	case player::EAttackNo_One:	// 攻撃1.
		// 攻撃時の当たり判定を設定する.
		if( FAILED( m_pAttackCollManager->InitSphere(
			&m_AttackPosition,
			&m_vRotation,
			&m_vScale.x,
			FOR_INIT_ZERO_VEC3,
			m_AttackAdjParam.CollisionRadius[attackAdjNo] ) )) return false;
		setAttackData( player::EAnimNo_Attack1, attackAdjNo );
		// 最初の攻撃はアニメーションを設定する.
		SetAnimation( tmpAttackData.AnimNo );
		m_pEffects[player::EEffectNo_AttackOne]->Play( m_vPosition );
		CSoundManager::PlaySE("PlayerAttack");
		CSoundManager::PlaySE("PlayerVoiceAttack1");
		bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_AttackSE );
		break;

	case player::EAttackNo_Two:	// 攻撃2.
		setAttackData( player::EAnimNo_Attack2, attackAdjNo );
		break;

	case player::EAttackNo_Three:// 攻撃3.
		setAttackData( player::EAnimNo_Attack3, attackAdjNo );
		break;

	default:
		break;
	}
	
	// キューにデータを挿入.
	m_AttackDataQueue.push( tmpAttackData );
	
	return true;
}

// ライフ計算関数.
void CPlayer::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return; // SPカメラが動作しているなら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return; // ノックバック中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return; // 死亡中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return; // 回避中なら終了.
	
	bool isAttack = false;
	const float oldLifePoint = m_LifePoint;
	proc( m_LifePoint, isAttack );

	// 回避中だが攻撃を食らっていたら.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )	== true &&
		isAttack == true ){
		m_LifePoint = oldLifePoint;	// 前回の体力を入れる.
	}
	// 攻撃を食らったら.
	if( isAttack == true ){
		if (m_LifePoint > FOR_INIT_ZERO_FLOAT) CSoundManager::PlaySE("PlayerVoiceHit");
		// ダメージアニメーションを設定.
		SetAnimation( player::EAnimNo_Damage );
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_KnockBack );
		m_vRotation.y	= atan2( m_HitVector.x, m_HitVector.z )+static_cast<float>(D3DX_PI);
		m_MoveVector	= m_HitVector;
		// 攻撃キューを全部取り出す.
		for( int i = FOR_INIT_ZERO_INT; i < static_cast<int>(m_AttackDataQueue.size()); i++ ) m_AttackDataQueue.pop();
		m_AttackComboCount = player::EAttackNo_None;
	}
	// 体力がなくなったら.
	if( m_LifePoint <= 0.0f ){
		CSoundManager::PlaySE("PlayerVoiceDead");
		// 攻撃キューを全部取り出す.
		for( int i = FOR_INIT_ZERO_INT; i < static_cast<int>(m_AttackDataQueue.size()); i++ ) m_AttackDataQueue.pop();
		m_AttackComboCount = player::EAttackNo_None;
		// 死亡アニメーションを設定.
		SetAnimation( player::EAnimNo_Dead );
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_Dead );
		m_vRotation.y	= atan2( m_HitVector.x, m_HitVector.z )+static_cast<float>(D3DX_PI);
		m_MoveVector	= -m_HitVector;	// ベクトル値を反転して調整.
	}

	// 体力が一定値を超えないようにする.
	if( m_LifePoint < m_Parameter.LifeMax ) return;
	m_LifePoint = m_Parameter.LifeMax;
}

// 特殊能力回復時間、効力時間設定関数.
void CPlayer::SetSPEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->IsUpdate() == true ) return;

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( m_ItemSpecialAbilityValue, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->SetTime( tmpTime );

	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->Set();
}

// 攻撃力、効力時間設定関数.
void CPlayer::SetAttackEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_Attack]->IsUpdate() == true ) return;

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( m_AttackPower, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->Set();
}

// 移動速度、効力時間設定関数.
void CPlayer::SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->IsUpdate() == true ) return;

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( m_MoveSpeed, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->Set();
}

// 麻痺の設定.
void CPlayer::SetParalysisTime( const std::function<void(float&)>& proc )
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// 回避中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// ノックバック中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;	// SPカメラが動作中なら終了.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// 死亡中なら終了.

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->Set();
	m_pEffects[player::EEffectNo_Paralysis]->Play( m_vPosition );
	m_AnimSpeed = STOP_ANIM_SPEED;
	CSoundManager::NoMultipleSEPlay("PlayerVoiceParalysis");
}

// アニメーションのずれ(引きずり)を調整.
bool CPlayer::AnimationDraggingAdj( const int& animAdjNo )
{
	if( m_AnimAdjDrggingParam.StartFrame[animAdjNo] <= m_AnimFrameList[m_NowAnimNo].NowFrame && 
		m_AnimFrameList[m_NowAnimNo].NowFrame <= m_AnimAdjDrggingParam.EndFrame[animAdjNo] ){
		// アニメーションのずれを調整.
		m_vPosition.x -= sinf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];
		m_vPosition.z -= cosf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];

		// 見えない壁との当たり判定.
		if( CActor::IsCrashedWallX() == true ) m_vPosition.x += sinf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];
		if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += cosf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];

		return true;
	}
	return false;
}

// 当たり判定の設定.
bool CPlayer::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	if( FAILED( m_pCollManager->InitCapsule( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.CapsuleAdjRadius,
		FOR_INIT_ZERO_FLOAT ) )) return false;

	// 攻撃用の当たり判定初期化.
	if( m_pAttackCollManager == nullptr ){
		m_pAttackCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pAttackCollManager->InitSphere(
		&m_AttackPosition,
		&m_vRotation,
		&m_vScale.x,
		FOR_INIT_ZERO_VEC3,
		m_AttackAdjParam.CollisionRadius[0] ) )) return false;
	return true;
}

// エフェクトの設定.
bool CPlayer::EffectSetting()
{
	const char* effectNames[] =
	{
		ATTACK_ONE_EFFECT_NAME,
		ATTACK_TWO_EFFECT_NAME,
		ATTACK_THREE_EFFECT_NAME,
		SPECIAL_ABILITY_EFFECT_NAME,
		AVOIDANCE_EFFECT_NAME,
		PARALYSIS_EFFECT_NAME,
	};
	const int effectNum = sizeof(effectNames)/sizeof(effectNames[0]);
	// メモリの最大値設定.
	m_pEffects.reserve(effectNum);

	for( int i = FOR_INIT_ZERO_INT; i < effectNum; i++ ){
		m_pEffects.emplace_back( std::make_shared<CEffectManager>() );
		if( m_pEffects[i]->SetEffect( effectNames[i] ) == false ) return false;
	}

	return true;
}

// アニメーションフレームの設定.
bool CPlayer::SetAnimFrameList()
{
	if( m_pSkinMesh == nullptr ) return false;
	// 調整用アニメーションフレームのリストを用意.
	player::SAnimationAdjFrameList list;
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimAdjList.bin", list ) == false ) return false;

	for( int i = player::EAnimNo_Begin; i < player::EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-list.Frame[i] };
	}

	// アニメーションの引きずり調整リストを読み込む.
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam ) == false ) return false;
	// 攻撃調整用パラメーターを読み込む.
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam ) == false ) return false;

	return true;
}

// サウンドの設定.
bool CPlayer::SoundSetting()
{
	VolumeSetting("PlayerVoiceAvoidMove", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceHit",		  VOICE_VOLUME);
	VolumeSetting("PlayerVoiceParalysis", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceAttack1", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceAttack2", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceAttack3", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceSpecial", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceDead",	VOICE_VOLUME);
	return true;
}

// 音量の設定.
void CPlayer::VolumeSetting(const char * soung, float volume)
{
	CSoundManager::SetAnotherSEVolume(soung, volume);
	CSoundManager::SetUseAnotherSEVolumeFlag(soung, true);
}


// エディット用の描画関数.
void CPlayer::EditRender()
{
#if _DEBUG
	ImGui::SetNextWindowSize( ImVec2(440.0f,470.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( ImVec2(WND_W-440,0.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();
	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"プレイヤーの設定", &isOpen );

	if( ImGui::TreeNode( u8"パラメータの設定" )){
		CImGuiManager::DragFloat( u8"移動速度",						&m_Parameter.MoveSpeed );
		CImGuiManager::DragFloat( u8"体力",							&m_Parameter.LifeMax );
		CImGuiManager::DragFloat( u8"攻撃力",						&m_Parameter.AttackPower );
		CImGuiManager::DragInt(   u8"無敵時間",						&m_Parameter.InvincibleTime );
		CImGuiManager::DragFloat( u8"特殊能力最大値",				&m_Parameter.SpecialAbilityMax );
		CImGuiManager::DragFloat( u8"特殊能力回復値",				&m_Parameter.SpecialAbilityValue );
		CImGuiManager::DragInt(   u8"攻撃コンボ最大数",				&m_Parameter.AttackComboMax );
		CImGuiManager::DragInt(   u8"攻撃キュー追加最大数",			&m_Parameter.AttackQueueMax );
		CImGuiManager::DragFloat( u8"回避の移動距離",				&m_Parameter.AvoidMoveDistance );
		CImGuiManager::DragFloat( u8"回避用の移動速度",				&m_Parameter.AvoidMoveSpeed );
		CImGuiManager::DragFloat( u8"カメラの移動速度",				&m_Parameter.CameraMoveSpeed );
		CImGuiManager::DragFloat( u8"カメラの距離",					&m_Parameter.CameraDistance );
		CImGuiManager::DragFloat( u8"カメラの高さ",					&m_Parameter.CameraHeight );
		CImGuiManager::DragFloat( u8"スフィアの調整座標 X",			&m_Parameter.SphereAdjPos.x );
		CImGuiManager::DragFloat( u8"スフィアの調整座標 Y",			&m_Parameter.SphereAdjPos.y );
		CImGuiManager::DragFloat( u8"スフィアの調整座標 Z",			&m_Parameter.SphereAdjPos.z );
		CImGuiManager::DragFloat( u8"スフィアの調整半径",			&m_Parameter.SphereAdjRadius );
		CImGuiManager::DragFloat( u8"ノックバック移動速度",			&m_Parameter.HitKnocBackMoveSpeed );
		CImGuiManager::DragFloat( u8"回転の許容範囲",				&m_Parameter.ToleranceRadian );
		CImGuiManager::DragFloat( u8"回転速度",						&m_Parameter.RotationSpeed );
		CImGuiManager::DragFloat( u8"掛け合わせる移動量の加算値",	&m_Parameter.MoveSpeedMulAddValue );
		CImGuiManager::DragFloat( u8"掛け合わせる移動量の最大値",	&m_Parameter.MoveSpeedMulMaxValue );
		CImGuiManager::DragFloat( u8"攻撃の索敵距離",				&m_Parameter.AttackSearcLenght );
		CImGuiManager::DragFloat( u8"扇の攻撃範囲",					&m_Parameter.AttackFanRadian );
		CImGuiManager::DragFloat( u8"敵のノックバック力",			&m_Parameter.EnemyKnockBackPower );	
		
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"アニメーションの引きずり調整パラメータの設定" )){
		if( ImGui::TreeNode( u8"攻撃1" )){
			CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack1] );
			CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack1] );
			CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack1] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"攻撃2" )){
			CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack2] );
			CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack2] );
			CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack2] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"攻撃3" )){
			CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack3] );
			CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack3] );
			CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack3] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"死亡" )){
			CImGuiManager::DragDouble(	u8"開始フレーム",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Dead] );
			CImGuiManager::DragDouble(	u8"終了フレーム",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Dead] );
			CImGuiManager::DragFloat(	u8"調整用の移動速度",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Dead] );
			ImGui::TreePop();
		}
		
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"攻撃調整パラメータの設定" )){
		if( ImGui::TreeNode( u8"攻撃1" )){
			CImGuiManager::DragDouble(	u8"有効終了フレーム",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_One-1] );
			CImGuiManager::DragDouble(	u8"当たり判定有効終了フレーム",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_One-1] );
			CImGuiManager::DragFloat(	u8"当たり判定半径",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_One-1] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"攻撃2" )){
			CImGuiManager::DragDouble(	u8"有効終了フレーム",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Two-1] );
			CImGuiManager::DragDouble(	u8"当たり判定有効終了フレーム",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Two-1] );
			CImGuiManager::DragFloat(	u8"当たり判定半径",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Two-1] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"攻撃3" )){
			CImGuiManager::DragDouble(	u8"有効終了フレーム",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Three-1] );
			CImGuiManager::DragDouble(	u8"当たり判定有効終了フレーム",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Three-1] );
			CImGuiManager::DragFloat(	u8"当たり判定半径",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Three-1] );
			ImGui::TreePop();
		}
		
		ImGui::TreePop();
	}

	// パラメータの設定.
	SetParameter( m_Parameter );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"読込") == true ){
		// データの読み込み.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Parameter );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
		if( s_Success.IsSucceeded == true ){
			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"保存") == true ){
		// データの書き込み.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_Parameter );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// 成功かどうかを描画.

	ImGui::End();
#endif	// #if _DEBUG.
}

// ウィジェット設定.
bool CPlayer::WidgetSetting()
{
	m_pWidget.emplace_back(std::make_shared<CBackGauge>());
	m_pWidget.emplace_back(std::make_shared<CLifeGauge>());
	m_pWidget.emplace_back(std::make_shared<CSpecialAbilityGauge>());

	for (const auto& s : m_pWidget)
	{
		if (s->Init() == false) return false;
	}

	return true;
}