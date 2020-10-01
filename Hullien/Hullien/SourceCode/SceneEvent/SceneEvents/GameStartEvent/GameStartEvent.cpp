#include "GameStartEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include ".\..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventGirl\EventGirl.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventAlien\EventAlien_A\EventAlien_A.h"
#include "..\..\..\GameObject\Actor\Barrier\Barrier.h"
#include "..\..\..\GameObject\MotherShipUFO\MotherShipUFO.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\EventManager\EventManager.h"

#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Common\DebugText\DebugText.h"

/***********************************
*	スタートイベントクラス.
**/
CGameStartEvent::CGameStartEvent()
	: m_pGroundStage		( nullptr )
	, m_pSpawnUFO			( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGirl				( nullptr )
	, m_pAlienA				( nullptr )
	, m_pBarrier			( nullptr )
	, m_pMotherShipUFO		( nullptr )
	, m_pEventCamera		( nullptr )
	, m_pEventManager		( nullptr )
	, m_vCameraPosition		( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ))
	, m_vCameraRotation		( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ))
	, m_vLookPosition		( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ))
	, m_vUFOPosition		( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ))
	, m_EventStep			( EEventStep::EventStart )
	, m_NowStep				( 0 )
	, m_Speed				( 0.0f )
	, m_DecelerationZ		( 0.0f )
	, m_Count				( 0.0f )
	, m_IsDisp				( false )
	, m_stPlayer			()
	, m_stGirl				()
	, m_stAlien				()
{
	m_pGroundStage		= std::make_shared<CGroundStage>();
	m_pSpawnUFO			= std::make_shared<CSpawnUFO>();
	m_pPlayer			= std::make_shared<CEventPlayer>();
	m_pGirl				= std::make_shared<CEventGirl>();
	m_pAlienA			= std::make_shared<CEventAlienA>();
	m_pBarrier			= std::make_shared<CBarrier>();
	m_pMotherShipUFO	= std::make_shared<CMotherShipUFO>();
	m_pEventCamera		= std::make_shared<CEventCamera>();
}

CGameStartEvent::~CGameStartEvent()
{
}

// 読込関数.
bool CGameStartEvent::Load()
{
	if( m_pGroundStage->Init() == false )	return false;	// 地面の初期化.
	if( SpawnUFOInit() == false )			return false;	// UFOの初期化.
	if( PlayerInit() == false )				return false;	// プレイヤーの初期化.
	if( GirlInit() == false )				return false;	// 女の子の初期化.
	if( AlienInit() == false )				return false;	// 宇宙人Aの初期化.
	if( MotherShipUFOInit() == false )		return false;	// マザーシップの初期化.
	if( CameraInit() == false )				return false;	// カメラの初期化.

	m_IsEventEnd = false;
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;

	return true;
}

// 更新関数.
void CGameStartEvent::Update()
{
	DebugOperation();

	// シーンの設定.
	SceneSetting();
	// アクタの更新.
	ActorUpdate();
	// カメラの更新.
	CameraUpdate();

	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_EventStep = EEventStep::GameStart;
	}
}

// 描画関数.
void CGameStartEvent::Render()
{
	m_pGroundStage->Render();	// ステージの描画.
	m_pSpawnUFO->Render();		// UFOの描画.
	m_pPlayer->Render();		// プレイヤーの描画.
	m_pGirl->Render();			// 女の子の描画.
	m_pAlienA->Render();		// 宇宙Aの描画.
	m_pBarrier->Render();
	m_pMotherShipUFO->Render();

	CFade::Render();

	DebugRender();
}

// カメラ初期化関数.
bool CGameStartEvent::CameraInit()
{
	m_vCameraPosition = { 23.0f , 7.5f, 20.0f };
	m_vLookPosition = m_pPlayer->GetPosition();
	m_vLookPosition.y = m_pPlayer->GetPosition().y + 3.0f;
	return true;
}

