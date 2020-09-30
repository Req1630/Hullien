#include "GameStartEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include ".\..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventGirl\EventGirl.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventAlien\EventAlien_A\EventAlien_A.h"
#include "..\..\..\GameObject\Actor\Barrier\Barrier.h"
#include "..\..\..\GameObject\MotherShipUFO\MotherShipUFO.h"
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
	, m_NowStep				( 0 )
	, m_Speed				( 0.0f )
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
	if( m_pGroundStage->Init() == false ) return false;			// 地面の初期化.
	if( m_pSpawnUFO->Init() == false ) return false;			// UFOの初期化.
	if( m_pPlayer->Init() == false ) return false;					// プレイヤーの初期化.
	if( m_pGirl->Init() == false ) return false;						// 女の子の初期化.
	if( m_pAlienA->Init() == false ) return false;					// 宇宙人Aの初期化.
	if (m_pMotherShipUFO->Init() == false) return false;		// マザーシップの初期化.

	m_IsEventEnd = false;
	m_vCameraPosition = { 23.0f , 7.5f, 20.0f };
	m_vLookPosition = m_pPlayer->GetPosition();
	m_vLookPosition.y = m_pPlayer->GetPosition().y + 3.0f;
	m_vUFOPosition = { 0.0f, 12.0f, 120.0f };
	m_stPlayer.vPosition.z = 60.0f;
	m_stGirl.vPosition.z =  64.5f;
	m_pMotherShipUFO->SetPosition({ 0.0f, 20.0f, 100.0f });

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
	bool flag = true;

	// アクタの更新.
	ActorUpdate();

	switch (m_NowStep)
	{ 
	case 0:	//逃げるプレイヤーと女の子.
		m_vCameraPosition.z = m_stPlayer.vPosition.z;
		m_vLookPosition = m_stPlayer.vPosition;
		break;

	case 1:	//2人を追うUFOの視点.
		m_vCameraPosition = m_vUFOPosition;
		m_vCameraPosition.y = m_vUFOPosition.y + 5.0f;
		m_vCameraPosition.z = m_vUFOPosition.z - 5.0f;
		m_vLookPosition.z = m_pPlayer->GetPosition().z - 1.5f;
		break;

	case 2:	//UFO定位置につく.
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

		// カメラ位置.
		m_vCameraPosition = D3DXVECTOR3(3.5f, 7.0f, 10.5f);

		// カメラの視点移動.
		m_vLookPosition = m_pSpawnUFO->GetPosition();


		// 次のステップ.
		if (m_pSpawnUFO->GetPosition().z < -10.0f)
		{
			m_NowStep++;
		}
		break;

	case 3: // UFO停止.
		count++;
		if (count < 100)
		{
			m_vUFOPosition.y =  m_vUFOPosition.y + static_cast<float>(sin(D3DX_PI * 2.0f / 100.0f * count) * 0.1f);
		}
		else
		{
			if (m_vUFOPosition.y >= 11.5f) m_vUFOPosition.y -= 0.01f;
		}

		if (m_vUFOPosition.y < 11.5f && count >= 200)
		{
			m_NowStep++;
			count = 0;
		}
		break;

	case 4: //宇宙人登場.
		m_vCameraPosition = D3DXVECTOR3(0.0f, 8.5f, -1.0f);
		m_vLookPosition.y = m_pAlienA->GetPosition().y+1.0f;
		if (m_IsDisp == false)
		{
			m_pAlienA->Spawn(m_pSpawnUFO->GetPosition());
			m_IsDisp = true;
		}

		if (m_pAlienA->GetPosition().y >= 7.0f)
		{
			y = m_pAlienA->GetPosition().y - 0.05f;
		}
		else
		{
			m_NowStep++;
		}

		m_pAlienA->SetPosition({
			m_pAlienA->GetPosition().x,
			y,
			m_pAlienA->GetPosition().z });
		break;

	case 5: // 宇宙人ダッシュ.
		count++;
		y = m_pAlienA->GetPosition().y + static_cast<float>(sin(D3DX_PI * 2.0f / 90.0f * count) * 0.01f);
		if (count <= 200)
		{
			z = m_pAlienA->GetPosition().z;
		}
		else
		{
			z = m_pAlienA->GetPosition().z + 0.5f;
		}

		m_pAlienA->SetPosition({
			m_pAlienA->GetPosition().x,
			y,
			z });

		if (z >= 0.0f) 
		{ 
			m_NowStep++;
		}
		break;

	case 6: // 女の子捕まる.
		m_vCameraPosition = { -20.0f, 5.0f, -10.0f };
		m_vLookPosition = m_pPlayer->GetPosition();
		m_pAlienA->SetPosition({
			m_pAlienA->GetPosition().x,
			5.0f,
			z });

		if (z <= 50.0f)
		{
			z = m_pAlienA->GetPosition().z + 0.3f;
		}
		else
		{
			m_NowStep++; 
			m_vCameraPosition = { -6.5f, 8.4f, -10.0f };
			m_vLookPosition = m_pGirl->GetPosition();
		}

		m_pAlienA->Collision( m_pGirl.get() );
		m_pGirl->Collision(m_pAlienA.get());

		break;

	case 7:	//バリア発動指示.
		m_pPlayer->SetRotationY(static_cast<float>(D3DXToRadian(180)));
		m_pPlayer->Update();

		if (m_pPlayer->IsSpecialAbility() == true)
		{
			m_pBarrier->Init();	// バリアの初期化.
			m_NowStep++;

			m_vCameraPosition = { -38.4f,3.5f,4.5f };
			m_vLookPosition = { 0.0f,12.0f,50.0f };
		}
		break;

	case 8:	//バリア発動.
		m_pEventCamera->SetViewingAngle(0.5f);

		m_pBarrier->SetTargetPos(*m_pGirl.get());
		m_pBarrier->Update();
		m_pAlienA->Collision(m_pBarrier.get());

		m_stAlien.vPosition = m_pAlienA->GetPosition();
		if (m_pAlienA->IsBarrierHit() == true) { m_stAlien.vPosition.x -= 1.0f; }
		m_pAlienA->SetPosition(m_stAlien.vPosition);

		if (m_pBarrier->IsActive() == false)
		{
			m_NowStep++;
		}
		break;

	case 9:	// 女の子帰還.
		m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;
		CFade::SetFadeIn();
		if (CFade::GetInstance()->GetIsFade() == false) { m_NowStep++; }
		break; 

	case 10:	//ゲームシーンへ.
		m_IsEventEnd = true;
		break;

	default:
		break;
	}

	m_pEventCamera->SetPosition( m_vCameraPosition );
	m_pEventCamera->SetLookPosition(m_vLookPosition);
	CCameraManager::SetCamera( m_pEventCamera );

	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// 描画関数.
