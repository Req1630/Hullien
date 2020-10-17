#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Arm\Arm.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

#define IS_TEMP_MODEL_RENDER

CAlienA::CAlienA()
{
	m_ObjectTag = EObjectTag::Alien_A;
	m_pArm = std::make_unique<CArm>();
}

CAlienA::~CAlienA()
{
}

// 初期化関数.
bool CAlienA::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	// 既に読み込めていたら終了.
	if( m_pTempStaticMesh != nullptr ) return true;
	// モデルの取得.
	CMeshResorce::GetStatic( m_pTempStaticMesh, MODEL_TEMP_NAME );
	// モデルが読み込めてなければ false.
	if( m_pTempStaticMesh == nullptr ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	if( m_pArm->Init() == false ) return false;
	return true;
}

// 更新関数.
void CAlienA::Update()
{
	SetMoveVector( m_TargetPosition );	// 目的の座標のベクトルを取得.
	CurrentStateUpdate();				// 現在の状態の更新
	// アーム.
	m_pArm->SetPosition( m_vPosition );		// 座標を設定.
	m_pArm->SetRotationY( m_vRotation.y );	// 回転情報を設定.
	m_pArm->Update();						// 更新.
}

// 描画関数.
void CAlienA::Render()
{
	// 画面の外なら終了.
	if( IsDisplayOut() == true ) return;
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;
	
	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, 1.0f } );
	m_pSkinMesh->SetAnimSpeed( 0.01 );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );
#else
	if( m_pTempStaticMesh == nullptr ) return;
	m_pTempStaticMesh->SetPosition( m_vPosition );
	m_pTempStaticMesh->SetRotation( m_vRotation );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->SetColor( { 0.8f, 0.0f, 0.0f, 1.0f } );
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pTempStaticMesh->Render();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pTempStaticMesh->SetBlend( false );
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
	m_pArm->Render();	// アームの描画.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CAlienA::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );	// 女の子との当たり判定.
	BarrierCollision( pActor );	// バリアとの当たり判定.
}

// スポーン.
bool CAlienA::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// 既にスポーン済みなら終了.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// パラメータを設定.
	// 初期化に失敗したら終了.
	if( Init() == false ) return false;
	m_vPosition			= spawnPos;					// スポーン座標の設定.
	m_LifePoint			= m_Parameter.LifeMax;		// 体力の設定.
	m_NowState			= EAlienState::Spawn;		// 現在の状態をスポーンに変更.

	return true;
}

// スポーン.
void CAlienA::Spawning()
{
	CAlien::Spawning();
}

// 移動.
void CAlienA::Move()
{
	CAlien::Move();
}

// 拐う.
void CAlienA::Abduct()
{
	CAlien::Abduct();
}

// 怯み.
void CAlienA::Fright()
{
	CAlien::Fright();
}

// 死亡.
void CAlienA::Death()
{
	CAlien::Death();
}

// 逃げる.
void CAlienA::Escape()
{
	CAlien::Escape();
}

// 当たり判定の設定.
bool CAlienA::ColliderSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
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
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}