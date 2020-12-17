#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CConfigCursor;
class CVolumeConfigWidget;		// 音量設定UIクラス.
class CCameraConfigWidget;		// カメラ設定UIクラス.
class CControllerConfigWidget;	// コントローラー設定UIクラス.
class CGraphicConfigWidget;		// グラフィック設定UIクラス.

/********************************************
*	設定UIクラス.
**/
class CConfigWidget : public CSceneWidget
{

	const char* SPRITE_BACK			= "ConfigBack";
	const char* SPRITE_VOLUME		= "Volume";
	const char* SPRITE_CAMERA		= "Camera";
	const char* SPRITE_CONTROLLER	= "Controller";
	const char* SPRITE_GRAPHIC		= "Graphic";
	const char* SPRITE_RETURN_TITLE	= "BackTitle";

	const int BACKGROUND	= 0;	//背景配列番号.
	const int VOLUME		= 1;	//開始配列番号.
	const int CAMERA		= 2;	//開始配列番号.
	const int CONTROLLER	= 3;	// 
	const int GRAPHIC		= 4;	// 

	enum enConfigState
	{
		EConfigState_None,

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

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 音量の設定をできないようにする.
	void OffVolumeSeting();

	// 設定を終了したか.
	bool IsEndConfig();

	// タイトルに戻るか.
	bool IsReturnToTitle();

private:
	// カーソルの設定.
	void CursorSetting();
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::unique_ptr<CConfigCursor>			m_pCursor;				// カーソルクラス.
	std::unique_ptr<CVolumeConfigWidget>	m_pVolumeConfig;		// 音量設定UI.
	std::unique_ptr<CCameraConfigWidget>	m_pCameraConfig;		// カメラ設定UI.
	std::unique_ptr<CControllerConfigWidget>m_pControllerConfig;	// コントローラー設定UI.
	std::unique_ptr<CGraphicConfigWidget>	m_pGraphicConfig;		// グラフィック設定UI.
	D3DXVECTOR3	m_ReturnTitlePosition;	// タイトルへ戻る画像の座標.
	int			m_SelectState;			// 選択状態.
	int			m_OldSelectState;
	int			m_NowConfigState;		// 現在の設定の状態.
	bool		m_IsNowGameScene;		// ゲームシーンかどうか.
	bool		m_IsReturnToTitle;		// タイトルに戻る.
};

#endif	// #ifndef CONFIG_WIDGET_H.