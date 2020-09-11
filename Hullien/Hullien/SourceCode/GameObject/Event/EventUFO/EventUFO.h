#ifndef EVENT_UFO_H
#define EVENT_UFO_H

#include "..\..\GameObject.h"

/**************************************
*	�C�x���g�pUFO�N���X.
**/
class CEventUFO : public CGameObject
{
	const char* MODEL_NAME = "ufo_mini";	// ���f����.
	const float POS_HEIGHT = 10.0f;			// UFO�̍���.

public:
	CEventUFO();
	virtual ~CEventUFO();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �傫���̐ݒ�֐�.
	void SetScale(const D3DXVECTOR3& vScale) { m_vSclae = vScale; }

private:
	// ���f���̎擾.
	bool GetModel();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// ���b�V��.

};

#endif //#ifndef EVENT_UFO_H.
