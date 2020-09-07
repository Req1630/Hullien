#ifndef SKY_DOME_H
#define SKY_DOME_H

#include "..\GameObject.h"

class CSkyDome : public CGameObject
{
	const char* MODEL_NAME = "space";	// ���f����.

public:
	CSkyDome();
	virtual ~CSkyDome();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// ���f���̎擾.
	bool GetModel();

private:
	std::shared_ptr<CDX9StaticMesh>	m_StaticMesh;	// ���b�V��.
};

#endif	// #ifndef SKY_DOME_H.