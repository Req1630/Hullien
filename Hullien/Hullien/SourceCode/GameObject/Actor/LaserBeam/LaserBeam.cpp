#include "LaserBeam.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"

CLaserBeam::CLaserBeam()
	: m_TargetPosition	( 0.0f, 0.0f, 0.0f )
	, m_IsInAttack		( false )
	, m_IsEndAttack		( false )
{
}

CLaserBeam::~CLaserBeam()
{
}

// �������֐�.
bool CLaserBeam::Init()
{
	if( CollisionSetting() == false ) return false; 
	return true;
}

// �X�V�֐�.
void CLaserBeam::Update()
{
	if( m_IsInAttack == false ) return;
}

// �`��֐�.
void CLaserBeam::Render()
{
	if( m_IsInAttack == false ) return;

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CLaserBeam::Collision( CActor* pActor )
{
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// �X�t�B�A�̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	if( GetAsyncKeyState('M') & 0x0001 )
		pActor->SetParalysisTime( [&]( float& time ){ time = 3; } );
}

// ������W�̐ݒ�֐�.
void CLaserBeam::SetTargetPos( CActor& actor )
{
	// �v���C���[����Ȃ���ΏI��.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	if( m_IsInAttack == true ) return;
	m_TargetPosition = actor.GetPosition();	// �v���C���[�̍��W���擾.
}

// �U���J�n.
void CLaserBeam::Shot()
{
	m_IsInAttack = true;
}

// �p�����[�^�������ɖ߂�.
void CLaserBeam::ResetParam()
{
	m_IsInAttack	= false;
	m_IsEndAttack	= false;
}

// �����蔻��̐ݒ�.
bool CLaserBeam::CollisionSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f, 0.0f, 0.0f },
		1.0f ) )) return false;

	return true;
}