// UFO初期化関数.
bool CGameStartEvent::SpawnUFOInit()
{
	if (m_pSpawnUFO->Init() == false) return false;	
	m_vUFOPosition = { 0.0f, 12.0f, 120.0f };

	return true;
}

// プレイヤー初期化関数.
bool CGameStartEvent::PlayerInit()
{
	if (m_pPlayer->Init() == false) return false;	
	m_stPlayer.vPosition.z = 60.0f;
	return true;
}

// 女の子初期化関数.
bool CGameStartEvent::GirlInit()
{
	if (m_pGirl->Init() == false) return false;
	m_stGirl.vPosition.z = 64.5f;
	return true;
}

// 宇宙人初期化関数.
bool CGameStartEvent::AlienInit()
{
	if (m_pAlienA->Init() == false) return false;
	m_stAlien.IsDisp = false;
	return true;
}

// マザーシップ初期化関数.
bool CGameStartEvent::MotherShipUFOInit()
{
	if (m_pMotherShipUFO->Init() == false) return false;
	m_pMotherShipUFO->SetPosition({ 0.0f, 20.0f, 100.0f });
	return true;
}

// アクタの更新関数.
void CGameStartEvent::ActorUpdate()
{
	// プレイヤー.
	m_pPlayer->SetOptionalState( m_stPlayer );
	// 女の子.
	m_pGirl->SetOptionalState( m_stGirl );
	// 宇宙人.
	m_pAlienA->SetOptionalState( m_stAlien );
	// UFOの位置設定.
	m_pSpawnUFO->SetPosition( m_vUFOPosition );
}

// カメラの更新関数.
void CGameStartEvent::CameraUpdate()
{
	m_pEventCamera->SetPosition(m_vCameraPosition);
	m_pEventCamera->SetLookPosition(m_vLookPosition);
	CCameraManager::SetCamera(m_pEventCamera);
}

// シーンの設定.
void CGameStartEvent::SceneSetting()
{
	switch (m_EventStep)
	{
	case EEventStep::Escape_PlayerAndGirl:	//逃げるプレイヤーと女の子.
		EscapePlayerAndGirl();
		break;
	case EEventStep::Viewpoint_UFO:	//2人を追うUFOの視点.
		ViewpointUFO();
		break;
	case EEventStep::Move_UFO:	//UFO定位置につく.
		MoveUFO();
		break;
	case EEventStep::Stop_UFO: // UFO停止.
		StopUFO();
		break;
	case EEventStep::Appearance_Alien: //宇宙人登場.
		AppearanceAlien();
		break;
	case EEventStep::Move_Alien: // 宇宙人ダッシュ.
		MoveAlien();
		break;
	case EEventStep::GetCaught_Girl: // 女の子捕まる.
		GetCaughtGirl();
		break;
	case EEventStep::InvocatingOrder_Barrier:	//バリア発動指示.
		InvocatingOrderBarrier();
		break;
	case EEventStep::Invocating_Barrier:	//バリア発動.
		InvocatingBarrier();
		break;
	case EEventStep::Return_Girl:	// 女の子帰還.
		ReturnGirl();
		break;
	case EEventStep::GameStart:	//ゲームシーンへ.
		GameStart();
		break;
	default:
		break;
	}
}

// 次のシーンに進める.
void CGameStartEvent::NextStep()
{
	m_NowStep++;
	m_EventStep = static_cast<EEventStep>(m_NowStep);
}

// 逃げるプレイヤーと女の子.
void CGameStartEvent::EscapePlayerAndGirl()
{
	m_vCameraPosition.z = m_stPlayer.vPosition.z;
	m_vLookPosition = m_stPlayer.vPosition;
	m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

	// 次のステップへ.
	if (m_stPlayer.vPosition.z <= 40.0f) NextStep();
}

