#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\GameWidgetManager\GameWidgetManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ContinueWidget\ContinueWidget.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase				( pSceneManager )
	, m_GameObjManager	( nullptr )
	, m_WidgetManager		( nullptr )
	, m_ContinueWidget		( nullptr )
	, m_Fade						( nullptr )
	, m_ChangeSceneState ( EChangeSceneState::Clear )
	, m_IsChangeScene		( false )
	, m_DispContinue			(0)
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
	m_Fade						= std::make_unique<CFade>();
	m_Fade->SetFadeOut(0.05f);

}

CGame::~CGame()
{
}

//============================.
//	読込関数.
//============================.
bool CGame::Load()
{
	if (InitFade() == false)							return false;
	if( m_GameObjManager->Init() == false )	return false;
	if( m_WidgetManager->Init() == false )		return false;
	if( m_ContinueWidget->Init() == false )		return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	if (m_Fade->GetFadeState() == CWidget::EFadeState::Out
		&& m_Fade->GetIsFade() == true)
	{
		m_Fade->Update();
	}

	m_GameObjManager->Update();
	m_WidgetManager->Update(m_GameObjManager.get());
	UpdateContinue();
	ChangeScene();	

#if 1	// 次のシーンへ移動.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 )
	{
		SetChangeScene(EChangeSceneState::Clear);
	}
#else 
	if( m_pLimitTime->IsFinish() == true )
	{
		m_pSceneManager->NextSceneMove();
	}
#endif	// #if 0.
}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{
	m_GameObjManager->Render();
	m_WidgetManager->Render();

	if (m_DispContinue == 1)
	{
		//プレイヤーの体力が0になったか取得.
		// コンテニュー.
		m_ContinueWidget->Render();
	}
	if (m_Fade->GetIsFade() == true)
	{
		m_Fade->Render();
	}

}

// フェード初期化関数.
bool CGame::InitFade()
{
	if (m_Fade->Init() == false) return false;
	m_Fade->SetFadeOut(0.01f);

	return true;
}

// コンテニュー処理関数.
void CGame::UpdateContinue()
{
	if (GetAsyncKeyState(VK_F1) & 0x0001)
	{
		m_DispContinue++;
		if (m_DispContinue >= 2)
		{
			m_DispContinue = 0;
		}
	}

	if (m_DispContinue == 1)
	{
		//プレイヤーの体力が0になったか取得.
		// コンテニュー.
		m_ContinueWidget->Update();

		if (m_ContinueWidget->GetIsDrawing() == true) return;
		switch (m_ContinueWidget->GetSelectState())
		{
		case CContinueWidget::ESelectState::Yes:
			if (GetAsyncKeyState(VK_RETURN) & 0x0001)
			{
				SetChangeScene(EChangeSceneState::Game);
			}
			break;
		case CContinueWidget::ESelectState::No:
			if (GetAsyncKeyState(VK_RETURN) & 0x0001)
			{
				SetChangeScene( EChangeSceneState::GameOver );
			}
			break;
		default:
			break;
		}
	}
}

// シーン切り替え関数.
void CGame::ChangeScene()
{
	// シーン切り替えフラグが立っていなければ処理しない.
	if (m_IsChangeScene == false) return;
	m_Fade->Update();

	// フェードイン状態かつフェード中なら処理しない.
	if (m_Fade->GetFadeState() == CWidget::EFadeState::In
		&& m_Fade->GetIsFade() == true) return;

	// どのシーンに遷移するか.
	SelectScene();
}

// シーンの選択.
void CGame::SelectScene()
{
	switch (m_ChangeSceneState)
	{
	case EChangeSceneState::Game:
		// リトライ処理.
		m_pSceneManager->RetryGame();
		break;
	case EChangeSceneState::Clear:
		// 次のシーンへ移行.
		m_pSceneManager->NextSceneMove();
		break;
	case EChangeSceneState::GameOver:
		// ゲームオーバーの設定.
		m_pSceneManager->OnGameOver();
		// 次のシーンへ移行.
		m_pSceneManager->NextSceneMove();
		break;
	default:
		break;
	}
}

// シーン切り替え設定関数.
void CGame::SetChangeScene( const EChangeSceneState& changeState )
{
	m_ChangeSceneState = changeState;
	m_Fade->SetFadeIn(0.01f);
	m_IsChangeScene = true;
}
