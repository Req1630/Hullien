#ifndef GRAPHIC_CONFIG_WIDGET_H
#define GRAPHIC_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include "..\..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include <vector>

class CConfigCursor;
class CSlinder;
class CSwitch;
class CButtonExp;		// �{�^������UI�N���X.

class CGraphicConfigWidget : public CWidget
{
	const char* SPRITE_FULL_SC_NAME		= "FullSC";
	const char* SPRITE_MARK_NAME		= "GraphicMark";
	const float INPUT_WAIT_TIME_MAX		= 30.0;

	const D3DXVECTOR3 BUTTON_EXP_RENDER_POS = { 900.0f, 650.0f, 0.0f };

	// �I�����.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,	// ���ڑI�����.
		ESelectState_FullSC,	// �t���X�N���[���I��.

		ESelectState_Max,
	} typedef ESelectState;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Control,	// ����.

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
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;		// �摜.
	std::unique_ptr<CConfigCursor>			m_pCursor;
	std::unique_ptr<CSwitch>				m_pSwitch;
	std::unique_ptr<CButtonExp>				m_pButtonExp;	// �{�^������UI.
	bool	m_IsFullScreen;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	float	m_InputWaitTime;
};

#endif	// #ifndef GRAPHIC_CONFIG_WIDGET_H.