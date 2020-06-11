#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"

CAlienA::CAlienA()
	: m_Parameter	()
{
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienA::~CAlienA()
{
}

// �������֐�.
bool CAlienA::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienA::Update()
{
	SetMoveVector( m_GirlPosition );
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V.

	if( GetAsyncKeyState('Q') & 0x8000 ){
		TargetRotation();
	}
	if( GetAsyncKeyState('W') & 0x8000 ){
		Move();
	}
} 

// �`��֐�.
void CAlienA::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );

	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CAlienA::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	if( pActor->GetObjectTag() != EObjectTag::Player ) return;

	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	auto lifeDe = []( float& life )
	{
		life -= 1.0f;
	};
	pActor->LifeCalculation( lifeDe );
}

// �X�|�[��.
bool CAlienA::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != EAlienState::None ) return true;
	// �������Ɏ��s������I��.
	if( Init() == false ) return false;
	m_Parameter = param;	// �p�����[�^��ݒ�.
	m_vPosition = spawnPos;	// �X�|�[�����W�̐ݒ�.
	m_NowState = EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.

	return true;
}

// �X�|�[��.
void CAlienA::Spawning()
{

}

// �ړ�.
void CAlienA::Move()
{
	CAlien::VectorMove( m_Parameter.MoveSpeed );
}

// ����.
void CAlienA::Abduct()
{
}

// ����.
void CAlienA::Fright()
{
}

// ���S.
void CAlienA::Death()
{
}

// ������.
void CAlienA::Escape()
{
}

// �����蔻��̐ݒ�.
bool CAlienA::ColliderSetting()
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