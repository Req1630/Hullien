#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameWidget\GameWidgetManager\GameWidgetManager.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ContinueWidget\ContinueWidget.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ConfigWidget\ConfigWidget.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\SceneEvent\EventManager\EventManager.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\GameObject\Widget\SceneTransition\SceneTransition.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\XAudio2\SoundManager.h"

#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\Common\Fog\Fog.h"
#include "..\..\..\Resource\EffectResource\EffectResource.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase			( pSceneManager )
	, m_pGameObjManager		( nullptr )
	, m_pWidgetManager		( nullptr )
	, m_pContinueWidget		( nullptr )
	, m_pEventManager		( nullptr )
	, m_NowEventScene		( EEventSceneState::GameStart )
	, m_NextSceneState		( ENextSceneState::None )
	, m_WaitCount			( 0.0f )
	, m_IsContinueSelect	( false )
	, m_IsConfig			( false )
	, m_IsPlayGameBGM		( false )
	, m_IsPlayDangerBGM		( false )
{
	m_pGameObjManager		= std::make_unique<CGameActorManager>();
	m_pWidgetManager		= std::make_unique<CGameWidgetManager>();
	m_pContinueWidget		= std::make_unique<CContinueWidget>();
	m_pEventManager			= std::make_unique<CEventManager>();
}

CGame::~CGame()
{
}

//============================.
//	読込関数.
//============================.
bool CGame::Load()
{
	CEffectResource::Release();
	CEffectResource::Load( CDirectX11::GetDevice(), CDirectX11::GetContext() );
	if( m_pGameObjManager->Init()	== false )	return false;
	if( m_pWidgetManager->Init()	== false )	return false;
	if( m_pContinueWidget->Init()	== false )	return false;
	if( CConfigWidget::Init()		== false )	return false;
	CConfigWidget::SetIsNowGameScene( true );
	if (m_pSceneManager->GetRetry() == false)
	{
		m_NowEventScene = EEventSceneState::GameStart;
		CSceneTransition::SetFadeIn();
	}
	else
	{
		m_NowEventScene = EEventSceneState::Game;
		CFade::SetFadeOut();
	}
	CSceneTexRenderer::SetIsStartLoad( false );

	return true;
}

//============================.
//	更新関数.
//============================.
void CGame::Update()
{
	CFog::Update();	// フォグの更新.


	switch (m_NowEventScene)
	{
	case EEventSceneState::Game:
		m_pSceneManager->OnEditSceneChangeActive();
		GameUpdate();
		break;
	case EEventSceneState::Continue:
		ContinueUpdate();
		break;
	case EEventSceneState::GameStart:
	case EEventSceneState::GameOver_Girl:
	case EEventSceneState::Clear:
		m_pSceneManager->OffEditSceneChangeActive();
		m_pEventManager->Update();
		break;
	default:
		break;
	}

	// イベントシーンの切り替え.
	ChangeEventScene();


#if 0	// 次のシーンへ移動.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if (CFade::GetIsFade() == true) return;
		SetChangeScene(EChangeSceneState::Clear);
	}
#else 
	// 次のシーンへ移動.
	NextSceneMove();
#endif	// #if 0.
#ifdef _DEBUG
	if( GetAsyncKeyState(VK_F4) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
#endif	// #ifdef _DEBUG.
}

//============================.
//	描画関数.
//============================.
void CGame::Render()
{
	switch (m_NowEventScene)
	{
	case EEventSceneState::Game:
		ModelRender();
		m_pGameObjManager->SpriteRender();
		m_pWidgetManager->Render();
		break;
	case EEventSceneState::Continue:
		if (m_pGameObjManager->IsGameOver() == false) {
			m_pEventManager->Render();
		}
		else {
			ModelRender();
			m_pGameObjManager->SpriteRender();
			m_pWidgetManager->Render();
		}
		m_pContinueWidget->Render();
		break;
	case EEventSceneState::GameStart:
	case EEventSceneState::GameOver_Girl:
	case EEventSceneState::Clear:
		m_pEventManager->Render();
		break;
	default:
		break;
	}
	if( m_IsConfig == true ){
		CConfigWidget::Render();
	}
	CEditRenderer::PushRenderProc( 
		[&]()
		{
			ImGui::Image( CSceneTexRenderer::GetGBuffer()[0], ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetGBuffer()[1], ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetGBuffer()[2], ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetTransBaffer(), ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetBlurTexture(), ImVec2(800, 400) );
		});	
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

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_pGameObjManager->Render();

	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// エフェクトなどの描画.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pGameObjManager->Render();

	//--------------------------------------------.
	// 描画パス3.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pGameObjManager->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.
	CSceneTexRenderer::Render( !CSceneTransition::GetIsFade() );
}

