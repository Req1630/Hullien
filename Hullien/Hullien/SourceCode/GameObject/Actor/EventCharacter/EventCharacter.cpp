#include "EventCharacter.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

/***************************************
*	�C�x���g�p�L�����N�^�N���X.
**/
CEventCharacter::CEventCharacter()
	: m_pSkinMesh( nullptr )
#ifdef IS_TEMP_MODEL_RENDER
	, m_pTempStaticMesh( nullptr )
#endif
	, m_MoveVector					( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation				( 0.0f, 0.0f, 0.0f )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter						()
	, m_NowMoveState				( EMoveState::None )
	, m_ModelAlpha					( 0.0f )
{
	m_vPosition.y = INIT_POSITION_ADJ_HEIGHT;
}

CEventCharacter::~CEventCharacter()
{
}

// �ړ��֐�.
void CEventCharacter::Move()
{
	TargetRotation();
	CEventCharacter::VectorMove(m_Parameter.MoveSpeed);
}

// ���ݒ�֐�.
void CEventCharacter::SetOptionalState(SOptionalState state)
{
	m_Parameter.Destination = state.Destination;
	m_Parameter.MoveSpeed = state.MoveSpeed;
	m_Parameter.RotationalSpeed = state.RotationalSpeed;
}

// ���b�V���̕\���֐�.
void CEventCharacter::MeshRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return;

	m_pSkinMesh->SetPosition(m_vPosition);
	m_pSkinMesh->SetRotation(m_vRotation);
	m_pSkinMesh->SetScale(m_vSclae);
	m_pSkinMesh->SetAnimSpeed(0.01);
	m_pSkinMesh->Render();
#else
	if (m_pTempStaticMesh == nullptr) return;

	m_pTempStaticMesh->SetPosition(m_vPosition);
	m_pTempStaticMesh->SetRotation(m_vRotation);
	m_pTempStaticMesh->SetScale(m_vSclae);
	m_pTempStaticMesh->Render();
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// ���f���̎擾�֐�
bool CEventCharacter::GetModel(const char * modelName)
{
#ifndef IS_TEMP_MODEL_RENDER
	// ���ɓǂݍ��߂Ă�����I��.
	if (m_pSkinMesh != nullptr) return true;
	// ���f���̎擾.
	CMeshResorce::GetSkin(m_pSkinMesh, modelName);
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if (m_pSkinMesh == nullptr) return false;
	return true;
#else
	// ���ɓǂݍ��߂Ă�����I��.
	if (m_pTempStaticMesh != nullptr) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic(m_pTempStaticMesh, modelName);
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if (m_pTempStaticMesh == nullptr) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

// �ړ��x�N�g���ݒ�֐�.
void CEventCharacter::SetMoveVector(const D3DXVECTOR3& targetPos)
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
void CEventCharacter::TargetRotation()
{
	if (m_NowMoveState != EMoveState::Rotation) return;

	// �ړI�n�̃x�N�g����p��.
	SetMoveVector( m_Parameter.Destination );
	
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
void CEventCharacter::VectorMove(const float& moveSpeed)
{
	if (m_NowMoveState != EMoveState::Move) return;

	float lenght = D3DXVec3Length(&D3DXVECTOR3(m_Parameter.Destination - m_vPosition));

	m_vPosition.x -= sinf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;
	m_vPosition.z -= cosf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;

	float researchLengh = D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_vPosition));
	if (researchLengh >= m_Parameter.ResearchLenght) m_NowMoveState = EMoveState::Rotation;

	if (lenght >= 1.0f) return;
}

/******************************************************
*	�ȉ����擾�E�ݒ�֐�.
*/

// X���W�ʒu�ݒ�֐�.
void CEventCharacter::SetPositionX(const float& vPos_x)
{
	m_vPosition.x = vPos_x;
}

// Y���W�ʒu�ݒ�֐�.
void CEventCharacter::SetPositionY(const float& vPos_y)
{
	m_vPosition.y = vPos_y;
}

// Z���W�ʒu�ݒ�֐�.
void CEventCharacter::SetPositionZ(const float& vPos_z)
{
	m_vPosition.z = vPos_z;
}

// ��]�l�擾�֐�.
D3DXVECTOR3 CEventCharacter::GetRotation() const
{
	return m_vRotation;
}

// ��]�l�ݒ�֐�.
void CEventCharacter::SetRotation(const D3DXVECTOR3& vRot)
{
	m_vRotation = vRot;
}

// X���W��]�l�ݒ�֐�.
void CEventCharacter::SetRotationX(const float& vRot_x)
{
	m_vRotation.x = vRot_x;
}

// Y���W��]�l�ݒ�֐�.
void CEventCharacter::SetRotationY(const float& vRot_y)
{
	m_vRotation.y = vRot_y;
}

// Z���W��]�l�ݒ�֐�.
void CEventCharacter::SetRotationZ(const float& vRot_z)
{
	m_vRotation.z = vRot_z;
}

// �傫���擾�֐�.
D3DXVECTOR3 CEventCharacter::GetScale() const
{
	return m_vSclae;
}

// �傫���ݒ�֐�.
void CEventCharacter::SetScale(const D3DXVECTOR3& vScale)
{
	m_vSclae = vScale;
}

// X���W�傫���ݒ�֐�.
void CEventCharacter::SetScaleX(const float & vScale_x)
{
	m_vSclae.x = vScale_x;
}

// Y���W�傫���ݒ�֐�.
void CEventCharacter::SetScaleY(const float & vScale_y)
{
	m_vSclae.y = vScale_y;
}

// Z���W�傫���ݒ�֐�.
void CEventCharacter::SetScaleZ(const float & vScale_z)
{
	m_vSclae.z = vScale_z;
}
