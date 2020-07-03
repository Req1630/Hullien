#include "Alien_C.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

CAlienC::CAlienC()
{
	m_ObjectTag = EObjectTag::Alien_C;
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienC::~CAlienC()
{
}

// �������֐�.
bool CAlienC::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienC::Update()
{
	SetMoveVector( m_TargetPosition );
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V.
	// �̗͂�0��葽����ΏI��.
	if( m_Parameter.Life > 0.0f ) return;
	// �̗͂��Ȃ���Δ���������.
	m_IsExplosion = true;
}

// �`��֐�.
void CAlienC::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.5f, 0.8f, m_ModelAlpha } );
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
void CAlienC::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );
}

// �X�|�[��.
bool CAlienC::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
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
void CAlienC::Spawning()
{
	CAlien::Spawning();
}

// �ړ�.
void CAlienC::Move()
{
	CAlien::Move();
}

// ����.
void CAlienC::Abduct()
{
	CAlien::Abduct();
}

// ����.
void CAlienC::Fright()
{
	CAlien::Fright();
}

// ���S.
void CAlienC::Death()
{
	CAlien::Death();
}

// ������.
void CAlienC::Escape()
{
	CAlien::Escape();
}

// �����蔻��̐ݒ�.
bool CAlienC::ColliderSetting()
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