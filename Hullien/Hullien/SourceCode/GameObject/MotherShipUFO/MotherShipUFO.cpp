#include "MotherShipUFO.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\Actor\Actor.h"

CMotherShipUFO::CMotherShipUFO()
	: m_pStaticMesh		( nullptr )
	, m_pCollManager	( nullptr )
	, m_Param			()
{
	m_vPosition = INIT_POS;
}

CMotherShipUFO::~CMotherShipUFO()
{
}

// �������֐�.
bool CMotherShipUFO::Init()
{
	if( GetModel() == false ) return false;
	if( CollisionSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CMotherShipUFO::Update()
{
}

// �`��֐�.
void CMotherShipUFO::Render()
{
	if( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->Render();

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��.
void CMotherShipUFO::Collision( CActor* pActor )
{
	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	D3DXVECTOR3 pos = pActor->GetPosition();
	pos.y += ADD_POS_POWER;	// ���W����ɂ�����.
	pActor->SetPosition( pos );
}

// �p�����[�^�̐ݒ�.
void CMotherShipUFO::SetParameter( const SMotherShipUFOParam& param )
{
	m_Param = param;
	m_vPosition = m_Param.Position;
	if( m_pCollManager != nullptr ){
		m_pCollManager->InitSphere(
			&m_vPosition,
			&m_vRotation,
			&m_vSclae.x,
			m_Param.SphereAdjPos,
			m_Param.CollisionRadius );
	}
}

// ���f���̎擾.
bool CMotherShipUFO::GetModel()
{
	if( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// �����蔻��̐ݒ�.
bool CMotherShipUFO::CollisionSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f, 0.0f, 0.0f },
		COLLISION_RADIUS ))){
		return false;
	}
	return true;
}