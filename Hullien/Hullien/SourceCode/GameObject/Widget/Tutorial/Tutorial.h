#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include "..\Widget.h"

#include <vector>

class CTransition;
class CButtonExp;

class CTutorial : public CWidget
{
	const char* SPRITE_TUTORIAL_ONE_NAME	= "Tutorial1";	// チュートリアル画像.
	const char* SPRITE_TUTORIAL_TWO_NAME	= "Tutorial2";	// チュートリアル画像.
	const char* SPRITE_TUTORIAL_THREE_NAME	= "Tutorial3";	// チュートリアル画像.
	const char* SPRITE_BACKGROUND_NAME		= "Fade";		// 背景画像.

	const char* SPRITE_ARROW_RIGHT			= "arrowRight";			// 矢印画像.
	const char* SPRITE_ARROW_LEFT			= "arrowLeft";			// 矢印画像.
	const char* SPRITE_SELECT_ARROW_RIGHT	= "selectArrowRight";	// 選択状態の矢印画像.
	const char* SPRITE_SELECT_ARROW_LEFT	= "selectArrowLeft";	// 選択状態の矢印画像.

	const float BACK_GROUND_ALPHA_SPEED		= 0.01f;	// 背景のアルファ速度.
	const float BACK_GROUND_ALPHA_MAX		= 0.4f;		// 背景のアルファ最大値.
	const float BACK_GROUND_ALPHA_MIN		= 0.0f;		// 背景のアルファ最小値.
	const float TRANSITION_SPEED			= 0.02f;	// トランジションの速度.
	const float TRANSITION_MAX				= 1.0f;		// トランジションの最大値.
	const float TRANSITION_MIN				= 0.0f;		// トランジションの最小値.

	const float ARROW_RENDER_ADJ_POS_X		= 5.0f;		// 矢印の描画座標調整値.

	const float ARROW_SCALE			= 1.0f;
	const float ARROW_ADJ_SCALE		= 0.5f;
	const float ARROW_SCALE_VALUE	= -0.2f;

	const float	INPUT_WAIT_TIME_MAX		= 30.0f;			// 入力待機フレーム.

	const D3DXVECTOR2 BUTTON_EXP_RENDER_POS = { 650.0f, 600.0f };

	enum enTutorialState : unsigned char
	{
		ETutorialState_None,

		ETutorialState_One = 0,	// 一枚目.
		ETutorialState_Two,		// 二枚目.
		ETutorialState_Three,	// 三枚目.

		ETutorialState_Max,

		ETutorialState_Start	= ETutorialState_One,	// 開始値.
		ETutorialState_End		= ETutorialState_Max,	// 終了値.

	} typedef ETutorialState;

	enum enArrowSpriteNo : char
	{
		EArrowSpriteNo_None = -1,

		EArrowSpriteNo_ArrowRight = 0,
		EArrowSpriteNo_ArrowLeft,

		EArrowSpriteNo_SelectArrowRight,
		EArrowSpriteNo_SelectArrowLeft,

		EArrowSpriteNo_Max,

		EArrowSpriteNo_Start	= EArrowSpriteNo_ArrowRight,		// 開始値.
		EArrowSpriteNo_End		= EArrowSpriteNo_SelectArrowLeft,	// 終了値.

	} typedef EArrowSpriteNo;

	struct stArrowParam
	{
		D3DXVECTOR3 vPos;	// 座標.
		float		Scale;	// 大きさ.

		stArrowParam()
			: vPos	( 0.0f, 0.0f, 0.0f )
			, Scale	( 1.0f )
		{}
	} typedef SArrowParam;

public:
	CTutorial();
	virtual ~CTutorial();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// チュートリアルを開始する.
	inline void SetupTutorial() { m_IsSetup = true; m_IsTutorialEnd = false; }
	// チュートリアルが終了したか.
	inline bool IsTutorialEnd() { return m_IsTutorialEnd; }

private:
	// 矢印の描画.
	void ArrowRender();

	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::vector<std::shared_ptr<CSprite>>	m_pArrowSprites;
	std::vector<SArrowParam>				m_ArrowParams;
	std::unique_ptr<CTransition>			m_pTransition;
	std::unique_ptr<CButtonExp>				m_pButtonExp;
	int		m_NowSpriteNo;
	int		m_SelectMoveNo;
	float	m_InputWaitTime;
	float	m_BackGroundAlpha;
	float	m_TransitionValue;
	bool	m_IsSetup;
	bool	m_IsTutorialEnd;
};

#endif	// #ifndef TUTORIAL_WIDGET_H.