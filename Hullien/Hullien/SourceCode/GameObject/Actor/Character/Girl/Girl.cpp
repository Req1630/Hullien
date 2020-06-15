#include "Girl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

CGirl::CGirl()
	: m_Parameter	()
{
	m_ObjectTag = EObjectTag::Girl;
	m_vSclae = { 0.03f, 0.03f, 0.03f };
}

CGirl::~CGirl()
{
}

// 初期化関数.
bool CGirl::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;

	return true;
}

// 更新関数.
void CGirl::Update()
{
	if( GetAsyncKeyState(VK_UP) & 0x8000 ) m_vPosition.z -= 0.04f;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 ) m_vPosition.z += 0.04f;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 ) m_vPosition.x -= 0.04f;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 ) m_vPosition.x += 0.04f;
}

// 描画関数.
void CGirl::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( 0.01 );
	m_pSkinMesh->Render();

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// 当たり判定関数.
void CGirl::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
}

// 相手座標の設定関数.
void CGirl::SetTargetPos( CActor& actor )
{
	m_vPosition = actor.GetPosition();
}

// 移動関数.
void CGirl::Move()
{
}

// 当たり判定の作成.
bool  CGirl::ColliderSetting()
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