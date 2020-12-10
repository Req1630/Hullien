#include "STGEnemy.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\..\Common\Font\Font.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\Bullet\BuletManager\BuletManager.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"

STG::CEnemy::CEnemy()
	: CEnemy	( STG::SEnemyParam() )
{
}

STG::CEnemy::CEnemy( const STG::SEnemyParam& param )
	: PARAMETER				( param )
	, m_pGuns				()
	, m_pFont				( nullptr )
	, m_NowState			( STG::EEnemyState::Spawn )
	, m_DeadUpParam			()
	, m_FontRotation		( FONT_ROTATION )
	, m_MoveSpeed			( 0.0f )
	, m_MoveingDistance		( 0.0f )
	, m_MoveingDistanceMax	( 0.0f )
	, m_ShakeCount			( SHAKE_COUNT_MAX )
	, m_EscapeCount			( ESCAPE_COUNT_MAX )
	, m_SpawnCount			( 0 )
	, m_IsHitShake			( false )
	, m_IsMySpawnLast		( false )
{
	m_pFont			= std::make_unique<CFont>();
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition.z	= INIT_POSITION_Z;
	m_vPosition.x	= PARAMETER.PositionX;
	m_vRotation		= { 0.0f, 0.0f, static_cast<float>(D3DXToRadian(90)) };
	m_MoveSpeed		= PARAMETER.MoveSpeed;
	m_LifePoint		= PARAMETER.LifePoint;
	m_vScale		= { PARAMETER.TextSize, PARAMETER.TextSize, PARAMETER.TextSize };

	SBulletManagerParam s = {};
	m_pGuns.emplace_back( std::make_unique<CBulletManager>( s ) );
}

STG::CEnemy::CEnemy( 
	const STG::SEnemyParam& param, 
	const std::vector<SBulletManagerParam>& bulletParams )
	: CEnemy	( param )
{
	m_pGuns.resize( bulletParams.size() );
	auto it = bulletParams.begin();
	for( auto& g : m_pGuns ){
		g = std::make_unique<CBulletManager>( *it );
		it++;
	}
}

STG::CEnemy::~CEnemy()
{
}

// 初期化関数.
bool STG::CEnemy::Init()
{
	if( CollisionInit()	== false ) return false;
	for( auto& g : m_pGuns ) if( g->Init()	== false ) return false;
	if( FAILED( m_pFont->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ) ))	return false;

	return true;
}

// 更新関数.
void STG::CEnemy::Update()
{
	// 弾の更新.
	for( auto& g : m_pGuns ) g->Update();

	switch( m_NowState )
	{
	case STG::EEnemyState::Spawn:	Spawn();	break;	// スポーン.
	case STG::EEnemyState::Move:	Move();		break;	// 移動.
	case STG::EEnemyState::Shot:	Shot();		break;	// 弾を撃つ.
	case STG::EEnemyState::Escape:	Escape();	break;	// 逃げる.
	case STG::EEnemyState::Dead:	Dead();		break;	// 死亡.
	default:									break;
	}

	HitShake();		// ヒット時の揺れ.
}

