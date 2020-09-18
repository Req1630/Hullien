#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"

class CGroundStage;	// 地面クラス.
class CSpawnUFO;		// イベント用UFOクラス.
class CEventPlayer;		// イベント用プレイヤークラス.
class CGirl;					// 女の子クラス.
class CAlienA;				// 宇宙人Aクラス.
class CEventCamera;	// イベント用カメラクラス.
class CEventManager;	// イベント管理クラス.

/***********************************
*	スタートイベントクラス.
**/
class CGameStartEvent : public CEventBase
{
public:
	CGameStartEvent();
	virtual ~CGameStartEvent();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>		m_pGroundStage;
	std::shared_ptr<CSpawnUFO>		m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>		m_pPlayer;
	std::shared_ptr<CGirl>					m_pGirl;
	std::shared_ptr<CAlienA>				m_pAlienA;
	std::shared_ptr<CEventCamera>		m_pEventCamera;
	std::shared_ptr<CEventManager>	m_pEventManager;
	D3DXVECTOR3							m_vPosition;
	D3DXVECTOR3							m_vRotation;
	D3DXVECTOR3							m_vLookPosition;
	D3DXVECTOR3							m_vUFOPosition;
	int												m_NowStep;
	float											m_Speed;
};

#endif //#ifndef START_EVENT_H.