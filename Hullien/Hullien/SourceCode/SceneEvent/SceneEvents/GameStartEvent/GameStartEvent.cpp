#include "GameStartEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include ".\..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\Character\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\Character\Girl\Girl.h"
#include "..\..\..\GameObject\Actor\Character\Alien\Alien_A\Alien_A.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\EventManager\EventManager.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/***********************************
*	スタートイベントクラス.
**/
CGameStartEvent::CGameStartEvent()
	: m_pGroundStage	( nullptr )
	, m_pSpawnUFO		( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGirl					( nullptr )
	, m_pAlienA				( nullptr )
	, m_pEventCamera	( nullptr )
	, m_pEventManager	( nullptr )
	, m_vPosition			(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vRotation			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vLookPosition	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vUFOPosition		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_NowStep			(0)
	, m_Speed				( false )
{
	m_pGroundStage	= std::make_shared<CGroundStage>();
	m_pSpawnUFO	= std::make_shared<CSpawnUFO>();
	m_pPlayer			= std::make_shared<CEventPlayer>();
	m_pGirl				= std::make_shared<CGirl>();
	m_pAlienA			= std::make_shared<CAlienA>();
	m_pEventCamera	= std::make_shared<CEventCamera>();
}

CGameStartEvent::~CGameStartEvent()
{
}

// 読込関数.
bool CGameStartEvent::Load()
{
	if( m_pGroundStage->Init() == false ) return false;	// 地面の初期化.
	if( m_pSpawnUFO->Init() == false ) return false;	// UFOの初期化.
	if( m_pPlayer->Init() == false ) return false;			// プレイヤーの初期化.
	if( m_pGirl->Init() == false ) return false;				// 女の子の初期化.
	if( m_pAlienA->Init() == false ) return false;			// 宇宙人Aの初期化.

	m_IsEventEnd = false;
	m_vPosition = { 23.0f , 7.5f, 20.0f };
	m_vLookPosition = m_pPlayer->GetPosition();
	m_vLookPosition.y = m_pPlayer->GetPosition().y + 3.0f;
	m_vUFOPosition = { 0.0f, 12.0f, 70.0f };
	m_pPlayer->SetPosition({ 0.0f, 4.0f, 40.0f });
	m_pGirl->SetPosition({ 0.0f, 4.0f, 42.5f });
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;

	return true;
}

// 更新関数.
void CGameStartEvent::Update()
{
	DebugOperation();

	static float y = 0.0f;
	static float z = 0.0f;
	static int count = 0;

	switch (m_NowStep)
	{
	case 0:
		// プレイヤーの位置設定.
		m_pPlayer->SetPosition({
			m_pPlayer->GetPosition().x,
			m_pPlayer->GetPosition().y,
			m_pPlayer->GetPosition().z - 0.1f });
		// 女の子の位置設定.
		m_pGirl->SetPosition({
			m_pGirl->GetPosition().x,
			m_pGirl->GetPosition().y,
			m_pGirl->GetPosition().z - 0.1f });

		m_vPosition.z = m_pPlayer->GetPosition().z;
		m_vLookPosition = m_pPlayer->GetPosition();

		if (m_pPlayer->GetPosition().z <= 20.0f)
		{
			m_NowStep++;
		}
		break;

	case 1:
		if (m_pPlayer->GetPosition().z >= 0.0f)
		{
			// プレイヤーの位置設定.
			m_pPlayer->SetPosition({
				m_pPlayer->GetPosition().x,
				m_pPlayer->GetPosition().y,
				m_pPlayer->GetPosition().z - 0.1f });
			// 女の子の位置設定.
			m_pGirl->SetPosition({
				m_pGirl->GetPosition().x,
				m_pGirl->GetPosition().y,
				m_pGirl->GetPosition().z - 0.1f });

			m_vPosition.z = m_pPlayer->GetPosition().z;
		}

		// UFO移動.
		if (m_pSpawnUFO->GetPosition().z < -10.0f)
		{
			static float speed = 0.0f;
			m_vUFOPosition.z = m_pSpawnUFO->GetPosition().z - (0.3f + speed);
			speed -= 0.0046f;
		}
		else
		{
			m_vUFOPosition.z = m_pSpawnUFO->GetPosition().z - 0.3f;
		}

		// カメラの視点移動.
		if (m_vLookPosition.y < m_pSpawnUFO->GetPosition().y)
		{
			m_vLookPosition.y = m_vLookPosition.y + y;
			y += static_cast<float>(D3DX_PI) * 0.002f;
		}
		else
		{
			m_vLookPosition.y = m_pSpawnUFO->GetPosition().y;
		}
		if (m_vLookPosition.z < m_pSpawnUFO->GetPosition().z)
		{
			m_vLookPosition.z = m_vLookPosition.z + z;
			z += static_cast<float>(D3DX_PI) * 0.0025f;
		}
		else
		{
			m_vLookPosition.z = m_pSpawnUFO->GetPosition().z;
		}

		// カメラ位置.
		if (m_vPosition.x >= 3.5f) m_vPosition.x -= 0.05f;
		if (m_vPosition.y >= 7.0f) m_vPosition.y -= 0.01f;
		if (m_vPosition.z <= 10.5f) m_vPosition.z += 1.0f;

		// 次のステップ.
		if (m_pSpawnUFO->GetPosition().z < -20.0f)
		{
			m_NowStep++;
		}
		break;

	case 2:
		if (count < 100)
		{
			m_vUFOPosition.y = m_vUFOPosition.y + sinf(D3DX_PI * 2.0f / 100.0f * count) * 0.1f;
			count++;
		}
		else
		{
			m_vLookPosition.x = m_vLookPosition.y + sin(D3DX_PI * 2.0f / 2.5f * count) * 2.0f;
			count++;

			if (m_vUFOPosition.y >= 11.5f) m_vUFOPosition.y -= 0.01f;
		}

		if (m_vUFOPosition.y < 11.5f) m_NowStep++;
		break;

	case 3:
		break;
	default:
		break;
	}

	m_pSpawnUFO->SetPosition( m_vUFOPosition );

	m_pEventCamera->SetPosition( m_vPosition );
	m_pEventCamera->SetLookPosition(m_vLookPosition);
	CCameraManager::SetCamera( m_pEventCamera );

	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
//		m_IsEventEnd = true;
	}
}

// 描画関数.
void CGameStartEvent::Render()
{
	m_pGroundStage->Render();	// ステージの描画.
	m_pSpawnUFO->Render();		// UFOの描画.
	m_pPlayer->Render();			// プレイヤーの描画.
	m_pGirl->Render();				// 女の子の描画.
//	m_pAlienA->Render();			// 宇宙Aの描画.
	DebugRender();
}

void CGameStartEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameStartEvent -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 2.0f ,0.0f });
	CDebugText::Render("- CameraPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 3.0f ,0.0f });
	CDebugText::Render("Pos_x", m_vPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 4.0f ,0.0f });
	CDebugText::Render("Pos_y", m_vPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 5.0f ,0.0f });
	CDebugText::Render("Pos_z", m_vPosition.z);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 7.0f ,0.0f });
	CDebugText::Render("- LookPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 8.0f ,0.0f });
	CDebugText::Render("Pos_x", m_vLookPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 9.0f ,0.0f });
	CDebugText::Render("Pos_y", m_vLookPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 10.0f ,0.0f });
	CDebugText::Render("Pos_z", m_vLookPosition.z);
}

void CGameStartEvent::DebugOperation()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_vPosition.z += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_vPosition.z -= m_Speed;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_vPosition.y += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_vPosition.y -= m_Speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_vPosition.x += m_Speed;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_vPosition.x -= m_Speed;
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

