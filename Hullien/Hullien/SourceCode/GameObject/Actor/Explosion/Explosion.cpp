#include "Explosion.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

CExplosion::CExplosion()
	: m_Param				()
	, m_CollSphereRadius	( 0.0f )
{
}

CExplosion::~CExplosion()
{
}

// �������֐�.
bool CExplosion::Init()
{
	if( ColliderSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CExplosion::Update()
{
}

// �`��֐�.
void CExplosion::Render()
{
	m_CollSphereRadius += m_Param.ExplosionSpeed;
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
	if( m_pCollManager == nullptr ) return;
	// �����蔻��̃T�C�Y��ύX.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Param.SphereAdjPos,
		m_CollSphereRadius );
#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CExplosion::Collision( CActor* pActor )
{
	// �G�t�F�N�g�̕`�������\��.
	
	if( m_CollSphereRadius >= m_Param.SphereMaxRadius )return;
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��,
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// �U���֐�.
	auto attackProc = [&]( float& life ){ life -= m_Param.AttackPower; };
	pActor->LifeCalculation( attackProc );	// �Ώۂ̗̑͂����炷.
}

// �����蔻��̐ݒ�.
bool CExplosion::ColliderSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	return true;
}