#include "EventAlien.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

/****************************************
*	�C�x���g�p�F���l�N���X.
**/
CEventAlien::CEventAlien()
	: m_TargetRotation				( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter						()
	, m_NowState						( EEventAlienState::None )
	, m_NowMoveState				( EMoveState::None )
	, m_ModelAlpha					( 0.0f )
	, m_WaitCount						( 0 )
{
}

CEventAlien::~CEventAlien()
{
}

// ������W�̐ݒ�.
void CEventAlien::SetTargetPos(CEventActor & actor)
{
	SetGirlPos(actor);
}

// ���݂̏�Ԃ̍X�V�֐�.
void CEventAlien::CurrentStateUpdate()
{
	switch (m_NowState)
	{
	case EEventAlienState::Spawn:
		this->Spawning();
		break;
	case EEventAlienState::Move:
		this->Move();
		break;
	case EEventAlienState::Abduct:
		this->Abduct();
		break;
	case EEventAlienState::Escape:
		this->Escape();
		break;
	case EEventAlienState::BlowAway:
		this->BlowAway();
		break;
	case EEventAlienState::Wait:
		this->Wait();
		break;
	default:
		break;
	}
}

// ���̎q�̍��W��ݒ�.
void CEventAlien::SetGirlPos(CEventActor& actor)
{
	if (m_NowMoveState == EMoveState::Move) return;
	// ���̎q����Ȃ���ΏI��.
	if (actor.GetObjectTag() != EObjectTag::Girl) return;
	m_vDestination = actor.GetPosition();	// ���̎q�̍��W���擾.

}

// �ړ��x�N�g���ݒ�֐�.
void CEventAlien::SetMoveVector(const D3DXVECTOR3 & targetPos)
{
	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f(
		targetPos.x - m_vPosition.x,
		targetPos.z - m_vPosition.z);

	// �ړ��p�x�N�g�����擾.
	m_MoveVector.x = sinf(m_TargetRotation.y);
	m_MoveVector.z = cosf(m_TargetRotation.y);
}

// �ړI�̍��W�։�].
void CEventAlien::TargetRotation()
{
	if (m_NowMoveState != EMoveState::Rotation) return;

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf(m_vRotation.y);
	myVector.z = cosf(m_vRotation.y);

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// �ړI�̃x�N�g���ƁA�����̃x�N�g���̊O�ς����߂�.
	float cross = myVector.x * m_MoveVector.z - myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x + myVector.z * m_MoveVector.z;
	dot = acosf(dot / (myLenght * targetLenght));

	// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// ���ς����e�͈͂Ȃ�.
	if (-TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN) {
		m_vRotation.y = m_TargetRotation.y;	// �^�[�Q�b�g�ւ̉�]�擾.
		// �ړ��p�x�N�g�����擾.
		m_MoveVector.x = sinf(m_vRotation.y);
		m_MoveVector.z = cosf(m_vRotation.y);
		m_BeforeMoveingPosition = m_vPosition;
		m_NowMoveState = EMoveState::Move;
	}
}

// �ړ��֐�.
void CEventAlien::VectorMove(const float& moveSpeed)
{
	if (m_NowMoveState != EMoveState::Move) return;

	float lenght = D3DXVec3Length(&D3DXVECTOR3(m_vDestination - m_vPosition));

	m_vPosition.x -= sinf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;
	m_vPosition.z -= cosf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;

	float researchLengh = D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_vPosition));
	if (researchLengh >= m_Parameter.ResearchLenght) m_NowMoveState = EMoveState::Rotation;

	if (lenght >= 1.0f) return;

	m_NowMoveState = EMoveState::Wait;
}

// �ҋ@�֐�.
void CEventAlien::WaitMove()
{
	if (m_NowMoveState != EMoveState::Wait) return;
	m_WaitCount++;	// �ҋ@�J�E���g���Z.
	if (m_WaitCount < m_Parameter.WaitTime*FPS) return;
	m_NowMoveState = EMoveState::Rotation;	// �ړ���Ԃ���]����.
	m_WaitCount = 0;	// �ҋ@�J�E���g�̏�����.
}

// �X�|�[����.
void CEventAlien::Spawning()
{
	// ���f���̃X�P�[���l�𑫂��Ă���.

	// ���f���̃A���t�@�l�𑫂��Ă���.
	m_ModelAlpha += m_Parameter.ModelAlphaAddValue;
	if (m_ModelAlpha < MODEL_ALPHA_MAX) return;
	m_NowState = EEventAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// �ړ�.
void CEventAlien::Move()
{
	TargetRotation();			// ��].
	CEventAlien::VectorMove(m_MoveSpeed);	// �ړ�.
	CEventAlien::WaitMove();							// �ҋ@.

	if (m_NowState == EEventAlienState::Abduct) return;
	m_NowState = EEventAlienState::Escape;
	m_NowMoveState = EMoveState::Rotation;	// �ړ���Ԃ���]����.
}

// ����.
void CEventAlien::Abduct()
{
	if (m_IsBarrierHit == true) return;

	SetMoveVector(*m_pAbductUFOPosition);
	m_vDestination = *m_pAbductUFOPosition;

	TargetRotation();
	CEventAlien::VectorMove(m_MoveSpeed);
}

// ������.
void CEventAlien::Escape()
{
	if (m_IsBarrierHit == true) return;

	SetMoveVector(*m_pAbductUFOPosition);
	m_vDestination = *m_pAbductUFOPosition;
	TargetRotation();
	CEventAlien::VectorMove(m_MoveSpeed);
}

// �������.
void CEventAlien::BlowAway()
{
}

// �ҋ@.
void CEventAlien::Wait()
{
}

// ���̎q�Ƃ̓����蔻��.
void CEventAlien::GirlCollision(CEventActor* pActor)
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if (pActor->GetObjectTag() != EObjectTag::Girl) return;
	if (m_IsBarrierHit == true) return;
	if (m_NowMoveState == EMoveState::Attack) return;	// �U����Ԃ͏I��.
	if (m_NowState == EEventAlienState::Spawn) return;	// �X�|�[����ԂȂ�I��.

	bool isAbduct = false;
	if (m_NowState == EEventAlienState::Abduct) {
		isAbduct = true;
	}
	else {
		isAbduct = true;
	}

	if (isAbduct == false) return;
	// ���̂̓����蔻��.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
	pActor->SetTargetPos(*this);

	if (m_NowState == EEventAlienState::Abduct) return;
	m_NowState = EEventAlienState::Abduct;
	m_NowMoveState = EMoveState::Rotation;
}

// �o���A�Ƃ̓����蔻��.
void CEventAlien::BarrierCollision(CEventActor* pActor)
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if (pActor->GetObjectTag() != EObjectTag::Bariier) return;
	const float moveSpeed = -2.0f;
	// ���̂̓����蔻��.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) {
		m_MoveSpeed = m_Parameter.MoveSpeed;
		m_IsBarrierHit = false;

	}
	else {
		m_MoveSpeed = moveSpeed;
		m_IsBarrierHit = true;
		m_NowState = EEventAlienState::Move;
		m_NowMoveState = EMoveState::Move;
	}
}
