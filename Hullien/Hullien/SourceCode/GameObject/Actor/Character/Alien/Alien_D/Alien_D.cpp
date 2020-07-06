#include "Alien_D.h"
#include "..\..\..\LaserBeam\LaserBeam.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

CAlienD::CAlienD()
	: m_pAttackRangeSprite	( nullptr )
	, m_pLaserBeam			( nullptr )
	, m_AttackCount			( 0.0f )
	, m_IsAttackStart		( false )
{
	m_ObjectTag = EObjectTag::Alien_D;
	m_vSclae = { 0.05f, 0.05f, 0.05f };
	m_pLaserBeam = std::make_unique<CLaserBeam>();
}

CAlienD::~CAlienD()
{
}

// 初期化関数.
bool CAlienD::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( GetSprite( SPRITE_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	if( m_pLaserBeam->Init() == false ) return false; 
	return true;
}

// 更新関数.
void CAlienD::Update()
{
	SetMoveVector( m_TargetPosition );
	m_pLaserBeam->Update();
	CurrentStateUpdate();	// 現在の状態の更新.
}

// 描画関数.
void CAlienD::Render()
{
	m_pLaserBeam->Render();
	ModelRender();
	AttackRangeSpriteRender();

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CAlienD::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	AttackCollision( pActor );
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
}

// スポーン.
bool CAlienD::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_Parameter = param;	// パラメータを設定.
	m_vPosition = spawnPos;	// スポーン座標の設定.
	m_NowState = EAlienState::Spawn;	// 現在の状態をスポーンに変更.
	return true;
}

// モデルの描画.
void CAlienD::ModelRender()
{
	if( m_pSkinMesh == nullptr ) return;
	
	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.8f, 0.8f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );
}

// 攻撃範囲のスプライト描画.
void CAlienD::AttackRangeSpriteRender()
{
	if( m_pAttackRangeSprite == nullptr ) return;
	if( m_NowMoveState != EMoveState::Attack ) return;

	D3DXVECTOR4 color;
	if( m_IsAttackStart == true ){
		// 攻撃の色 (黄).
		color = ATTACK_RANGE_DANGER_COLOR;
		color.w = m_AttackCount;
	} else {
		// 攻撃の予告の色 (赤).
		color = ATTACK_RANGE_COLOR;
		color.w = m_AttackCount;
	}

	m_pAttackRangeSprite->SetPosition( { m_TargetPosition.x, m_TargetPosition.y+0.002f, m_TargetPosition.z } );
	m_pAttackRangeSprite->SetRotation( { static_cast<float>(D3DXToRadian(90)), 0.0f, 0.0f } );
	m_pAttackRangeSprite->SetScale( 10.0f );
	m_pAttackRangeSprite->SetColor( color );
	m_pAttackRangeSprite->SetBlend( true );
	m_pAttackRangeSprite->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pAttackRangeSprite->Render();
	m_pAttackRangeSprite->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pAttackRangeSprite->SetBlend( false );
}

// スポーン.
void CAlienD::Spawning()
{
	CAlien::Spawning();
}

// 移動.
void CAlienD::Move()
{
	TargetRotation();		// 回転.
	CAlienD::VectorMove( m_Parameter.MoveSpeed );	// 移動.
	Attack();				// 攻撃.
	CAlien::WaitMove();		// 待機.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState = EAlienState::Escape;
	m_NowMoveState = EMoveState::Rotation;	// 移動状態を回転する.
}

// 拐う.
void CAlienD::Abduct()
{
}

// 怯み.
void CAlienD::Fright()
{
	CAlien::Fright();
}

// 死亡.
void CAlienD::Death()
{
	CAlien::Death();
}

// 逃げる.
void CAlienD::Escape()
{
	CAlien::Escape();
}

// 攻撃関数.
void CAlienD::Attack()
{
	if( m_NowMoveState != EMoveState::Attack ) return;

	const float attackSpeed = m_IsAttackStart == false ? 0.005f : -0.01f;
	m_AttackCount += attackSpeed;	// 攻撃カウントの追加.

	// 攻撃カウントが攻撃時間より多くなれば攻撃を始める.
	if( m_AttackCount >= ATTACK_TIME ){
		m_IsAttackStart = true;
		m_pLaserBeam->Shot();	// ビームを打つ.
	}

	if( m_AttackCount >= 0.0f ) return;
	m_NowMoveState = EMoveState::Wait;
}

// 移動関数.
void CAlienD::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	if( lenght >= m_Parameter.AttackLenght ) return;
	m_pLaserBeam->ResetParam();
	m_IsAttackStart = false;
	m_AttackCount = 0.0f;
	m_NowMoveState = EMoveState::Attack;
}

// 相手座標の設定.
void CAlienD::SetTargetPos( CActor& actor )
{
	// プレイヤーじゃなければ終了.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_pLaserBeam->SetTargetPos( actor );	// プレイヤーの座標を取得.
	if( m_IsAttackStart == true ) return;
	m_TargetPosition = actor.GetPosition();	// プレイヤーの座標を取得.
}

// 攻撃の当たり判定.
void CAlienD::AttackCollision( CActor* pActor  )
{
	if( m_pLaserBeam == nullptr ) return;
	m_pLaserBeam->Collision( pActor );	// レーザーの当たり判定.
}

// スプライトの取得.
bool CAlienD::GetSprite( const char* spriteName )
{
	// 既に読み込めていたら終了.
	if( m_pAttackRangeSprite != nullptr ) return true;
	// スプライトの取得.
	m_pAttackRangeSprite = CSpriteResource::GetSprite( spriteName );
	// スプライトが読み込めてなければ false.
	if( m_pAttackRangeSprite == nullptr ) return false;
	return true;
}

// 当たり判定の設定.
bool CAlienD::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	
	return true;
}