void CGameStartEvent::Render()
{
	m_pGroundStage->Render();	// ステージの描画.
	m_pSpawnUFO->Render();		// UFOの描画.
	m_pPlayer->Render();			// プレイヤーの描画.
	m_pGirl->Render();				// 女の子の描画.
	if (m_IsDisp == true)
	{
		m_pAlienA->Render();			// 宇宙Aの描画.
	}
	m_pBarrier->Render();
	m_pMotherShipUFO->Render();
	CFade::Render();

	DebugRender();
}

// アクタの更新関数.
void CGameStartEvent::ActorUpdate()
{
	switch (m_NowStep)
	{
	case 0:
		m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
		m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

		// 次のステップへ.
		if (m_stPlayer.vPosition.z <= 40.0f) { m_NowStep++; }
		break;
	case 1:
		if (m_stPlayer.vPosition.z > 0.0f)
		{
			m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
			m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;
		}

		// UFO移動.
		m_vUFOPosition.z -= 0.3f;

		// 次のステップへ.
		if (m_pSpawnUFO->GetPosition().z <= 40.0f)	m_NowStep++;
		break;
	case 2:
		// プレイヤーの位置設定.
		m_stPlayer.vPosition.z = 0.0f;
		// 女の子の位置設定.
		m_stGirl.vPosition.z = 4.5f;
		break;
	case 6:
		m_stGirl.vPosition = m_pGirl->GetPosition();
		break;
	default:
		break;
	}

	// プレイヤー.
	m_pPlayer->SetOptionalState(m_stPlayer);
	// 女の子.
	m_pGirl->SetOptionalState(m_stGirl);
	// UFOの位置設定.
	m_pSpawnUFO->SetPosition(m_vUFOPosition);


}

// カメラの更新関数.
void CGameStartEvent::CameraUpdate()
{
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

