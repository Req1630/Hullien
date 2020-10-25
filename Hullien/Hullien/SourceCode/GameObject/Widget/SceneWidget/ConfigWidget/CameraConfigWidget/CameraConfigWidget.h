#ifndef CAMERA_CONFIG_WIDGET_H
#define CAMERA_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include <vector>

class CCursor;

class CCameraConfigWidget : public CWidget
{
	const char* SPRITE_CONTROL_NAME = "CmaeraControl";
	const char* SPRITE_SPEED_NAME = "CmaeraSpeed";
	const char* SPRITE_NORMAL_NAME = "CameraNormal";
	const char* SPRITE_INVERSION_NAME = "CameraInversion";
	const char* SPRITE_CHOICE_NAME = "Choice";

	// �I�����.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_CameraControl,
		ESelectState_CameraSpeed,

		ESelectState_Max,
	} typedef ESelectState;

	// �ݒ���.
	enum enConfigState
	{
	};
public:
	CCameraConfigWidget();
	virtual ~CCameraConfigWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// �摜.
	std::unique_ptr<CCursor>				m_pCursor;
};

#endif	// #ifndef CAMERA_CONFIG_WIDGET_H.