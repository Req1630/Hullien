#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CConfigCursor;
class CVolumeConfigWidget;		// 音量設定UIクラス.
class CCameraConfigWidget;		// カメラ設定UIクラス.
class CControllerConfigWidget;	// コントローラー設定UIクラス.
class CGraphicConfigWidget;		// グラフィック設定UIクラス.
class CBlendSprite;				// ブレンド描画用クラス.
class CTransition;				// トランジションクラス.
class CButtonExp;				// ボタン説明UIクラス.

/********************************************
*	設定UIクラス.
**/
class CConfigWidget
{
	const char* SPRITE_BACK			= "ConfigBack";
	const char* SPRITE_ICON			= "ConfigIcon";
	const char* SPRITE_VOLUME		= "Volume";
	const char* SPRITE_CAMERA		= "Camera";
	const char* SPRITE_CONTROLLER	= "Controller";
	const char* SPRITE_GRAPHIC		= "Graphic";
	const char* SPRITE_RETURN_TITLE	= "BackTitle";

	const int BACKGROUND	= 0;	//背景配列番号.
	const int VOLUME		= 2;	//開始配列番号.
	const int CAMERA		= 3;	//開始配列番号.
	const int CONTROLLER	= 4;	// 
	const int GRAPHIC		= 5;	// 

	const float FADE_VALUE_MAX = 1.0f;
	const float FADE_VALUE_MIN = 0.0f;
	const float FADE_SPEED = 0.03f;

	const float INPUT_WAIT_TIME_MAX = 30.0f;

	const D3DXVECTOR3 BUTTON_EXP_RENDER_POS = { 900.0f, 650.0f, 0.0f };

	enum enConfigState
	{
		EConfigState_None = 1,

		EConfigState_Volume,	// 音量の設定.
		EConfigState_Camera,	// カメラの操作.
		EConfigState_Controller,// コントローラーの設定.
		EConfigState_Graphic,	// グラフィックの設定.
		EConfigState_BackTitle,	// タイトルへ戻る.

		EConfigState_End,

		EConfigState_Max,
	} typedef EConfigState;

public:
	CConfigWidget();
	CConfigWidget( const bool& isGame );
	virtual ~CConfigWidget();

	// インスタンスの生成.
	static CConfigWidget* GetInstance()
	{
		static std::unique_ptr<CConfigWidget> pInstance = std::make_unique<CConfigWidget>();
		return pInstance.get();
	}

	// 初期化関数.
	static bool Init();
	// 更新関数.
	static void Update();
	// 描画関数.
	static void Render();

	// 音量の設定をできないようにする.
	static void OffVolumeSeting();

	// 設定を終了したか.
	static bool IsEndConfig();

	// タイトルに戻るか.
	static bool IsReturnToTitle();

	// ゲームシーンかどうか設定.
	static void SetIsNowGameScene( const bool& isNowGameScene ){ GetInstance()->m_IsNowGameScene = isNowGameScene; }

private:
	// カーソルの設定.
	void CursorSetting();
	// スプライト設定関数.
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::unique_ptr<CConfigCursor>			m_pCursor;				// カーソルクラス.
	std::unique_ptr<CVolumeConfigWidget>	m_pVolumeConfig;		// 音量設定UI.
	std::unique_ptr<CCameraConfigWidget>	m_pCameraConfig;		// カメラ設定UI.
	std::unique_ptr<CControllerConfigWidget>m_pControllerConfig;	// コントローラー設定UI.
	std::unique_ptr<CButtonExp>				m_pButtonExp;			// ボタン説明UI.
	std::unique_ptr<CGraphicConfigWidget>	m_pGraphicConfig;		// グラフィック設定UI.
	std::unique_ptr<CBlendSprite>			m_pBlendSprite;			// ブレンド描画用.
	std::unique_ptr<CTransition>			m_pTransition;			// トランジション.
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3	m_ReturnTitlePosition;	// タイトルへ戻る画像の座標.
	int			m_SelectState;			// 選択状態.
	int			m_OldSelectState;
	int			m_NowConfigState;		// 現在の設定の状態.
	float		m_InputWaitTime;
	float		m_FadeValue;
	bool		m_IsNowGameScene;		// ゲームシーンかどうか.
	bool		m_IsReturnToTitle;		// タイトルに戻る.
	bool		m_IsLoadEnd;
};

#endif	// #ifndef CONFIG_WIDGET_H.