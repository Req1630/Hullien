#ifndef EVENT_CHARACTER_H
#define EVENT_CHARACTER_H

#include "..\Actor.h"

#define IS_TEMP_MODEL_RENDER	// �����f���\��.

/***************************************
*	�C�x���g�p�L�����N�^�N���X.
**/
class CEventCharacter : public CActor
{
	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
//	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

public:
	// �C�ӂŐݒ肷����.
	struct stOptionalState
	{
		D3DXVECTOR3		Destination;				// �ړI�n.
		float						RotationalSpeed;			// ��]���x.
		float						MoveSpeed;				// �ړ����x.

	}typedef SOptionalState;

protected:
	const float INIT_POSITION_ADJ_HEIGHT = 4.0f;	// �����p���W�̍���.

	// �ړ����.
	enum class enMoveState
	{
		None,

		Rotation,
		Move,

		Max,
	} typedef EMoveState;

	// �L�����N�^�̏��.
	struct stCharacterParam : public SOptionalState
	{
		float						ModelAlphaAddValue;	// ���f���̃A���t�@�l�̉��Z����l.
		float						ResearchLenght;			// �Č�������ۂ̋���.
		D3DXVECTOR3		SphereAdjPos;				// �X�t�B�A�̒������W.
		float						SphereAdjRadius;			// �X�t�B�A�̒������a.

		stCharacterParam()
			: ModelAlphaAddValue(0.0f)
			, ResearchLenght(0.0f)
			, SphereAdjPos(0.0f, 0.0f, 0.0f)
			, SphereAdjRadius(0.0f)
		{}
	}typedef SCharacterParam;

public:
	CEventCharacter();
	virtual ~CEventCharacter();

	// �ړ��֐�.
	virtual void Move();

	// ���ݒ�֐�.
	void SetOptionalState(SOptionalState state);

	// ���W�ʈʒu�ݒ�֐�.
	void SetPositionX(const float& vPos_x);
	void SetPositionY(const float& vPos_y);
	void SetPositionZ(const float& vPos_z);

	// ��]�l�擾�֐�.
	D3DXVECTOR3 GetRotation() const;
	// ��]�l�ݒ�֐�.
	void SetRotation(const D3DXVECTOR3& vRot);
	void SetRotationX(const float& vRot_x);
	void SetRotationY(const float& vRot_y);
	void SetRotationZ(const float& vRot_z);

	// �傫���擾�֐�.
	D3DXVECTOR3 GetScale() const;
	// �傫���ݒ�֐�.
	void SetScale(const D3DXVECTOR3& vScale);
	void SetScaleX(const float& vScale_x);
	void SetScaleY(const float& vScale_y);
	void SetScaleZ(const float& vScale_z);
	
protected:
	// ���b�V���̕\���֐�.
	void MeshRender();
	// ���f���̎擾�֐�.
	bool GetModel(const char* modelName);
	// �ړ��x�N�g���ݒ�֐�.
	virtual void SetMoveVector(const D3DXVECTOR3& targetPos);
	// �ړI�̍��W�։�].
	void TargetRotation();
	// �ړ��֐�.
	virtual void VectorMove(const float& moveSpeed);


protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// �X�L�����b�V��.
#ifdef IS_TEMP_MODEL_RENDER
	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;	// ���̃��f���f�[�^.
#endif	// #ifdef IS_TEMP_MODEL_RENDER.

	D3DXVECTOR3		m_MoveVector;					// �ړ��x�N�g��.
	D3DXVECTOR3		m_TargetRotation;				// �ڕW�̉�]���.
	D3DXVECTOR3		m_BeforeMoveingPosition;		// �ړ��O�̍��W.
	SCharacterParam	m_Parameter;						// �p�����[�^.
	EMoveState			m_NowMoveState;				// ���݂̈ړ����.
	float						m_ModelAlpha;						// ���f���̃A���t�@�l.
};

#endif	//#ifndef EVENT_CHARACTER_H.
