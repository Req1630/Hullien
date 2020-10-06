#ifndef GAMECLEAR_EVENT_H
#define GAMECLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// 地面クラス.
class CSpawnUFO;				// イベント用UFOクラス.
class CEventPlayer;				// イベント用プレイヤークラス.
class CEventGirl;				// イベント用女の子クラス.
class CEventAlienA;				// イベント用宇宙人Aクラス.
class CEventCamera;				// イベント用カメラクラス.
class CEventManager;			// イベント管理クラス.

/************************************
*	クリアイベントクラス.
**/
class CGameClearEvent : public CEventBase
{
public:
	CGameClearEvent();
	virtual ~CGameClearEvent();

	// 読み込み関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// スプライト描画関数.
	virtual void SpriteRender() override;

private:
	// カメラ初期化関数.
	bool CameraInit();
	// UFO初期化関数.
	bool SpawnUFOInit();
	// プレイヤー初期化関数.
	bool PlayerInit();
	// 女の子初期化関数.
	bool GirlInit();
	// 宇宙人初期化関数.
	bool AlienInit();
	// アクタの更新関数.
	void ActorUpdate();
	// カメラの更新関数.
	void CameraUpdate();
	// シーンの設定.
	void SceneSetting();

	// 次のシーンに進める.
	virtual void NextStep() override;
	// スキップ.
	virtual void Skip() override;

	// 以下デバッグ用.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>			m_pPlayer;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventAlienA>			m_pAlienA;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventManager>			m_pEventManager;
	CEventCharacter::SOptionalState			m_stPlayer;			//プレイヤーの情報.
	CEventCharacter::SOptionalState			m_stGirl;			//女の子の情報.
	CEventCharacter::SOptionalState			m_stAlien;			//宇宙人の情報.
	CEventCamera::SCameraState				m_stCamera;			//カメラの情報.
	
	D3DXVECTOR3								m_vUFOPosition;		// UFOの位置.
	D3DXVECTOR3								m_vUFOScale;		// UFOの大きさ.
	D3DXVECTOR3								m_vAlienOldPosition;// 宇宙人の前座標.
	int										m_WaitCount;		// 待機カウント.
	int										m_SwingCameraCount;	// カメラを揺らすカウント.
	float									m_Gravity;			// 重力.

	float									m_Speed;
	int										m_NowScene;			// 現在のシーン/
	int										m_UFOScale;			// 現在のシーン/

};


#endif	//#ifndef GAMECLEAR_EVENT_H