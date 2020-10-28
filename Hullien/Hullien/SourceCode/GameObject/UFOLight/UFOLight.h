#ifndef UFO_LIGHT_H
#define UFO_LIGHT_H

#include "..\GameObject.h"

class CUFOLight : public CGameObject
{
	const char* MODEL_NAME = "light";

	const float SCALE_UP_HEIGHT_SPEED = 0.2f;
	const float SCALE_UP_WEDTH_SPEED = 0.02f;
	const float SCALE_MAX = 1.0f;

	enum class enUFOLightState
	{
		None,
		

		Discharge,		// ���o��.
		CleanUP,		// �ЂÂ���.
		EndDischarge,	// ���o���I�����.
		EndCleanUP,		// �ЂÂ��I�����.

		Max,
	} typedef EUFOLightState;
public:
	CUFOLight();
	virtual ~CUFOLight();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ������o����.
	void Discharge();
	// ����ЂÂ���.
	void CleanUP();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
	EUFOLightState					m_NowState;
};

#endif	// #ifndef UFO_LIGHT_H.