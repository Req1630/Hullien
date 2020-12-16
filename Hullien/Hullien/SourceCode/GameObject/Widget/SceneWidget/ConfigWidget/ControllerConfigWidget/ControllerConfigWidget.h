#ifndef CONTROLLER_CONFIG_WIDGET_H
#define CONTROLLER_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include "..\..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include <vector>

class CCursor;	// �J�[�\���N���X.
class CSwitch;	// �X�C�b�`�N���X.

class CControllerConfigWidget : public CWidget
{
	const char* SPRITE_CONTROL_NAME		= "Vibration";
	const char* SPRITE_MARK_NAME		= "ControllerMark";
	const float INPUT_WAIT_TIME_MAX		= 30.0;
	// �I�����.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,	// ���ڑI�����.
		ESelectState_Vibration,	// �o�C�u���[�V�����I��.

		ESelectState_Max,
	} typedef ESelectState;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Control,	// ����.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CControllerConfigWidget();
	virtual ~CControllerConfigWidget();

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
	void VibChoiceUpdate();
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// �摜.
	std::unique_ptr<CCursor>				m_pCursor;
	std::unique_ptr<CSwitch>				m_pSwitch;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	float	m_InputWaitTime;
};

#endif	// #ifndef CONTROLLER_CONFIG_WIDGET_H.