#ifndef UFO_LIGHT_H
#define UFO_LIGHT_H

#include "..\GameObject.h"

class CUFOLight : public CGameObject
{
	const char* MODEL_NAME = "light";
public:
	CUFOLight();
	virtual ~CUFOLight();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
};

#endif	// #ifndef UFO_LIGHT_H.