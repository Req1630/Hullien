#ifndef BUTTON_EXPLANATION_H
#define BUTTON_EXPLANATION_H

#include "..\Widget.h"

#include <vector>

class CButtonExp : public CWidget
{
	const char* SPRITE_L_STICK_NAME			= "stickL";			// Lボタンの画像.
	const char* SPRITE_SELECT_NAME			= "move";			// selectの画像.
	const char* SPRITE_B_BUTTON_NAME		= "buttonB";		// Bボタンの画像.
	const char* SPRITE_DETERMINATION_NAME	= "determination";	// 決定画像.
	const char* SPRITE_A_BUTTON_NAME		= "buttonA";		// Aボタンの画像.
	const char* SPRITE_CANCEL_NAME			= "cancel";			// キャンセル画像.
	const char* SPRITE_BACK_NAME			= "back";			// キャンセル画像.

	const float DEFAULT_SPRITE_RENDER_POS_X	= 950.0f;
	const float DEFAULT_SPRITE_RENDER_POS_Y	= 680.0f;

	const float L_STICK_SPRITE_RENDER_POS_X	= -70.0f;
	const float SELECT_SPRITE_RENDER_POS_X	= -40.0f;
	const float B_SPRITE_RENDER_POS_X		= 50.0f;
	const float DET_SPRITE_RENDER_POS_X		= 80.0f;
	const float A_SPRITE_RENDER_POS_X		= 170.0f;
	const float CANCEL_SPRITE_RENDER_POS_X	= 200.0f;

	const float SPRITE_RENDER_WIDTH	= A_SPRITE_RENDER_POS_X - B_SPRITE_RENDER_POS_X;

	const float SPRITE_RENDER_POS_X;
	const float SPRITE_RENDER_POS_Y;

	const float BUTTON_SPRITE_SIZE	= 0.4f;
	const float CHAR_SPRITE_SIZE	= 0.7f;

	// スプライト番号.
	enum enSpriteNo : char
	{
		ESpriteNo_None = -1,

		ESpriteNo_LStick,			// Lスティック.
		ESpriteNo_Select,			// 選択.
		ESpriteNo_B,				// Bボタン.
		ESpriteNo_Determination,	// 決定.
		ESpriteNo_A,				// Aボタン.
		ESpriteNo_Cancel,			// キャンセル.

		ESpriteNo_Max,

		ESpriteNo_Begin = ESpriteNo_LStick,	// 開始.
		ESpriteNo_End = ESpriteNo_Max,	// 終了.

	} typedef ESpriteNo;

public:
	CButtonExp();
	CButtonExp( const float& weight, const float& height );
	virtual~CButtonExp();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// キャンセルの文字を戻るにするかどうか.
	inline void SetBackString(){ m_IsBackString = true; }

	// キャンセル・戻るの文字を描画するかどうか.
	inline void SetIsCancelRender( const bool& isRender ){ m_IsCancelStringRender = isRender; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::vector<D3DXVECTOR3>				m_PositionList;
	std::vector<float>						m_SpriteSizeList;
	bool									m_IsBackString;
	bool									m_IsCancelStringRender;
};

#endif	// #ifndef BUTTON_EXPLANATION_H.
