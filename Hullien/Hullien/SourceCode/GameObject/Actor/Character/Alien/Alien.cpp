#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Arm\Arm.h"

#define IS_TEMP_MODEL_RENDER

CAlien::CAlien()
	: m_pArm					( nullptr )
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter				()
	, m_NowState				( EAlienState::None )
	, m_NowMoveState			( EMoveState::None )
	, m_HasAnyItem				( EItemList::None )
	, m_LifePoint				( 0.0f )
	, m_ModelAlpha				( 0.0f )
	, m_WaitCount				( 0 )
	, m_pIsAlienOtherAbduct		( nullptr )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
	, m_IsRisingMotherShip		( false )
{
	m_vSclae = { 0.0f, 0.0f, 0.0f };
}

CAlien::~CAlien()
{
}

// 相手座標の設定.
void CAlien::SetTargetPos( CActor& actor )
{
	SetGirlPos( actor );
}

// ライフ計算関数.
void CAlien::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	if( m_NowState == EAlienState::Fright ) return;

	bool isAttack = false;
	proc( m_LifePoint, isAttack );
	m_NowState = EAlienState::Fright;	// 怯み状態へ遷移.

	if( m_LifePoint > 0.0f ) return;
	// 体力が 0.0以下なら死亡状態へ遷移.
	m_NowState = EAlienState::Death;
}

// 現在の状態の更新関数.
void CAlien::CurrentStateUpdate()
{
	switch( m_NowState )
	{
	case EAlienState::Spawn:
		this->Spawning();
		break;
	case EAlienState::Move:
		this->Move();
		break;
	case EAlienState::Abduct:
		this->Abduct();
		break;
	case EAlienState::Fright:
		this->Fright();
		break;
	case EAlienState::Death:
		this->Death();
		break;
	case EAlienState::Escape:
		this->Escape();
		break;
	default:
		break;
	}
}

// 女の子の座標を設定.
void CAlien::SetGirlPos( CActor& actor )
{
	if( m_NowMoveState == EMoveState::Move ) return;
	if( *m_pIsAlienOtherAbduct == true ) return;

	// 女の子じゃなければ終了.
	if( actor.GetObjectTag() != EObjectTag::Girl ) return;
	m_TargetPosition = actor.GetPosition();	// 女の子の座標を取得.
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f( 
		m_vPosition.x - m_TargetPosition.x,
		m_vPosition.z - m_TargetPosition.z );

	// 女の子と一定の距離置いた座標を設定.
	m_TargetPosition.x += sinf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
	m_TargetPosition.z += cosf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
}

// 座標設定関数.
void CAlien::SetPosition( const D3DXVECTOR3& vPos )
{
	if( *m_pIsAlienOtherAbduct == false ) return;
	m_vPosition				= vPos;
	m_IsRisingMotherShip	= true;
}

// 移動ベクトル設定関数.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f( 
		m_vPosition.x - targetPos.x,
		m_vPosition.z - targetPos.z );

	// 移動用ベクトルを取得.
	m_MoveVector.x = sinf( m_TargetRotation.y );
	m_MoveVector.z = cosf( m_TargetRotation.y );
}

// 目的の座標へ回転.
void CAlien::TargetRotation()
{
	if( m_NowMoveState != EMoveState::Rotation ) return;

	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// 目的の座標へ向けて回転.
	if( CCharacter::TargetRotation( m_MoveVector, ROTATIONAL_SPEED, TOLERANCE_RADIAN ) == false )
	m_vRotation.y			= m_TargetRotation.y;	// ターゲットへの回転取得.
	m_BeforeMoveingPosition = m_vPosition;			// 現在の座標を記憶.
	m_NowMoveState			= EMoveState::Move;		// 移動状態へ遷移.
}

// 移動関数.
void CAlien::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;
	
	// ベクトルを使用して移動.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// 再度目的の座標を探すか比較.
	// 回転時に記憶した座標と現在の座標の距離が一定以上なら.
	if( D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition-m_vPosition)) >= m_Parameter.ResearchLenght ) 
		m_NowMoveState = EMoveState::Rotation;	// 回転状態へ移動.

	if( D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition-m_vPosition)) >= 1.0f ) return;
	// 現在の座標と目的の座標の距離が一定以上なら.
	m_NowMoveState = EMoveState::Wait;	// 待機状態へ遷移.
}

// 待機関数.
void CAlien::WaitMove()
{
	if( m_NowMoveState != EMoveState::Wait ) return;
	m_WaitCount++;	// 待機カウント加算.
	if( m_WaitCount < m_Parameter.WaitTime*FPS ) return;
	m_NowMoveState	= EMoveState::Rotation;	// 移動状態を回転する.
	m_WaitCount		= 0;	// 待機カウントの初期化.
}

