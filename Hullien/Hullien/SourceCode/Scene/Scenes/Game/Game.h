#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGameActorManager;	// ゲームオブジェクト管理クラス.
class CGameWidgetManager;	// UI管理クラス.
class CContinueWidget;		// コンテニューUIクラス.
class CSkyDome;					// 背景クラス.
class CEventManager;			//イベント管理クラス.
class CPeraRenderer;	// G-Buufer描画用.

/**********************************
*	ゲームシーンクラス.
*/
class CGame : public CSceneBase
{
private:
	// シーンの状態.
	enum class enEventSceneState
	{
		None,

		GameStart,			// ゲームスタート.
		Game,				// ゲーム中.
		GameOver_Player,	// プレイヤーが死んだ場合.
		GameOver_Girl,		// 女の子が連れ去られた場合.
		Continue,			// コンテニュー.
		Clear,				// クリア.

		Max,
	}typedef EEventSceneState;

	// シーン切り替え状態.
	enum class enNextSceneState
	{
		None,

		Game,			//ゲーム.
		Clear,			//クリア.
		GameOver,		//ゲームオーバー.
	}typedef ENextSceneState;

public:
	CGame(CSceneManager* pSceneManager);
	virtual ~CGame();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// モデルの描画.
	void ModelRender();
	// フェード初期化関数.
	bool InitFade();
	// ゲーム処理関数.
	void UpdateGame();
	// コンテニュー処理関数.
	void UpdateContinue();
	// シーン切り替え関数.
	void ChangeEventScene();
	// 次のシーンに移行.
	void NextSceneMove();

private:
	std::unique_ptr<CGameActorManager>		m_GameObjManager;	// ゲームオブジェクト管理クラス.
	std::unique_ptr<CGameWidgetManager>		m_WidgetManager;	// ゲームUI管理クラス.
	std::unique_ptr<CContinueWidget>		m_ContinueWidget;	// コンテニューUIクラス.
	std::unique_ptr<CSkyDome>				m_pSkyDome;			// 背景.
	std::unique_ptr<CEventManager>			m_pEventManager;	// イベント管理クラス.
	std::unique_ptr<CPeraRenderer>			m_pPeraRenderer;
	EEventSceneState						m_NowEventScene;	// 現在のイベントシーン.
	ENextSceneState							m_NextSceneState;	// 次のシーン状態.
	bool									m_IsChangeScene;	// シーン切り替えが可能か.

};

#endif	// #ifndef GAME_H.