#include "EventGirl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\Actor\Actor.h"

/****************************************
*	�C�x���g�p���̎q�N���X.
**/
CEventGirl::CEventGirl()
	: m_Parameter					()
	, m_pSearchCollManager	( nullptr )
	, m_OldPosition				( 0.0f, 0.0f, 0.0f )
	, m_NowState					( ENowState::None )
	, m_NowMoveState			( EMoveState::None )
{
	m_ObjectTag = EObjectTag::Girl;
	m_NowState = ENowState::Protected;
	m_NowMoveState = EMoveState::Wait;
	m_pSearchCollManager = std::make_shared<CCollisionManager>();
}

CEventGirl::~CEventGirl()
{
}

// �������֐�.
bool CEventGirl::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (GetModel(MODEL_NAME) == false) return false;
#else
	if (GetModel(MODEL_TEMP_NAME) == false) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if (ColliderSetting() == false) return false;

	return true;
}

// �X�V�֐�.
void CEventGirl::Update()
{
	switch (m_NowState)
	{
	case ENowState::None:
		break;
	case ENowState::Protected:
		break;
	case ENowState::Abduct:
		break;
	case ENowState::Move:
		Move();
		break;
	case ENowState::Wait:
		break;
	default:
		break;
	}
}

// �`��֐�
void CEventGirl::Render()
{
	MeshRender();	// ���b�V���̕`��.

#if _DEBUG
	if (m_pCollManager == nullptr) return;
	m_pCollManager->DebugRender();
	if (m_pSearchCollManager == nullptr) return;
	m_pSearchCollManager->DebugRender();
#endif	// #if _DEBUG.

}

// �����蔻��֐�.
void CEventGirl::Collision(CEventActor * pActor)
{
	if (pActor == nullptr) return;
	SearchCollision(pActor);
	if (m_pCollManager == nullptr) return;
	if (m_pCollManager->GetSphere() == nullptr) return;
}

// ������W�̐ݒ�֐�
void CEventGirl::SetTargetPos(CEventActor& actor)
{
	m_vPosition = actor.GetPosition();
	m_NowState = ENowState::Abduct;
}

void CEventGirl::SetTargetPos(CActor& actor)
{
	m_vPosition = actor.GetPosition();
	m_NowState = ENowState::Abduct;
}

// �ړ��֐�.
void CEventGirl::Move()
{
	switch (m_NowMoveState)
	{
	case EMoveState::None:
		break;
	case EMoveState::Rotation:
		TargetRotation();
		break;
	case EMoveState::Move:
		TargetMove();
		break;
	case EMoveState::Wait:
		break;
	default:
		break;
	}
}

// �ړI�̏ꏊ�Ɍ����ĉ�].
void CEventGirl::TargetRotation()
{
	if (m_NowMoveState != EMoveState::Rotation) return;

	const D3DXVECTOR3 targetPosition = { 0.0f, 0.0f, 0.0f };
	// �ړI�̉�]�����擾.
	D3DXVECTOR3 targetRotation = { 0.0f, 0.0f, 0.0f };
	targetRotation.y = atan2f(
		targetPosition.x - m_vPosition.x,
		targetPosition.z - m_vPosition.z);
	m_MoveVector.x = sinf(targetRotation.y);
	m_MoveVector.z = cosf(targetRotation.y);

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

	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

	// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// ���ς����e�͈͂Ȃ�.
	if (-TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN) {
		m_vRotation.y = targetRotation.y;	// �^�[�Q�b�g�ւ̉�]�擾.
											// �ړ��p�x�N�g�����擾.
		m_MoveVector.x = sinf(m_vRotation.y);
		m_MoveVector.z = cosf(m_vRotation.y);
		m_NowMoveState = EMoveState::Move;
	}
}

// �ړI�̏ꏊ�Ɍ����Ĉړ�.
void CEventGirl::TargetMove()
{
	if (m_NowMoveState != EMoveState::Move) return;

	// �ړI�̏ꏊ.
	const float moveSpeed = 0.05f;
	m_vPosition.x -= sinf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;
	m_vPosition.z -= cosf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;

	float lenght = D3DXVec3Length(&D3DXVECTOR3(m_vDestination - m_vPosition));

	if (lenght >= 1.0f) return;

	m_NowMoveState = EMoveState::Wait;

}

// ���G�̓����蔻��.
void CEventGirl::SearchCollision(CEventActor * pActor)
{
	if (pActor == nullptr) return;
	if (m_pSearchCollManager == nullptr) return;
	if (m_pSearchCollManager->GetSphere() == nullptr) return;

	// ���ɘA�ꋎ���Ă�����I��.
	if (m_NowState == ENowState::Abduct) return;

	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if ((pActor->GetObjectTag() != EObjectTag::Alien_A) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_B) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_C) &&
		(pActor->GetObjectTag() != EObjectTag::Alien_D)) return;

	// ���̂̓����蔻��.
	if (m_pSearchCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
}

// �����蔻��̍쐬.
bool CEventGirl::ColliderSetting()
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
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	// ���̎q�̓����蔻��.
	if (FAILED(m_pCollManager->InitSphere(
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius))) return false;
	// ���G�̓����蔻��.
	if (FAILED(m_pSearchCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SearchCollRadius))) return false;

	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}
