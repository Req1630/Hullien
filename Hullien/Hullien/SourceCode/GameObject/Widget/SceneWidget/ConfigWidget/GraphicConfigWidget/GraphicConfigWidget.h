#ifndef GRAPHIC_CONFIG_WIDGET_H
#define GRAPHIC_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include "..\..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include <vector>

class CCursor;
class CSlinder;

class CGraphicConfigWidget : public CWidget
{
	const char* SPRITE_FULL_SC_NAME		= "FullSC";
	const char* SPRITE_CHOICE_NAME		= "Choice";
	const char* SPRITE_ON_NAME			= "FullSCOn";
	const char* SPRITE_OFF_NAME			= "FullSCOff";
	const float INPUT_WAIT_TIME_MAX		= 30.0;
	// �I�����.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,	// ���ڑI�����.
		ESelectState_FullSC,	// �t���X�N���[���I��.

		ESelectState_Max,
	} typedef ESelectState;

	enum enControlllerState
	{
		EControlllerState_None,

		EControlllerState_FullSCOn,		// �t���X�N���[���t���X�N���[��On.
		EControlllerState_FullSCOff,	// �t���X�N���[���t���X�N���[��Off.

		EControlllerState_Max,
	} typedef EControlllerState;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Control,	// ����.
		ESpriteNo_Choise,	// �I��.
		ESpriteNo_On,		// On.
		ESpriteNo_Off,		// Off.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CGraphicConfigWidget();
	virtual ~CGraphicConfigWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �I��������.
	bool IsEnd();

private:
	// ��ނ̑I��.
	void SelectType();
	// ����.
	void Determination();
	// �o�C�u�I���̍X�V.
	void FullScChoiceUpdate();
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// �摜.
	std::unique_ptr<CCursor>				m_pCursor;
	bool	m_IsFullScreen;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	int		m_NowControlState;
	float	m_InputWaitTime;
};

#endif	// #ifndef GRAPHIC_CONFIG_WIDGET_H.