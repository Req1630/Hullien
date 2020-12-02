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
	// 選択状態.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,	// 項目選択状態.
		ESelectState_FullSC,	// フルスクリーン選択.

		ESelectState_Max,
	} typedef ESelectState;

	enum enControlllerState
	{
		EControlllerState_None,

		EControlllerState_FullSCOn,		// フルスクリーンフルスクリーンOn.
		EControlllerState_FullSCOff,	// フルスクリーンフルスクリーンOff.

		EControlllerState_Max,
	} typedef EControlllerState;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Control,	// 操作.
		ESpriteNo_Choise,	// 選択.
		ESpriteNo_On,		// On.
		ESpriteNo_Off,		// Off.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CGraphicConfigWidget();
	virtual ~CGraphicConfigWidget();

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
	void FullScChoiceUpdate();
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// 画像.
	std::unique_ptr<CCursor>				m_pCursor;
	bool	m_IsFullScreen;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	int		m_NowControlState;
	float	m_InputWaitTime;
};

#endif	// #ifndef GRAPHIC_CONFIG_WIDGET_H.