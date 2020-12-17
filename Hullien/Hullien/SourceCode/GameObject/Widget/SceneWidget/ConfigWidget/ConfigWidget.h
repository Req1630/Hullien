#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CConfigCursor;
class CVolumeConfigWidget;		// ���ʐݒ�UI�N���X.
class CCameraConfigWidget;		// �J�����ݒ�UI�N���X.
class CControllerConfigWidget;	// �R���g���[���[�ݒ�UI�N���X.
class CGraphicConfigWidget;		// �O���t�B�b�N�ݒ�UI�N���X.

/********************************************
*	�ݒ�UI�N���X.
**/
class CConfigWidget : public CSceneWidget
{

	const char* SPRITE_BACK			= "ConfigBack";
	const char* SPRITE_VOLUME		= "Volume";
	const char* SPRITE_CAMERA		= "Camera";
	const char* SPRITE_CONTROLLER	= "Controller";
	const char* SPRITE_GRAPHIC		= "Graphic";
	const char* SPRITE_RETURN_TITLE	= "BackTitle";

	const int BACKGROUND	= 0;	//�w�i�z��ԍ�.
	const int VOLUME		= 1;	//�J�n�z��ԍ�.
	const int CAMERA		= 2;	//�J�n�z��ԍ�.
	const int CONTROLLER	= 3;	// 
	const int GRAPHIC		= 4;	// 

	enum enConfigState
	{
		EConfigState_None,

		EConfigState_Volume,	// ���ʂ̐ݒ�.
		EConfigState_Camera,	// �J�����̑���.
		EConfigState_Controller,// �R���g���[���[�̐ݒ�.
		EConfigState_Graphic,	// �O���t�B�b�N�̐ݒ�.
		EConfigState_BackTitle,	// �^�C�g���֖߂�.

		EConfigState_End,

		EConfigState_Max,
	} typedef EConfigState;

public:
	CConfigWidget();
	CConfigWidget( const bool& isGame );
	virtual ~CConfigWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// ���ʂ̐ݒ���ł��Ȃ��悤�ɂ���.
	void OffVolumeSeting();

	// �ݒ���I��������.
	bool IsEndConfig();

	// �^�C�g���ɖ߂邩.
	bool IsReturnToTitle();

private:
	// �J�[�\���̐ݒ�.
	void CursorSetting();
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::unique_ptr<CConfigCursor>			m_pCursor;				// �J�[�\���N���X.
	std::unique_ptr<CVolumeConfigWidget>	m_pVolumeConfig;		// ���ʐݒ�UI.
	std::unique_ptr<CCameraConfigWidget>	m_pCameraConfig;		// �J�����ݒ�UI.
	std::unique_ptr<CControllerConfigWidget>m_pControllerConfig;	// �R���g���[���[�ݒ�UI.
	std::unique_ptr<CGraphicConfigWidget>	m_pGraphicConfig;		// �O���t�B�b�N�ݒ�UI.
	D3DXVECTOR3	m_ReturnTitlePosition;	// �^�C�g���֖߂�摜�̍��W.
	int			m_SelectState;			// �I�����.
	int			m_OldSelectState;
	int			m_NowConfigState;		// ���݂̐ݒ�̏��.
	bool		m_IsNowGameScene;		// �Q�[���V�[�����ǂ���.
	bool		m_IsReturnToTitle;		// �^�C�g���ɖ߂�.
};

#endif	// #ifndef CONFIG_WIDGET_H.