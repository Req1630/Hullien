#ifndef UFO_LIGHT_H
#define UFO_LIGHT_H

#include "..\GameObject.h"

class CUFOLight : public CGameObject
{
	const char* MODEL_NAME = "light";	// ���f����.

	const float MODEL_ALPHA					= 0.5f;		// ���f���A���t�@�l.
	const float SCALE_UP_HEIGHT_SPEED		= 0.2f;		// �f�t�H���g�����g�呬�x.
	const float SCALE_UP_WEDTH_SPEED		= 0.02f;	// �f�t�H���g���g�呬�x.
	const float DEFAULT_SCALE_HEIGHT_MAX	= 1.0f;		// �����̍ő�T�C�Y.
	const float DEFAULT_SCALE_WEDTH_MAX		= 1.0f;		// ���̍ő�T�C�Y,
	
	const float SCALE_HEIGHT_MAX;	// �����g�呬�x.
	const float SCALE_WEDTH_MAX;	// ���g�呬�x.

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
	CUFOLight( const float& height, const float& wedht );
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

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// ���b�V��.
	EUFOLightState					m_NowState;		// ���݂̏��.
};

#endif	// #ifndef UFO_LIGHT_H.