#include "EventPlayer.h"

/********************************
*	�C�x���g�p�v���C���[�N���X.
**/
CEventPlayer::CEventPlayer()
{
}

CEventPlayer::~CEventPlayer()
{
}

// �������֐�
bool CEventPlayer::Init()
{
	if (GetModel(MODEL_NAME) == false) return false;

	return true;
}

// �X�V�֐�.
void CEventPlayer::Update()
{
}

// �`��֐�.
void CEventPlayer::Render()
{
	MeshRender();	// ���b�V���̕`��.
	EffectRender();
}

// �����蔻��֐�.
void CEventPlayer::Collision(CActor * pActor)
{
}

// ������W�̐ݒ�֐�.
void CEventPlayer::SetTargetPos(CActor & actor)
{
}

// �X�v���C�g�̕`��.
void CEventPlayer::SpriteRender()
{
}

// ����\�͂��g���Ă��邩.
bool CEventPlayer::IsSpecialAbility()
{
	if (m_HasUsableSP == false) return false;
	// ����\�͂��g����Ȃ�.
	m_HasUsableSP = false;	// ����������.
	return true;			// true��Ԃ�.
}

// �U������֐�.
void CEventPlayer::AttackController()
{
}

// ����\�͑���֐�.
void CEventPlayer::SPController()
{
}

// �ړ��֐�.
void CEventPlayer::Move()
{
}

// �G�t�F�N�g�`��֐�.
void CEventPlayer::EffectRender()
{
}

// �U���̓����蔻��.
void CEventPlayer::AttackCollision(CActor * pActor)
{
}

// �A�j���[�V�����ݒ�.
void CEventPlayer::SetAnimation(const EAnimNo & animNo)
{
}

// �����蔻��̐ݒ�.
bool CEventPlayer::ColliderSetting()
{
	return false;
}

// �G�t�F�N�g�̐ݒ�.
bool CEventPlayer::EffectSetting()
{
	return false;
}