// 描画関数.
void STG::CEnemy::Render()
{
	// 弾の描画.
	for( auto& g : m_pGuns ) g->Render();

	if( m_NowState == STG::EEnemyState::None ) return;

	m_pFont->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
	m_pFont->SetPosition( m_vPosition );
	m_pFont->SetRotation( m_FontRotation );
	m_pFont->SetScale( m_vScale );
	m_pFont->SetCoverage( true );
	m_pFont->Render( PARAMETER.Text );
	m_pFont->SetCoverage( false );

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// 当たり判定.
void STG::CEnemy::Collision( STG::CActor* pActor )
{
	if( pActor->GetActive()	== false ) return;	// 相手が動作してなければ終了.
	// 弾の当たり判定.
	for( auto& g : m_pGuns ) g->Collision( pActor );

	if( m_IsActive			== false ) return;	// 自分が動作してなければ終了.
	// カプセルの当たり判定.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;
}

// スポーン.
void STG::CEnemy::Spawn()
{
	m_SpawnCount++;	// スポーンカウントを加算.
	if( m_SpawnCount < PARAMETER.SpawnTime ) return;
	// スポーンカウントが一定値に達すれば.
	// スポーンする.
	m_NowState = STG::EEnemyState::Move;
	m_IsActive = true;
}


// 移動関数.
void STG::CEnemy::Move()
{
	m_vPosition.z += m_MoveSpeed;
	if( m_vPosition.z >= MOVE_SUB_POSITION_Z ) m_MoveSpeed -= MOVE_SUB_VALUE;

	if( m_MoveSpeed > 0.0f ) return;

	m_NowState = STG::EEnemyState::Shot;

	// 初期角度を設定.
	for( auto& g : m_pGuns ) g->SetInitAngle( m_vPosition, m_TargetPositon );
}

// 弾を撃つ.
void STG::CEnemy::Shot()
{
	for( auto& g : m_pGuns ) g->SetPosition( m_vPosition );
	for( auto& g : m_pGuns ) g->Shot();
	bool isEnd = true;
	for( auto& g : m_pGuns ){
		if( g->IsShotEnd() == false ) isEnd = false;
	}
	if( isEnd == false ) return;
	m_NowState = STG::EEnemyState::Escape;
}

// 逃げる.
void STG::CEnemy::Escape()
{
	// 移動速度を一定速度になるまで加算.
	if( m_MoveSpeed < PARAMETER.MoveSpeed ) m_MoveSpeed += MOVE_SUB_VALUE;

	// ベクトルを使用して移動.
	m_vPosition.x += m_MoveVector.x * m_MoveSpeed;
	m_vPosition.z += m_MoveVector.z * m_MoveSpeed;

	m_EscapeCount--;	// 逃げるカウントの減算.
	// 逃げるカウントが 0.0 より大きければ.
	if( m_EscapeCount > 0.0f ){
		m_MoveingDistance += m_MoveSpeed;	// 距離を加算.
	}

	// 移動距離が一定距離を超得たら.
	if( m_MoveingDistance >= m_MoveingDistanceMax ){
		SearchRandomMoveVector();	// 移動ベクトルを検索する.
	}

	// 画面外に出たら.
	if( IsDisplayOut( E_WND_OUT_ADJ_SIZE ) == true ){
		m_NowState = STG::EEnemyState::Dead;	// 死亡.
	}
}

// 死亡.
void STG::CEnemy::Dead()
{
	if( m_IsMySpawnLast == false ){
		m_IsActive	= false;		// 動作終了.
		m_vScale.x -= DEAD_SCALE_SUB_VALUE;	// スケールを加算.
		m_vScale.y -= DEAD_SCALE_SUB_VALUE;	// スケールを加算.
		m_vScale.z -= DEAD_SCALE_SUB_VALUE;	// スケールを加算.
		m_FontRotation.z += DEAD_ROTATION_SPEED;	// 回転させる.

		if( m_vScale.x > 0.0f ) return;
		// スケールが 0.0 以下になれば.
		m_NowState	= STG::EEnemyState::None;	// 何もない状態へ遷移.
		// 座標を画面外へ.
		m_vPosition	= { INIT_POSITION_Z, 0.0f, INIT_POSITION_Z };
	} else {
		// スポーンラストの際の死亡処理
		SpawnLastDead();
	}
}

// スポーンラストの際の死亡処理.
void STG::CEnemy::SpawnLastDead()
{
	D3DXVECTOR3 cameraPos = CCameraManager::GetPosition();	// カメラの座標.
	D3DXVECTOR3 vec = cameraPos - m_vPosition;	// 目的の座標へのベクトル.
	vec.y = 0.0f;
	float length = 0.0f;
	// 目的の座標との距離が一定以上なら.
	//	目的の座標に近づける.
	length = D3DXVec3Length( &vec );
	if( length >= DEAD_TARGET_POSITION_LENGTH && m_DeadUpParam.IsMoveEnd == false ){
		D3DXVec3Normalize( &vec, &vec );
		const float moveSpeed = length * DEAD_TARGET_MOVE_SPEED;
		m_vPosition.x += vec.x * moveSpeed;
		m_vPosition.z += vec.z * moveSpeed;
	}
	m_FontRotation.z += DEAD_ROTATION_SPEED;	// 回転させる.

	// 移動加速値の加算.
	m_DeadUpParam.MoveAccValue += DEAD_MOVE_ACC_ADD_VALUE;
	if( m_DeadUpParam.MoveAccValue >= DEAD_MOVE_ACC_VALUE_MAX ) 
		m_DeadUpParam.MoveAccValue = DEAD_MOVE_ACC_VALUE_MAX;

	// 座標を上にあげる.
	if( m_vPosition.y < cameraPos.y-DEAD_POSITION_Y_ADJ_VALUE ){
		m_vPosition.y += m_DeadUpParam.MoveSpeed + m_DeadUpParam.MoveAccValue;
	} else {
		m_vPosition.y = cameraPos.y-DEAD_POSITION_Y_ADJ_VALUE;
		m_FontRotation.z -= DEAD_ROTATION_SPEED;

		m_DeadUpParam.IsMoveEnd = true;
		if( m_DeadUpParam.IsPlaySE == false ){
			CSoundManager::PlaySE("camaeraHit");
			m_DeadUpParam.IsPlaySE = true;
		}
	}

	// カメラの揺れ.
	if( m_DeadUpParam.IsMoveEnd == true ){
		D3DXVECTOR3 cameraLookPos = CCameraManager::GetLookPosition();
		m_DeadUpParam.CameraShakeCount--;
		if( m_DeadUpParam.CameraShakeCount <= 0.0f ) m_DeadUpParam.CameraShakeCount = 0.0f;

		// 揺れ値の取得.
		const float SHAKE_VALUE =  m_DeadUpParam.GetShakeValue();
		cameraPos.x		+= SHAKE_VALUE;
		cameraPos.z		+= SHAKE_VALUE;
		cameraLookPos.x	+= SHAKE_VALUE * DEAD_CAMERA_SHAKE_ADJ_VALUE;
		cameraLookPos.z	+= SHAKE_VALUE * DEAD_CAMERA_SHAKE_ADJ_VALUE;

		CCameraManager::SetPosition( cameraPos );
		CCameraManager::SetLookPosition( cameraLookPos );
	}
}

// 当たった時の揺れ.
void STG::CEnemy::HitShake()
{
	if( m_IsHitShake == false ) return;

	m_ShakeCount -= SHAKE_SUB_VALUE;	// 揺れカウントを減算.
	m_vPosition.x += sinf( static_cast<float>(D3DX_PI)		* m_ShakeCount ) * SHAKE_SPEED;
	m_vPosition.z += sinf( static_cast<float>(D3DX_PI)*0.5f * m_ShakeCount ) * SHAKE_SPEED;

	if( m_ShakeCount > 0.0f ) return;
	// 揺れカウントが 0.0 以下になれば.
	m_ShakeCount = SHAKE_COUNT_MAX;	// 揺れカウントを初期化.
	m_IsHitShake = false;			// ヒットフラグを下す.
}

// ライフ計算関数.
void STG::CEnemy::LifeCalculation( const std::function<void(float&)>& proc )
{
	// 逃げる状態のみ攻撃を受ける.
	if( m_NowState != STG::EEnemyState::Escape ){
		m_IsHitShake = true;	// ヒットフラグを立てる.
		return;
	}

	proc( m_LifePoint );
	m_IsHitShake = true;	// ヒットフラグを立てる.

	if( m_LifePoint > 0.0f ) return;
	// ライフが0以下になれば
	m_NowState = STG::EEnemyState::Dead;	// 死亡状態へ遷移.
	CSoundManager::PlaySE(DEAD_SE_NAME);
}

// ランダムで移動ベクトルを検索.
void STG::CEnemy::SearchRandomMoveVector()
{
	// 移動ベクトルをランダムから取得.
	m_MoveVector.x = sinf( static_cast<float>(rand()) );
	m_MoveVector.z = cosf( static_cast<float>(rand()) );

	// 最大移動距離をランダムから取得.
	m_MoveingDistanceMax = fabsf(cosf( static_cast<float>(rand()) )) * ESCAPE_MOVE_DISTANCE;
	m_MoveingDistance = 0.0f;
	m_pGuns[0]->RandomShot();
}

// 当たり判定の作成.
bool STG::CEnemy::CollisionInit()
{
	float charByte = 2.0f;	// 文字のバイト数.
	// テキストの一文字目のバイト数を取得.
	if( IsDBCSLeadByte( PARAMETER.Text[0] ) == TRUE ) charByte = 1.0f;

	// テキストの大きさを取得.
	const float textScale = 
		( PARAMETER.TextSize * charByte * static_cast<float>(PARAMETER.Text.length()) );

	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		PARAMETER.TextSize,
		textScale ))) return false;
	return true;
}