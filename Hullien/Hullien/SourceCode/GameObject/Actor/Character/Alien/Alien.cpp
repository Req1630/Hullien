#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CAlien::CAlien()
{
}

CAlien::~CAlien()
{
}

// ������W�̐ݒ�.
void CAlien::SetOpponentPos( CActor& actor )
{
	// ���̎q����Ȃ���ΏI��.
	// ���͏��̎q�����Ȃ��̂�plyaer�őΏ�.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_GirlPosition = actor.GetPosition();	// ���̎q�̍��W���擾.
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
	}
}