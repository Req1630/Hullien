#include "EventPlayer.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Actor\Actor.h"
#include "..\...\..\..\..\..\Utility\XInput\XInput.h"

/********************************
*	�C�x���g�p�v���C���[�N���X.
**/
CEventPlayer::CEventPlayer()
	: m_NowAnimNo()
	, m_OldAnimNo()
	, m_AttackPosition()
	, m_pEffects( )
	, m_SpecialAbility( 0.0f )
	, m_HasUsableSP(false)
	, m_IsAttackSE(false)
{
	m_NowMoveState = EMoveState::Rotation;
}

CEventPlayer::~CEventPlayer()
{
}

// �������֐�
bool CEventPlayer::Init()
{
	if (GetModel(MODEL_TEMP_NAME) == false) return false;

	return true;
}

// �X�V�֐�.
void CEventPlayer::Update()
{
	Move();
	SPController();			// ����\�͑���.
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

// ����\�͂��g���Ă��邩.
bool CEventPlayer::IsSpecialAbility()
{
	if (m_HasUsableSP == false) return false;
	// ����\�͂��g����Ȃ�.
	m_HasUsableSP = false;	// ����������.
	return true;			// true��Ԃ�.
}

// ����\�͑���֐�.
void CEventPlayer::SPController()
{
	// Y�{�^���������ꂽ�u�Ԃ���Ȃ���ΏI��.
//	if (CXInput::Y_Button() != CXInput::enPRESSED_MOMENT) return;
	if (GetAsyncKeyState('Y') & 0x8000)
	{
		m_SpecialAbility = 0.0f;
		m_HasUsableSP = true;
	}
}

// �ړ��֐�.
void CEventPlayer::Move()
{
	m_Parameter.ResearchLenght = 3.0f;
	CEventCharacter::Move();
}

// �G�t�F�N�g�`��֐�.
void CEventPlayer::EffectRender()
{
}

// �A�j���[�V�����ݒ�.
void CEventPlayer::SetAnimation(const EAnimNo & animNo)
{
	if (m_pSkinMesh == nullptr) return;
	if (m_NowAnimNo == m_OldAnimNo) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimSet(static_cast<int>(animNo));
}

// �����蔻��̐ݒ�.
bool CEventPlayer::ColliderSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return false;
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	return true;
#else
	if (m_pTempStaticMesh == nullptr) return false;
	// ���b�V�����g�p���Ă̓����蔻�菉����.
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;

	// �U���p�̓����蔻�菉����.
	if (m_pAttackCollManager == nullptr) {
		m_pAttackCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pAttackCollManager->InitSphere(
		&m_AttackPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		1.0f))) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

// �G�t�F�N�g�̐ݒ�.
bool CEventPlayer::EffectSetting()
{
	return false;
}

