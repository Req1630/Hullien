#ifndef EVENT_CHARACTER_H
#define EVENT_CHARACTER_H

#include "..\Actor.h"

#define IS_TEMP_MODEL_RENDER	// �����f���\��.

/***************************************
*	�C�x���g�p�L�����N�^�N���X.
**/
class CEventCharacter : public CActor
{
protected:
	const float INIT_POSITION_ADJ_HEIGHT = 4.0f;	// �����p���W�̍���.

public:
	CEventCharacter();
	virtual ~CEventCharacter();

	// �ړ��֐�.
	virtual void Move() = 0;
	// �ړI�n�̎擾�֐�.
	D3DXVECTOR3 SetDestination() const { return m_vDestination; }
	// �ړI�n�̐ݒ�֐�.
	void SetDestination(const D3DXVECTOR3& vPos) { m_vDestination = vPos; }

	// ���W�ʈʒu�ݒ�֐�.
	void SetPositionX(const float& x);
	void SetPositionY(const float& y);
	void SetPositionZ(const float& z);

	// ��]�l�擾�֐�.
	D3DXVECTOR3 GetRotation() const;
	// ��]�l�ݒ�֐�.
	void SetRotation(const D3DXVECTOR3& vRot);
	void SetRotationX(const float& x);
	void SetRotationY(const float& y);
	void SetRotationZ(const float& z);

	// �傫���擾�֐�.
	D3DXVECTOR3 GetScale() const;
	
protected:
	// ���b�V���̕\���֐�.
	void MeshRender();
	// ���f���̎擾�֐�.
	bool GetModel(const char* modelName);

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// �X�L�����b�V��.
#ifdef IS_TEMP_MODEL_RENDER
	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;	// ���̃��f���f�[�^.
#endif	// #ifdef IS_TEMP_MODEL_RENDER.

	D3DXVECTOR3 m_vDestination;		// �ړI�n.
	D3DXVECTOR3 m_MoveVector;			// �ړ��x�N�g��.
	float m_MoveSpeed;							// �ړ����x.
};

#endif	//#ifndef EVENT_CHARACTER_H.
