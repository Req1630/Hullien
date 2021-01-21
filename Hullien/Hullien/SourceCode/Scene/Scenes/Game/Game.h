#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGameActorManager;	// ゲームオブジェクト管理クラス.
class CGameWidgetManager;	// UI管理クラス.
class CContinueWidget;		// コンテニューUIクラス.
class CConfigWidget;		// 設定UIクラス.
class CSkyDome;				// 背景クラス.
class CEventManager;		// イベント管理クラス.
class CTutorial;			// チュートリアルUIクラス.

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

		Title,
	}typedef ENextSceneState;

public:
	CGame( CSceneManager* pSceneManager );
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
	// ゲーム処理関数.
	void GameUpdate();
	// コンテニュー処理関数.
	void ContinueUpdate();
	// 設定画面の更新.
	void ConfigUpdate();
	// シーン切り替え関数.
	void ChangeEventScene();
	// 次のシーンに移行.
	void NextSceneMove();
	// 次のシーンを設定.
	void SetNextScene(EEventSceneState state, bool GameOver = false);
	// BGMの停止.
	void StopBGM(const char* name);
	// 全てのBGM停止.
	void StopAllBGM();

private:
	std::unique_ptr<CGameActorManager>	m_pGameObjManager;	// ゲームオブジェクト管理クラス.
	std::unique_ptr<CGameWidgetManager>	m_pWidgetManager; 	// ゲームUI管理クラス.
	std::unique_ptr<CContinueWidget>	m_pContinueWidget;	// コンテニューUIクラス.
	std::unique_ptr<CEventManager>		m_pEventManager; 	// イベント管理クラス.
	std::unique_ptr<CTutorial>			m_pTutorial;		// チュートリアルクラス.
	EEventSceneState					m_NowEventScene;	// 現在のイベントシーン.
	ENextSceneState						m_NextSceneState;	// 次のシーン状態.
	float								m_WaitCount;		// 待機カウント.
	bool								m_IsContinueSelect;	// コンテニューを選択しているか.
	bool								m_IsConfig;			// 設定中か.
	bool								m_IsPlayGameBGM;	// ゲームのBGMを再生しているか.
	bool								m_IsPlayDangerBGM;	// 危険BGMを再生しているか.
};

#endif	// #ifndef GAME_H.