// ゲーム処理関数.
void CGame::GameUpdate()
{
	if (m_pGameObjManager->IsDanger() == false)
	{
		if( m_IsPlayGameBGM == false ){
			if( CSoundManager::GetMoveUpThread("GameBGM") == false ){
				CSoundManager::ThreadPlayBGM( "GameBGM" );
				m_pSceneManager->SetNowBGMName("GameBGM");

				CSoundManager::SetBGMVolume( "DangerBGM", 0.0f );
				CSoundManager::ThreadPlayBGM("DangerBGM");
			} else {
				CSoundManager::AgainPlayBGM("GameBGM");
			}
			CSoundManager::StopBGM("DangerBGM");
			CSoundManager::FadeInBGM("GameBGM");
			m_IsPlayGameBGM = true;
			m_IsPlayDangerBGM = false;
		}
	}
	else
	{
		if( m_IsPlayDangerBGM == false ){
			CSoundManager::StopBGM("GameBGM");
			if( CSoundManager::GetMoveUpThread("DangerBGM") == false ){
				CSoundManager::ThreadPlayBGM("DangerBGM");
				m_pSceneManager->SetNowBGMName("DangerBGM");
			} else {
				CSoundManager::AgainPlayBGM("DangerBGM");
			}
			CSoundManager::BGMPointSeek("DangerBGM", 0, 0, 0.0 );
			CSoundManager::FadeInBGM("DangerBGM");

			m_IsPlayDangerBGM = true;
			m_IsPlayGameBGM = false;
		}
	}

	// 設定中じゃなければ.
	if( m_IsConfig == false && CFade::GetIsFade() == false ){
		// スタートボタンを押して設定画面を表示させる.
		if( CInput::IsMomentPress( EKeyBind::Start ) == true ){
			m_IsConfig = true;
			m_pGameObjManager->AnimationStop();	// アニメーションを停止する.
		}
	}

	if( m_IsConfig == false ){
		m_pGameObjManager->Update();
		m_pWidgetManager->Update(m_pGameObjManager.get());
	} else {
		ConfigUpdate();		
	}
}

// コンテニュー処理関数.
void CGame::ContinueUpdate()
{
	m_pContinueWidget->Update();

	if (m_pContinueWidget->GetIsDrawing() == true) return;
	if( m_IsContinueSelect == true ) return;

	switch (m_pContinueWidget->GetSelectState())
	{
	case CContinueWidget::ESelectState::Yes:
		if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
			CSoundManager::PlaySE("Determination");
			m_NextSceneState = ENextSceneState::Game;
			m_IsContinueSelect = true;
		}
		break;
	case CContinueWidget::ESelectState::No:
		if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
			CSoundManager::PlaySE("CancelDetermination");
			m_NextSceneState = ENextSceneState::GameOver;
			m_IsContinueSelect = true;
		}
		break;
	default:
		break;
	}
}

// 設定画面の更新.
void CGame::ConfigUpdate()
{
	CConfigWidget::Update();	// 設定UI更新.
	// 設定が終了したか.
	if( CConfigWidget::IsEndConfig() == true ){
		m_pGameObjManager->ResumeAnimation();	// アニメーションを再開.
		CConfigWidget::OffVolumeSeting();
		m_IsConfig = false;	// 設定終了.
	}

	// 設定が終了していれば終了する.
	if( m_IsConfig == false ) return;
	// タイトルへ戻るか,
	if( CConfigWidget::IsReturnToTitle() == true ){
		CConfigWidget::OffVolumeSeting();
		m_NextSceneState = ENextSceneState::Title;
	}
}

