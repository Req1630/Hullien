#ifndef ARM_H
#define ARM_H

#include "..\GameObject.h"

class CArm : public CGameObject
{
	const char* MODEL_NAME = "arm_open";

	const float APPEARANCE_ADD_VALUE	= 0.005f;
	const float APPEARANCE_COUNT_MAX	= 1.0f;
	const float SCALING_VALUE			= 0.8f;
	const float SCALING_VALUE_MAX		= 1.0f;
	const float SCALING_ADD_VALUE		= (1.0f-SCALING_VALUE)*0.04f;

	enum class enArmState
	{
		None,

		Appearance,	// �o��.


		Max,

		Start = Appearance,

	} typedef EArmState;
public:
	CArm();
	virtual ~CArm();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// �o��.
	void Appearance();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pSkinMesh;	// �X�L�����b�V��.
	std::shared_ptr<CDX9StaticMesh>	m_pAMesh;	// �X�L�����b�V��.
	EArmState						m_NowArmState;
	float	m_AppearanceCount;	// �o���J�E���g.
	float	m_ScalingValue;		// �g�k�l.
	bool	m_IsScaleDown;		// �k�����Ă��邩.
};

#endif	// #ifndef ARM_H.