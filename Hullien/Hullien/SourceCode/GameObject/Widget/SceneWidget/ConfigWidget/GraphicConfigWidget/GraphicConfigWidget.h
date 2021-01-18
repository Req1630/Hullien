#ifndef GRAPHIC_CONFIG_WIDGET_H
#define GRAPHIC_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include "..\..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include <vector>

class CConfigCursor;
class CSlinder;
class CSwitch;
class CButtonExp;		// ボタン説明UIクラス.

class CGraphicConfigWidget : public CWidget
{
	const char* SPRITE_FULL_SC_NAME		= "FullSC";
	const char* SPRITE_MARK_NAME		= "GraphicMark";
	const float INPUT_WAIT_TIME_MAX		= 30.0;

	const D3DXVECTOR3 BUTTON_EXP_RENDER_POS = { 900.0f, 650.0f, 0.0f };

	// 選択状態.
	enum enSelectState
	{
		ESelectState_None,

		ESelectState_Select,	// 項目選択状態.
		ESelectState_FullSC,	// フルスクリーン選択.

		ESelectState_Max,
	} typedef ESelectState;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Control,	// 操作.

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
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;		// 画像.
	std::unique_ptr<CConfigCursor>			m_pCursor;
	std::unique_ptr<CSwitch>				m_pSwitch;
	std::unique_ptr<CButtonExp>				m_pButtonExp;	// ボタン説明UI.
	bool	m_IsFullScreen;
	int		m_NowSelectState;
	int		m_OldNowSelectState;
	int		m_NowConfigState;
	float	m_InputWaitTime;
};

#endif	// #ifndef GRAPHIC_CONFIG_WIDGET_H.