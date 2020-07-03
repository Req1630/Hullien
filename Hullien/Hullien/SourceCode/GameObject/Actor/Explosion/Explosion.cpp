#include "Explosion.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

CExplosion::CExplosion()
	: m_Param			()
{
}

CExplosion::~CExplosion()
{
}

// �������֐�.
bool CExplosion::Init()
{
	const char* MODEL_NAME = "GhostA";	// ���f����(��ŏ����\��).
	if( GetModel( MODEL_NAME ) == false ) return false;
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
	m_Param.ExplosionTime += 0.1f;
	if( m_Param.ExplosionTime >= 1.0f )return;
#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CExplosion::Collision( CActor* pActor )
{
	// �G�t�F�N�g�̕`�������\��.
	
	if( m_Param.ExplosionTime >= 1.0f )return;
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

// ���f���̎擾.
bool CExplosion::GetModel( const char* modelName )
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pStaticMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic( m_pStaticMesh, modelName );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// �����蔻��̐ݒ�.
bool CExplosion::ColliderSetting()
{
	if( m_pStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Param.SphereAdjPos,
		m_Param.SphereAdjRadius ) )) return false;
	return true;
}