// UFO視点.
void CGameStartEvent::ViewpointUFO()
{
	// カメラの設定.
	m_vCameraPosition = m_vUFOPosition;
	m_vCameraPosition.y = m_vUFOPosition.y + 5.0f;
	m_vCameraPosition.z = m_vUFOPosition.z - 5.0f;
	m_vLookPosition.z = m_pPlayer->GetPosition().z - 1.5f;
	// プレイヤーと女の子の移動.
	if (m_stPlayer.vPosition.z > 0.0f)
	{
		m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
		m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;
	}

	// UFO移動.
	m_vUFOPosition.z -= 0.3f;

	// 次のステップへ.
	if (m_pSpawnUFO->GetPosition().z <= 40.0f)	 NextStep();
}

// UFO移動.
void CGameStartEvent::MoveUFO()
{
	// UFO移動.
	if (m_pSpawnUFO->GetPosition().z < -10.0f)
	{
		m_vUFOPosition.z = m_pSpawnUFO->GetPosition().z - (0.3f + m_DecelerationZ);
		m_DecelerationZ -= 0.0046f;
	}
	else
	{
		m_vUFOPosition.z = m_pSpawnUFO->GetPosition().z - 0.3f;
	}

	// カメラ位置.
	m_vCameraPosition = D3DXVECTOR3(3.5f, 7.0f, 10.5f);
	// カメラの視点移動.
	m_vLookPosition = m_pSpawnUFO->GetPosition();
	// プレイヤーの位置設定.
	m_stPlayer.vPosition.z = 0.0f;
	// 女の子の位置設定.
	m_stGirl.vPosition.z = 4.5f;

	// 次のステップ.
	if (m_pSpawnUFO->GetPosition().z < -10.0f)
	{
		m_DecelerationZ = 0.0f;
		NextStep();
	}
}

// UFO停止.
void CGameStartEvent::StopUFO()
{
	m_Count++;
	if (m_Count < 100)
	{
		m_vUFOPosition.y = m_vUFOPosition.y + static_cast<float>(sin(D3DX_PI * 2.0f / 100.0f * m_Count) * 0.1f);
	}
	else
	{
		if (m_vUFOPosition.y >= 11.5f) m_vUFOPosition.y -= 0.01f;
	}

	if (m_vUFOPosition.y < 11.5f && m_Count >= 200)
	{
		NextStep();
		m_stAlien.vPosition = m_pSpawnUFO->GetPosition();
		m_Count = 0;
	}
}

// 宇宙人登場.
void CGameStartEvent::AppearanceAlien()
{
	m_vCameraPosition = D3DXVECTOR3(0.0f, 8.5f, -1.0f);
	m_vLookPosition.y = m_pAlienA->GetPosition().y + 1.0f;

	m_stAlien.IsDisp = true;

	if (m_stAlien.vPosition.y >= 7.0f)
	{
		m_stAlien.vPosition.y -= 0.05f;
	}
	else
	{
		NextStep();
	}
}

// 宇宙人移動.
void CGameStartEvent::MoveAlien()
{
	m_Count++;
	m_stAlien.vPosition.y = m_pAlienA->GetPosition().y + static_cast<float>(sin(D3DX_PI * 2.0f / 90.0f * m_Count) * 0.01f);
	if (m_Count > 200)
	{
		m_stAlien.vPosition.z += 0.5f;
	}

	if (m_stAlien.vPosition.z >= 0.0f)
	{
		m_Count = 0;
		NextStep();
	}
}

// 女の子捕まる.
void CGameStartEvent::GetCaughtGirl()
{
	m_vCameraPosition = { -20.0f, 5.0f, -10.0f };
	m_vLookPosition = m_pPlayer->GetPosition();

	m_stAlien.vPosition.y = 5.0f;
	m_stAlien.vPosition.z += 0.3f;

	if (m_stAlien.vPosition.z > 50.0f)
	{
		NextStep();
		m_vCameraPosition = { -6.5f, 8.4f, -10.0f };
		m_vLookPosition = m_pGirl->GetPosition();
	}

	m_pAlienA->Collision(m_pGirl.get());
	m_pGirl->Collision(m_pAlienA.get());
	m_stGirl.vPosition = m_pGirl->GetPosition();
}

