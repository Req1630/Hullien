#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CAlien::CAlien()
	: m_TargetPosition		( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation		( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition	( nullptr )
	, m_Parameter			()
	, m_NowState			( EAlienState::None )
	, m_NowMoveState		( EMoveState::None )
	, m_ModelAlpha			( 0.0f )
	, m_WaitCount			( 0 )
	, m_pIsAlienOtherAbduct	( nullptr )
	, m_IsDelete			( false )
{
}

CAlien::~CAlien()
{
}

// ������W�̐ݒ�.
void CAlien::SetTargetPos( CActor& actor )
{
	if( m_NowMoveState == EMoveState::Move ) return;
	if( *m_pIsAlienOtherAbduct == true ) return;

	// ���̎q����Ȃ���ΏI��.
	// ���͏��̎q�����Ȃ��̂�plyaer�őΏ�.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_TargetPosition = actor.GetPosition();	// ���̎q�̍��W���擾.
}

// ���C�t�v�Z�֐�.
void CAlien::LifeCalculation( const std::function<void(float&)>& proc )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	if( m_NowState == EAlienState::Fright ) return;

	proc( m_Parameter.Life );
	m_NowState = EAlienState::Fright;
	if( m_Parameter.Life > 0.0f ) return;
	m_NowState = EAlienState::Death;
}

// ���݂̏�Ԃ̍X�V�֐�.
void CAlien::CurrentStateUpdate()
{
	switch( m_NowState )
	{
	case EAlienState::Spawn:
		this->Spawning();
		break;
	case EAlienState::Move:
		this->Move();
		break;
	case EAlienState::Abduct:
		this->Abduct();
		break;
	case EAlienState::Fright:
		this->Fright();
		break;
	case EAlienState::Death:
		this->Death();
		break;
	case EAlienState::Escape:
		this->Escape();
		break;
	default:
		break;
	}
}

// �ړ��x�N�g���ݒ�֐�.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f( 
		targetPos.x - m_vPosition.x,
		targetPos.z - m_vPosition.z );

	// �ړ��p�x�N�g�����擾.
	m_MoveVector.x = sinf( m_TargetRotation.y );
	m_MoveVector.z = cosf( m_TargetRotation.y );
}

// �ړI�̍��W�։�].
void CAlien::TargetRotation()
{
	if( m_NowMoveState != EMoveState::Rotation ) return;

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// �ړI�̃x�N�g���ƁA�����̃x�N�g���̊O�ς����߂�.
	float cross = myVector.x * m_MoveVector.z-myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x+myVector.z * m_MoveVector.z;
	dot = acosf( dot /( myLenght * targetLenght ));

	// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// ���ς����e�͈͂Ȃ�.
	if( -TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN ){
		m_vRotation.y = m_TargetRotation.y;	// �^�[�Q�b�g�ւ̉�]�擾.
		// �ړ��p�x�N�g�����擾.
		m_MoveVector.x = sinf( m_vRotation.y );
		m_MoveVector.z = cosf( m_vRotation.y );
		m_NowMoveState = EMoveState::Move;
	}
}

// �ړ��֐�.
void CAlien::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	if( lenght >= 1.0f ) return;

	m_NowMoveState = EMoveState::Wait;
}