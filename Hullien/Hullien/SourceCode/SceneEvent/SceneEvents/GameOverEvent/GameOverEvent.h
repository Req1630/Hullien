#ifndef GAMEOVER_EVENT_H
#define GAMEOVER_EVENT_H

#include "..\..\EventBase\EventBase.h"

class CGroundStage;				// 地面クラス.
class CSpawnUFO;				// イベント用UFOクラス.
class CEventGirl;				// イベント用女の子クラス.
class CEventCamera;				// イベント用カメラクラス.
class CEventManager;			// イベント管理クラス.
class CEventWidget;				// イベントのUIクラス.

/************************************
*	ゲームオーバーイベントクラス.
**/
class CGameOverEvent : public CEventBase
{
public:
	CGameOverEvent();
	virtual ~CGameOverEvent();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// スプライト描画関数.
	virtual void SpriteRender() override;

private:
	// 次のシーンに進める.
	virtual void NextStep() override;
	// スキップ.
	virtual void Skip() override;

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventManager>			m_pEventManager;
	std::shared_ptr<CEventWidget>			m_pEventWidget;
	CEventCharacter::SOptionalState			m_stGirl;			//女の子の情報.
	CEventCamera::SCameraState				m_stCamera;			//カメラの情報.
	D3DXVECTOR3								m_vUFOPosition;		// UFOの位置.
	D3DXVECTOR3								m_vUFOScale;		// UFOの大きさ.

};

#endif	//#ifndef GAMEOVER_EVENT_H.