#include "EventAlien.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

/****************************************
*	�C�x���g�p�F���l�N���X.
**/
CEventAlien::CEventAlien()
	: m_pAbductUFOPosition		( nullptr )
	, m_NowState				( EEventAlienState::None )
{
}

CEventAlien::~CEventAlien()
{
}

// ������W�̐ݒ�.
void CEventAlien::SetTargetPos(CActor & actor)
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
	default:
		break;
	}
}

// ���̎q�̍��W��ݒ�.
void CEventAlien::SetGirlPos(CActor& actor)
{
	if (m_NowMoveState == EMoveState::Move) return;
	// ���̎q����Ȃ���ΏI��.
	if (actor.GetObjectTag() != EObjectTag::Girl) return;

}

// �X�|�[����.
void CEventAlien::Spawning()
{
	if (m_Parameter.IsDisp == false)
	{
		m_Parameter.IsDisp = true;	
		m_Parameter.vScale = {0.0f,0.0f,0.0f};
	}

	// ���f���̃A���t�@�l�𑫂��Ă���.
	if (m_Parameter.ModelAlpha >= MODEL_ALPHA_MAX) return;
	m_Parameter.ModelAlpha += m_Parameter.AlphaSpeed;

	// ���f���̃X�P�[���l�𑫂��Ă���.
	if (m_Parameter.vScale.x >= MODEL_SCALE_MAX) return;
	m_vSclae.x += m_Parameter.ScaleSpeed;
	m_vSclae.y += m_Parameter.ScaleSpeed;
	m_vSclae.z += m_Parameter.ScaleSpeed;
	if (m_Parameter.vScale.x > MODEL_SCALE_MAX)
	{
		m_Parameter.vScale.x = MODEL_SCALE_MAX;
		m_Parameter.vScale.y = MODEL_SCALE_MAX;
		m_Parameter.vScale.z = MODEL_SCALE_MAX;
	}

}

// �ړ�.
void CEventAlien::Move()
{
}

// ����.
void CEventAlien::Abduct()
{
	if (m_IsBarrierHit == true) return;
}

// ������.
void CEventAlien::Escape()
{
	if (m_IsBarrierHit == true) return;
}

// �������.
void CEventAlien::BlowAway()
{
}

// ���̎q�Ƃ̓����蔻��.
void CEventAlien::GirlCollision(CActor* pActor)
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if (pActor->GetObjectTag() != EObjectTag::Girl) return;
	if (m_IsBarrierHit == true) return;
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
void CEventAlien::BarrierCollision(CActor* pActor)
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if (pActor->GetObjectTag() != EObjectTag::Bariier) return;
	const float moveSpeed = -2.0f;
	// ���̂̓����蔻��.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) {
		m_Speed  = m_Parameter.MoveSpeed;
		m_IsBarrierHit = false;

	}
	else {
		m_Speed = moveSpeed;
		m_IsBarrierHit = true;
		m_NowState = EEventAlienState::Move;
		m_NowMoveState = EMoveState::Move;
	}
}
