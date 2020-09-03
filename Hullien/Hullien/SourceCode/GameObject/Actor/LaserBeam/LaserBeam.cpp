#include "LaserBeam.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"

CLaserBeam::CLaserBeam()
	: m_MoveSpeed			( DEFAULT_MOVE_SPEED )
	, m_ParalysisTime		( DEFAULT_PARALYSIS_TIME )
	, m_TargetPosition		( 0.0f, 0.0f, 0.0f )
	, m_IsInAttack			( false )
	, m_IsEndAttack			( true )
	, m_FrameCount			( 0.0f )
	, m_FrameTime			( 1.0f )
	, m_InitPosition		( 0.0f, 0.0f, 0.0f )
	, m_ControlPointList	()
{
	m_ObjectTag = EObjectTag::LaserBeam;
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

	// ������W�̃T�C�Y�Ŕ�r.
	switch( m_ControlPointList.size() )
	{
	case 0:
		break;
	case 1:
		// �񎟌��x�W�F�Ȑ�.
		SecondaryBeziercurve();
		break;
	case 2:
		// �O�����x�W�F�Ȑ�.
		ThirdBezierCurve();
		break;
	default:
		break;
	}

	// �J�E���g���^�C���ȏ�ɂȂ�΃V���b�g�t���O������.
	if( m_FrameCount >= m_FrameTime ){
		m_IsInAttack = false;
		m_IsEndAttack = true;
	}
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
	if( m_IsInAttack == false ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// �X�t�B�A�̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	
	// �v���C���[��Ⴢɂ�����.
	pActor->SetParalysisTime( [&]( float& time ){ time = m_ParalysisTime; } );
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
void CLaserBeam::Shot( const D3DXVECTOR3& pos )
{
	if( m_IsInAttack == true ) return;
	m_IsInAttack	= true;
	m_IsEndAttack	= false;
	m_vPosition		= pos;
	m_InitPosition	= pos;
	m_FrameCount	= 0.0f;
}

// �p�����[�^�������ɖ߂�.
void CLaserBeam::ResetParam()
{
	m_IsEndAttack	= false;
}

// �x�W�F�Ȑ��̐���_�̐ݒ�.
void CLaserBeam::SetControlPointList( std::vector<D3DXVECTOR3> pointList )
{
	m_ControlPointList = pointList; 
}

// �񎟃x�W�F�Ȑ�.
void CLaserBeam::SecondaryBeziercurve()
{
	if( m_FrameCount >= m_FrameTime ) return;

	float a = m_FrameCount / m_FrameTime;
	float b = m_FrameTime - a;

	D3DXVECTOR3 point[2];

	point[0].x = b * m_InitPosition.x + a * m_ControlPointList[0].x;
	point[0].y = b * m_InitPosition.y + a * m_ControlPointList[0].y;
	point[0].z = b * m_InitPosition.z + a * m_ControlPointList[0].z;

	point[1].x = b * m_ControlPointList[0].x + a * m_TargetPosition.x;
	point[1].y = b * m_ControlPointList[0].y + a * m_TargetPosition.y;
	point[1].z = b * m_ControlPointList[0].z + a * m_TargetPosition.z;

	m_vPosition.x = b * point[0].x + a * point[1].x;
	m_vPosition.y = b * point[0].y + a * point[1].y;
	m_vPosition.z = b * point[0].z + a * point[1].z;

	const float modeSpeed = 0.1f;

	m_FrameCount += m_MoveSpeed;
}

// �O���x�W�F�Ȑ�.
void CLaserBeam::ThirdBezierCurve()
{
	if( m_FrameCount >= m_FrameTime ) return;

	float a = m_FrameCount / m_FrameTime;
	float b = m_FrameTime - a;

	D3DXVECTOR3 point[3];

	point[0].x = b * m_InitPosition.x + a * m_ControlPointList[0].x;
	point[0].y = b * m_InitPosition.y + a * m_ControlPointList[0].y;
	point[0].z = b * m_InitPosition.z + a * m_ControlPointList[0].z;

	point[1].x = b * m_ControlPointList[0].x + a * m_ControlPointList[1].x;
	point[1].y = b * m_ControlPointList[0].y + a * m_ControlPointList[1].y;
	point[1].z = b * m_ControlPointList[0].z + a * m_ControlPointList[1].z;

	point[2].x = b * m_ControlPointList[1].x + a * m_TargetPosition.x;
	point[2].y = b * m_ControlPointList[1].y + a * m_TargetPosition.y;
	point[2].z = b * m_ControlPointList[1].z + a * m_TargetPosition.z;

	D3DXVECTOR3 p[2];

	p[0].x = b * point[0].x + a * point[1].x;
	p[0].y = b * point[0].y + a * point[1].y;
	p[0].z = b * point[0].z + a * point[1].z;

	p[1].x = b * point[1].x + a * point[2].x;
	p[1].y = b * point[1].y + a * point[2].y;
	p[1].z = b * point[1].z + a * point[2].z;

	m_vPosition.x = b * p[0].x + a * p[1].x;
	m_vPosition.y = b * p[0].y + a * p[1].y;
	m_vPosition.z = b * p[0].z + a * p[1].z;

	m_FrameCount += m_MoveSpeed;
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