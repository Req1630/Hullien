#ifndef CONTROLLER_CONFIG_WIDGET_H
#define CONTROLLER_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include "..\..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include <vector>

class CCursor;	// カーソルクラス.
class CSwitch;	// スイッチクラス.

class CControllerConfigWidget : public CWidget
{
	const char* SPRITE_CONTROL_NAME		= "Vibration";
	const char* SPRITE_MARK_NAME		= "ControllerMark";
	const float INPUT_WAIT_TIME_MAX		= 30.0;
	// 選択状態.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,	// 項目選択状態.
		ESelectState_Vibration,	// バイブレーション選択.

		ESelectState_Max,
	} typedef ESelectState;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Control,	// 操作.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CControllerConfigWidget();
	virtual ~CControllerConfigWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 終了したか.
	bool IsEnd();

private:
	// 種類の選択.
	void SelectType();
	// 決定.
	void Determination();
	// バイブ選択の更新.
	void VibChoiceUpdate();
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// 画像.
	std::unique_ptr<CCursor>				m_pCursor;
	std::unique_ptr<CSwitch>				m_pSwitch;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	float	m_InputWaitTime;
};

#endif	// #ifndef CONTROLLER_CONFIG_WIDGET_H.