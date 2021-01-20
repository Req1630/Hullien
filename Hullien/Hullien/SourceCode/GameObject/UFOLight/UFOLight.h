#ifndef UFO_LIGHT_H
#define UFO_LIGHT_H

#include "..\GameObject.h"

class CUFOLight : public CGameObject
{
	const char* MODEL_NAME = "light";	// ���f����.

	const float MODEL_ALPHA					= 0.15f;	// ���f���A���t�@�l.
	const float SCALE_UP_HEIGHT_SPEED		= 0.2f;		// �f�t�H���g�����g�呬�x.
	const float SCALE_UP_WEDTH_SPEED		= 0.02f;	// �f�t�H���g���g�呬�x.
	const float DEFAULT_SCALE_HEIGHT_MAX	= 1.0f;		// �����̍ő�T�C�Y.
	const float DEFAULT_SCALE_WEDTH_MAX		= 1.0f;		// ���̍ő�T�C�Y,
	const float DEFAULT_SCALE_HEIGHT_MIN	= 0.0f;		// �����̍ŏ��T�C�Y.
	const float DEFAULT_SCALE_WEDTH_MIN		= 0.0f;		// ���̍ŏ��T�C�Y,

	// UFO���C�g�̏��.
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
	CUFOLight( const float& height, const float& wedth );
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
	// �������S�ɕ��o����.
	void DischargePreparation();
	// ����ЂÂ���.
	void CleanUPPreparation();

	// ���C�g�̑傫����ݒ�.
	void SetLightScale( const float& height, const float& wedth );

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// ���b�V��.
	EUFOLightState					m_NowState;		// ���݂̏��.
	float	m_ScaleHeightMax;
	float	m_ScaleWedthMax;
};

#endif	// #ifndef UFO_LIGHT_H.