// バリア発動準備.
void CGameStartEvent::InvocatingOrderBarrier()
{
	m_pPlayer->SetRotationY(static_cast<float>(D3DXToRadian(180)));
	m_pPlayer->Update();

	if (m_pPlayer->IsSpecialAbility() == true)
	{
		m_pBarrier->Init();	// バリアの初期化.
		NextStep();

		m_Count = 100;
	}
}

// バリア発動.
void CGameStartEvent::InvocatingBarrier()
{
	m_vLookPosition.x = m_vLookPosition.x + static_cast<float>(sin(D3DX_PI * 2.0f / 10.0f * m_Count) * (m_Count * 0.01f));
	m_vLookPosition.y = m_vLookPosition.y + static_cast<float>(sin(D3DX_PI * 2.0f / 10.0f * m_Count) * (m_Count * 0.01f));
	if (m_Count != 0) {
		m_Count--;
	}

	m_pBarrier->SetTargetPos(*m_pGirl.get());
	m_pBarrier->Update();
	m_pAlienA->Collision(m_pBarrier.get());

	m_stAlien.vPosition = m_pAlienA->GetPosition();
	if (m_pAlienA->IsBarrierHit() == true) { m_stAlien.vPosition.x -= 1.0f; }
	m_pAlienA->SetPosition(m_stAlien.vPosition);

	if (m_pBarrier->IsActive() == false)
	{
		NextStep();
	}
}

// 女の子帰還.
void CGameStartEvent::ReturnGirl()
{
	m_vUFOPosition.z = -100.0f;

	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

	if (m_stGirl.vPosition.z >= 20.0f) return;
	m_vLookPosition = m_stPlayer.vPosition;

	if (m_vCameraPosition.x < 0.0f) { m_vCameraPosition.x += 0.1f; }
	else { m_vCameraPosition.x = 0.0f; }

	if (m_vCameraPosition.y < 12.0f) { m_vCameraPosition.y += 0.2f; }
	else { m_vCameraPosition.y = 12.0f; }

	if (m_vCameraPosition.z < 30.0f) { m_vCameraPosition.z += 1.0f; }
	else { m_vCameraPosition.z = 30.0f; }

	if (m_stGirl.vPosition.z <= 0.0f)  NextStep();
}

// ゲーム開始.
void CGameStartEvent::GameStart()
{
	m_IsEventEnd = true;
}

void CGameStartEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameStartEvent -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 2.0f ,0.0f });
	CDebugText::Render("- CameraPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 3.0f ,0.0f });
	CDebugText::Render("Pos_x", m_vCameraPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 4.0f ,0.0f });
	CDebugText::Render("Pos_y", m_vCameraPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 5.0f ,0.0f });
	CDebugText::Render("Pos_z", m_vCameraPosition.z);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 7.0f ,0.0f });
	CDebugText::Render("- LookPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 8.0f ,0.0f });
	CDebugText::Render("Pos_x", m_vLookPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 9.0f ,0.0f });
	CDebugText::Render("Pos_y", m_vLookPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 10.0f ,0.0f });
	CDebugText::Render("Pos_z", m_vLookPosition.z);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 12.0f ,0.0f });
	CDebugText::Render("- ViewingAngle -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 13.0f ,0.0f });
	CDebugText::Render("ViewinfAngle :", m_pEventCamera->GetViewingAngle());

}

void CGameStartEvent::DebugOperation()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_vCameraPosition.z += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_vCameraPosition.z -= m_Speed;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_vCameraPosition.y += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_vCameraPosition.y -= m_Speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_vCameraPosition.x += m_Speed;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_vCameraPosition.x -= m_Speed;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_vLookPosition.y += m_Speed;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_vLookPosition.y -= m_Speed;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_vLookPosition.z -= m_Speed;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_vLookPosition.z += m_Speed;
	}
}

