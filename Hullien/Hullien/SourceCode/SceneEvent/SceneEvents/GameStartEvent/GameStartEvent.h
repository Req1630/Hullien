#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"

class CGroundStage;		// 地面クラス.
class CSpawnUFO;		// イベント用UFOクラス.
class CEventPlayer;		// イベント用プレイヤークラス.
class CEventGirl;		// イベント用女の子クラス.
class CEventAlienA;		// イベント用宇宙人Aクラス.
class CBarrier;			// バリアクラス.
class CMotherShipUFO;	// マザーシップクラス.
class CEventCamera;		// イベント用カメラクラス.
class CEventManager;	// イベント管理クラス.

/***********************************
*	スタートイベントクラス.
**/
class CGameStartEvent : public CEventBase
{
	// イベントの各ステップ.
	enum class enEventStep
	{
		Escape_PlayerAndGirl = 0,	// 逃げるプレイヤーと女の子.
		Viewpoint_UFO,				// UFOの視点.
		Move_UFO,					// UFO定位置まで移動.
		Stop_UFO,					// UFO停止.
		Appearance_Alien,			// 宇宙人登場.
		Move_Alien,					// 宇宙人前進.
		GetCaught_Girl,				// 女の子が捕まる.
		InvocatingOrder_Barrier,	// バリア発動準備.
		Invocating_Barrier,			// バリア発動.
		Return_Girl,				// 女の子帰還.
		GameStart,					// ゲーム開始.

		Max,
		EventStart = Escape_PlayerAndGirl,
	} typedef EEventStep;

private:


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
	// マザーシップ初期化関数.
	bool MotherShipUFOInit();
	// アクタの更新関数.
	void ActorUpdate();
	// カメラの更新関数.
	void CameraUpdate();
	// シーンの設定.
	void SceneSetting();
	// 次のシーンに進める.
	void NextStep();

	// 以下イベントのステップ.
	void EscapePlayerAndGirl();
	void ViewpointUFO();
	void MoveUFO();
	void StopUFO();
	void AppearanceAlien();
	void MoveAlien();
	void GetCaughtGirl();
	void InvocatingOrderBarrier();
	void InvocatingBarrier();
	void ReturnGirl();
	void GameStart();

	// 以下デバッグ用.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>		m_pGroundStage;
	std::shared_ptr<CSpawnUFO>			m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>		m_pPlayer;
	std::shared_ptr<CEventGirl>			m_pGirl;
	std::shared_ptr<CEventAlienA>		m_pAlienA;
	std::shared_ptr<CBarrier>			m_pBarrier;
	std::shared_ptr<CMotherShipUFO>		m_pMotherShipUFO;
	std::shared_ptr<CEventCamera>		m_pEventCamera;
	std::shared_ptr<CEventManager>		m_pEventManager;
	D3DXVECTOR3							m_vCameraPosition;
	D3DXVECTOR3							m_vCameraRotation;
	D3DXVECTOR3							m_vLookPosition;
	D3DXVECTOR3							m_vUFOPosition;
	EEventStep							m_EventStep;
	int									m_NowStep;
	float								m_Speed;
	float								m_DecelerationZ;	// z座標減速度.
	float								m_Count;			// カウント.
	bool								m_IsDisp;

	CEventCharacter::SOptionalState		m_stPlayer;
	CEventCharacter::SOptionalState		m_stGirl;
	CEventCharacter::SOptionalState		m_stAlien;
};

#endif //#ifndef START_EVENT_H.