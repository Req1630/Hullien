#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "..\EventBase\EventBase.h"

class CPeraRenderer;	// G-Buufer描画用.

// イベントの種類.
enum class enEventNo
{
	None,

	GameStart,				//ゲームスタート.
	GameClear,				//ゲームクリア.
	GameOver,				//ゲームオーバー.

	Max,

	Start = GameStart,	//初めのシーン.
}typedef EEvent;

/************************************
*	イベントシーン管理クラス.
**/
class CEventManager
{
public:
	CEventManager();
	~CEventManager();

	// 更新関数.
	void Update();
	// 次のイベントに移動.
	void NextEventMove();
	// イベントの終了を取得.
	bool GetIsEventEnd() const { return m_IsEventEnd; };
	// スキップの設定.
	void OnSkip() { m_IsSkip = true; }
	// ゲームオーバーの設定.
	void OnGameOver() { m_IsGameOver = true; }
	// もう一回最初から.
	void EventRetry();

private:
	// モデルの描画.
	void ModelRender();

private:
	std::shared_ptr<CEventBase>			m_pEventBase;	// イベント元クラス.
	std::unique_ptr<CPeraRenderer>		m_pPeraRenderer;
	EEvent										m_NowEvent;		// 現在のイベント.	
	EEvent										m_NextEvent;		// 次のイベント.
	bool											m_IsLoadEnd;		// ロードが終了したか.
	bool											m_IsSkip;			// スキップするか.
	bool											m_IsGameOver;	// ゲームオーバーか.
	bool											m_IsEventEnd;		// イベントが終了したか.
};

#endif	//#ifndef SCENE_EVENT_MANAGER_H