// シーン切り替え関数.
void CGame::ChangeEventScene()
{
	if (m_NowEventScene == EEventSceneState::Game)
	{
		// プレイヤーが死亡した場合.
		if (m_pGameObjManager->IsGameOver() == true)
		{
			m_NowEventScene = EEventSceneState::Continue;
			return;
		}
		// ゲームクリアの場合.
		if (m_pWidgetManager->IsGameFinish() == true)
		{
			SetNextScene(EEventSceneState::Clear);
		}
		else if (m_pGameObjManager->IsReturnAlien() == true)
		{
			// 女の子がUFOまで連れ去られた場合.
			SetNextScene(EEventSceneState::GameOver_Girl, true);
		}
	}

	// イベントが終了していれば更新.
	if (m_pEventManager->GetIsEventEnd() == false) return;
	switch (m_NowEventScene)
	{
	case EEventSceneState::GameStart:
		m_NowEventScene = EEventSceneState::Game;
		break;
	case EEventSceneState::GameOver_Girl:
		m_NowEventScene = EEventSceneState::Continue;
		break;
	case EEventSceneState::Clear:
		m_NextSceneState = ENextSceneState::Clear;
		break;
	default:
		break;
	}
}

// 次のシーンに移行.
void CGame::NextSceneMove()
{
	bool isCange = false;
	switch (m_NextSceneState)
	{
	case ENextSceneState::Game:
		CFade::SetFadeIn();
		StopAllBGM();	// BGMの停止.
		if(CFade::GetIsFade() == true) return;
		m_pSceneManager->RetryGame();
		isCange = true;
		break;
	case ENextSceneState::Clear:
		m_pEventManager->NextEventMove();
		m_pSceneManager->OnEditSceneChangeActive();
		m_pSceneManager->NextSceneMove();
		isCange = true;
		break;
	case ENextSceneState::GameOver:
		CFade::SetFadeIn();
		StopAllBGM();	// BGMの停止.
		if (CFade::GetIsFade() == true) return;
		m_pSceneManager->OnGameOver();
		m_pEventManager->NextEventMove();
		m_pSceneManager->OnEditSceneChangeActive();
		m_pSceneManager->NextSceneMove();
		isCange = true;
		break;
	case ENextSceneState::Title:
		CFade::SetFadeIn();
		StopAllBGM();	// BGMの停止.
		if (CFade::GetIsFade() == true) return;
		m_pSceneManager->SetNextTitle();
		m_pSceneManager->OnEditSceneChangeActive();
		m_pSceneManager->NextSceneMove();
		isCange = true;
		break;
	default:
		break;
	}
	if( isCange == true ){
		CSceneTexRenderer::SetIsStartLoad( true );
		CSceneTexRenderer::SetSaveScreen( true );
		CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
		CSceneTexRenderer::SetGBuffer();
		CSceneTexRenderer::Render();
	}
}

// 次のシーンを設定.
void CGame::SetNextScene(EEventSceneState state, bool GameOver)
{
	CFade::SetFadeIn();
	StopAllBGM();	// BGMの停止.

	if (CFade::GetIsFade() == true) return;
	m_NowEventScene = state;
	if (GameOver == true)
	{
		m_pEventManager->OnGameOver();
	}
	m_pEventManager->NextEventMove();
}

// BGMの停止.
void CGame::StopBGM(const char* name)
{
	// BGMがなっていなければ終了.
	if (CSoundManager::GetIsPlayBGM(name) == false) return;
	// BGMフェードアウト.
	CSoundManager::FadeOutBGM(name);

	// スプライトフェードが終了していなければ処理しない.
	if (CFade::GetIsFade() == true) return;
	// BGMを止める.
	while( CSoundManager::StopBGMThread(name) == false);
}

// 全てのBGM停止.
void CGame::StopAllBGM()
{
	// StopBGM で止められている場合、
	// StopBGMThread が機能しないので一瞬だけ鳴らす.
	CSoundManager::SetBGMVolume( "DangerBGM", 0.0f );
	CSoundManager::SetBGMVolume( "GameBGM", 0.0f );
	CSoundManager::AgainPlayBGM("DangerBGM");
	CSoundManager::AgainPlayBGM("GameBGM");

	StopBGM("StartEventBGM");
	StopBGM("GameBGM");
	StopBGM("DangerBGM");
	StopBGM("GameOverEvent");
}
