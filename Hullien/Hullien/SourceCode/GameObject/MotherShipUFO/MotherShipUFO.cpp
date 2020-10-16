#include "MotherShipUFO.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\Actor\Actor.h"
#include "..\..\Utility\FileManager\FileManager.h"

CMotherShipUFO::CMotherShipUFO()
	: m_pStaticMesh		( nullptr )
	, m_pCollManager	( nullptr )
	, m_Param			()
	, m_IsDisp			( true )
	, m_IsReturnAlien	( false )
{
}

CMotherShipUFO::~CMotherShipUFO()
{
}

// �������֐�.
bool CMotherShipUFO::Init()
{
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Param ) == false ) return false;
	if( GetModel() == false ) return false;
	if( CollisionSetting() == false ) return false;

	m_vPosition = m_Param.Position;

	return true;
}

// �X�V�֐�.
void CMotherShipUFO::Update()
{
}

// �`��֐�.
void CMotherShipUFO::Render()
{
	// ��ʊO�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
	if( m_pStaticMesh == nullptr ) return;
	if( m_IsDisp == false ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );

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
	m_IsReturnAlien = true;
	D3DXVECTOR3 pos = pActor->GetPosition();
	pos.y += m_Param.AddPosYPower;	// ���W����ɂ�����.
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
		m_Param.SphereAdjPos,
		m_Param.CollisionRadius ))){
		return false;
	}
	return true;
}