// スポーン中.
void CAlien::Spawning()
{
	m_vPosition.y -= 0.1f;
	if (m_ModelAlpha < MODEL_ALPHA_MAX) {
		m_vSclae.x += m_Parameter.ModelAlphaAddValue;
		m_vSclae.y += m_Parameter.ModelAlphaAddValue;
		m_vSclae.z += m_Parameter.ModelAlphaAddValue;
		// モデルのアルファ値を足していく.
		m_ModelAlpha += m_Parameter.ModelAlphaAddValue;
	}

	if( m_vPosition.y > 4.0f ) return;
	CSoundManager::NoMultipleSEPlay("AlienApp");
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 移動.
void CAlien::Move()
{
	TargetRotation();			// 回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.
	CAlien::WaitMove();			// 待機.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Escape;
	m_NowMoveState	= EMoveState::Rotation;	// 移動状態を回転する.
}

// 拐う.
void CAlien::Abduct()
{
	if( m_IsBarrierHit == true ) return;
	if( m_pArm == nullptr ) return;

	SetMoveVector( *m_pAbductUFOPosition );		// マザーシップの座標とのベクトルを取得.
	m_TargetPosition = *m_pAbductUFOPosition;	// マザーシップの座標を記憶.

	TargetRotation();	// マザーシップの方へ回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// 女の子を連れ去っていなければ.
	m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;		// 移動の回転状態へ遷移.
}

// 怯み.
void CAlien::Fright()
{
	m_InvincibleCount++;					// 無敵カウントを加算.
	if( IsInvincibleTime( m_Parameter.InvincibleTime ) == false ) return;
	m_NowState			= EAlienState::Move;	// 移動状態へ遷移.
	m_NowMoveState		= EMoveState::Rotation;	// 移動の回転状態へ遷移.
}

// 死亡.
void CAlien::Death()
{
	m_ModelAlpha -= m_Parameter.ModelAlphaSubValue;
	if( m_ModelAlpha > 0.0f ) return;
	CSoundManager::PlaySE("AlienDead");
	m_IsDelete = true;	// 死亡フラグを立てる.
}

// 逃げる.
void CAlien::Escape()
{
	if( m_IsBarrierHit == true ) return;

	SetMoveVector( *m_pAbductUFOPosition );		// マザーシップの座標とのベクトルを取得.
	m_TargetPosition = *m_pAbductUFOPosition;	// マザーシップの座標を記憶.

	TargetRotation();	// マザーシップの方へ回転.
	CAlien::VectorMove( m_MoveSpeed );	// 移動.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// 女の子を連れ去っていなければ.
	m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;		// 移動の回転状態へ遷移.
}

// アルファブレンドの設定.
void CAlien::AlphaBlendSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ){
		if( m_ModelAlpha < 1.0f ){
			m_pSkinMesh->SetBlend( false );
		}
	}
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::GBuffer ){
		m_pSkinMesh->SetBlend( true );
		if( m_ModelAlpha >= 1.0f ){
			m_pSkinMesh->SetBlend( false );
		}
	}
#else
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ){
		if( m_ModelAlpha < 1.0f ){
			m_pTempStaticMesh->SetBlend( false );
		}
	}
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::GBuffer ){
		m_pTempStaticMesh->SetBlend( true );
		if( m_ModelAlpha >= 1.0f ){
			m_pTempStaticMesh->SetBlend( false );
		}
	}
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
}

// 女の子との当たり判定.
void CAlien::GirlCollision( CActor* pActor )
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Girl ) return;
	if( m_IsBarrierHit == true ) return;
	if( m_NowMoveState == EMoveState::Attack )	return;	// 攻撃状態は終了.
	if( m_NowState == EAlienState::Spawn )		return;	// スポーン状態なら終了.
	if( m_NowState == EAlienState::Death )		return;	// 死亡していたら終了.
	if( m_NowState == EAlienState::Fright )		return;	// 怯み状態なら終了.

	if( m_NowState == EAlienState::Abduct ){
		// 連れ去っている状態なのでアームの座標を設定する.
		pActor->SetPosition( m_pArm->GetGrabPosition() );
		return;
	} else {
		// 既に連れ去っているか.
		if( *m_pIsAlienOtherAbduct == true ){
			// アームを片付けていなければ片付ける.
			if( m_pArm->IsCleanUp() == false ) m_pArm->SetCleanUp();
			return;
		}
	}

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	
	// 掴んでいなければ(アームを取り出してなければ).
	if( m_pArm->IsGrab() == false ){
		m_pArm->SetAppearance();	// アームを取り出す.
		return;
	} else {
		// アームの座標を設定する.
		pActor->SetPosition( m_pArm->GetGrabPosition() );
	}

	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Abduct;	// 連れ去る状態へ遷移.
	m_NowMoveState	= EMoveState::Rotation;	// 移動を回転へ遷移.
}

// バリアとの当たり判定.
void CAlien::BarrierCollision( CActor* pActor )
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if( pActor->GetObjectTag() != EObjectTag::Bariier ) return;
	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == true ){
		CSoundManager::PlaySE("BarrierHit");
		m_MoveSpeed		= BARRIER_HIT_MOVE_SPEED;	// バリア衝突時の移動速度に変更する.
		m_IsBarrierHit	= true;						// バリア衝突フラグを立てる.
		m_NowState		= EAlienState::Move;		// 移動状態へ遷移.
		m_NowMoveState	= EMoveState::Move;			// 移動の移動状態へ遷移.
		*m_pIsAlienOtherAbduct	= false;			// 女の子を連れ去るフラグを下す.
	} else {
		m_MoveSpeed		= m_Parameter.MoveSpeed;	// 通常の移動速度に戻す.
		m_IsBarrierHit	= false;	// バリア衝突フラグを下す.
	}
}