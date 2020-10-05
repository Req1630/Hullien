#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\GameWidgetManager\GameWidgetManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ContinueWidget\ContinueWidget.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\SceneEvent\EventManager\EventManager.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

#include "..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\..\Common\PeraRenderer\PeraRenderer.h"

CGame::CGame(CSceneManager* pSceneManager)
	: CSceneBase(pSceneManager)
	, m_GameObjManager(nullptr)
	, m_WidgetManager(nullptr)
	, m_ContinueWidget(nullptr)
	, m_pSkyDome(nullptr)
	, m_pPeraRenderer(nullptr)
	, m_NowScene(ESceneState::GameStart)
	, m_NextSceneState(ENextSceneState::None)
	, m_IsChangeScene(false)
{
	m_GameObjManager = std::make_unique<CGameActorManager>();
	m_WidgetManager = std::make_unique<CGameWidgetManager>();
	m_ContinueWidget = std::make_unique<CContinueWidget>();
	m_pSkyDome = std::make_unique<CSkyDome>();
	m_pEventManager = std::make_unique<CEventManager>();
	m_pPeraRenderer = std::make_unique<CPeraRenderer>();
	CFade::SetFadeOut();
}

CGame::~CGame()
{
	CSoundManager::StopBGMThread("GameBGM");
	CSoundManager::StopBGMThread("DangerBGM");
	m_pPeraRenderer->Release();
}

//============================.
//	読込関数.
//============================.
bool CGame::Load()
{
	if (m_GameObjManager->Init() == false)	return false;
	if (m_WidgetManager->Init() == false)	return false;
	if (m_ContinueWidget->Init() == false)	return false;
	if (m_pSkyDome->Init() == false)		return false;
	if (m_pPeraRenderer->Init(nullptr, nullptr) == E_FAIL) return false;

	if (m_pSceneManager->GetRetry() == false)
	{
		m_NowScene = ESceneState::GameStart;
	}
	else
	{
		m_NowScene = ESceneState::Game;
	}

	CSoundManager::GetInstance()->m_fMaxBGMVolume = 0.5f;
	CSoundManager::SetBGMVolume("GameBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);
	CSoundManager::SetBGMVolume("DangerBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	switch (m_NowScene)
	{
	case ESceneState::GameStart:
	case ESceneState::GameOver_Girl:
	case ESceneState::GameOver_Player:
	case ESceneState::Clear:
		m_pEventManager->Update();
		break;
	case ESceneState::Continue:
		UpdateContinue();
		break;
	case ESceneState::Game:
		UpdateGame();
		break;
	default:
		break;
	}
	ChangeScene();


#if 0	// 次のシーンへ移動.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if (CFade::GetIsFade() == true) return;
		SetChangeScene(EChangeSceneState::Clear);
	}
#else 
	NextSceneMove();
#endif	// #if 0.
}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{
	switch (m_NowScene)
	{
	case ESceneState::Game:
		// モデルの描画.
		ModelRender();
		m_GameObjManager->SpriteRender();
		m_WidgetManager->Render();
		break;

	case ESceneState::Continue:
		m_ContinueWidget->Render();
		break;
	default:
		break;
	}
}

//============================.
// モデルの描画.
//============================.
void CGame::ModelRender()
{
	//--------------------------------------------.
	// 描画パス1.
	//--------------------------------------------.
	// 深度テクスチャに影用の深度を書き込む.

	CShadowMap::SetRenderPass(0);
	//	m_pSkyDome->Render();	// 背景の影はいらない.
	m_GameObjManager->Render();

	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CShadowMap::SetRenderPass(1);
	CDirectX11::SetGBuufer();
	m_pSkyDome->Render();
	m_GameObjManager->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.

	CDirectX11::SetBackBuffer();
	m_pPeraRenderer->Render(CDirectX11::GetGBuffer());
}

// ゲーム処理関数.
void CGame::UpdateGame()
{
	if (m_GameObjManager->IsDanger() == false)
	{
		CSoundManager::ThreadPlayBGM("GameBGM");
		CSoundManager::FadeInBGM("GameBGM");
		CSoundManager::FadeOutBGM("DangerBGM");
	}
	else
	{
		CSoundManager::ThreadPlayBGM("DangerBGM");
		CSoundManager::FadeInBGM("DangerBGM");
		CSoundManager::FadeOutBGM("GameBGM");
	}


	m_GameObjManager->Update();
	m_WidgetManager->Update(m_GameObjManager.get());
}

// コンテニュー処理関数.
void CGame::UpdateContinue()
{
	m_ContinueWidget->Update();

	if (m_ContinueWidget->GetIsDrawing() == true) return;
	switch (m_ContinueWidget->GetSelectState())
	{
	case CContinueWidget::ESelectState::Yes:
		if (GetAsyncKeyState(VK_RETURN) & 0x8000
			|| CXInput::B_Button() == CXInput::enPRESSED_MOMENT)
		{
			if (m_IsChangeScene == true) return;
			CSoundManager::PlaySE("DeterminationSE");
			m_NextSceneState = ENextSceneState::Game;
		}
		break;
	case CContinueWidget::ESelectState::No:
		if (GetAsyncKeyState(VK_RETURN) & 0x8000
			|| CXInput::B_Button() == CXInput::enPRESSED_MOMENT)
		{
			if (m_IsChangeScene == true) return;
			CSoundManager::PlaySE("End");
			m_NextSceneState = ENextSceneState::GameOver;
		}
		break;
	default:
		break;
	}
}

// シーン切り替え関数.
void CGame::ChangeScene()
{
	if (m_NowScene == ESceneState::Game)
	{
		// ゲームオーバーの場合.
		if (m_GameObjManager->IsGameOver() == true)
		{
			if (m_GameObjManager->IsGirlAbduct() == true)
			{
				m_NowScene = ESceneState::GameOver_Girl;
			}
			else
			{
				m_NowScene = ESceneState::GameOver_Player;
			}
			m_pEventManager->OnGameOver();
			m_pEventManager->NextEventMove();
		}

		// ゲームクリアの場合.
		if (m_WidgetManager->IsGameFinish() == true)
		{
			m_NowScene = ESceneState::Clear;
			m_pEventManager->NextEventMove();
		}
	}

	// イベントが終了していれば更新.
	if (m_pEventManager->GetIsEventEnd() == false) return;
	switch (m_NowScene)
	{
	case ESceneState::GameStart:
		m_NowScene = ESceneState::Game;
		break;
	case ESceneState::GameOver_Girl:
	case ESceneState::GameOver_Player:
		m_NowScene = ESceneState::Continue;
		break;
	case ESceneState::Clear:
		m_NextSceneState = ENextSceneState::Clear;
		break;
	default:
		break;
	}
}

// 次のシーンに移行.
void CGame::NextSceneMove()
{
	switch (m_NextSceneState)
	{
	case ENextSceneState::Game:
		m_pSceneManager->RetryGame();
		break;
	case ENextSceneState::Clear:
		m_pSceneManager->NextSceneMove();
		break;
	case ENextSceneState::GameOver:
		m_pSceneManager->OnGameOver();
		m_pSceneManager->NextSceneMove();
		break;
	default:
		break;
	}
}

