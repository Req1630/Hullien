#include "SceneManager.h"
#include "..\Scenes\SceneList.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Utility\Input\KeyInput\KeyInput.h"
#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\XAudio2\SoundManager.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\Utility\Command\Command.h"
#include "..\..\Common\Gage\Gage.h"
#include "..\..\Common\Gage\CircleGage.h"

CSceneManager::CSceneManager()
	: m_hWnd					( nullptr )
	, m_pScene					( nullptr )
	, m_pCommand				( nullptr )
	, m_NowBGMName				()
	, m_NowScene				( EScene::Start )
	, m_NextScene				( EScene::Start )
	, m_IsLoadEnd				( false )
	, m_IsGameOver				( false )
	, m_IsGameEnd				( false )
	, m_IsRetry					( false )
	, m_IsChangeEditScene		( false )
	, m_IsEditSceneChangeActive	( true )
{
	NextSceneMove();
	m_pCommand = std::make_unique<CCommand>();
	CInput::InitKeyBind();
}

CSceneManager::~CSceneManager()
{
}

//=================================.
//	更新関数.
//=================================.
void CSceneManager::Update()
{
	if( m_pScene == nullptr ) return;

	CInput::StateUpdate();

	if( m_IsLoadEnd == false ){
		// 読み込みが終了していない場合.
		// 読み込みを行う.
		m_IsLoadEnd = m_pScene->Load();
	} else {
		// 読み込みが終了したら.
		m_pScene->Update();	// シーンの更新.
		// ゲーム終了してなければ描画する.
		if( m_IsGameEnd == false ){
			if( m_IsLoadEnd == true )
			{
				CCameraManager::InitViewProj();
				m_pScene->Render();	// シーンの描画.
			}
			if( m_IsLoadEnd == true ) CEditRenderer::Render();
		}
	}

	ChangeEditScene();	// エディットシーンに切り替える.

	static CCircleGage* gage = new CCircleGage();
	static bool once = false;
	if( once == false ){
		once = true;
		gage->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() );
	} else {
		CInput::Vector2<float> t;
		t = CInput::GetAxisValue<float>( EAxisBind::L_Right, EAxisBind::L_Forward );
		gage->SetPosition({200.0f, 200.0f, 0.0f});
		gage->SetRotation({0.0f, 0.0f, atan2f(t.x, t.y)});
		gage->Render();
	}

	// フェードの描画.
	if (CFade::Init() == false) return;
	CFade::Render();

}

//=================================.
// 次のシーンに移動.
//=================================.
void CSceneManager::NextSceneMove()
{
	m_IsLoadEnd = false;
	m_IsChangeEditScene = false;
	switch( m_NextScene )
	{
	case EScene::Title:
		m_pScene		= std::make_shared<CTitle>( this );
		m_IsGameOver	= false;
		m_NowScene		= m_NextScene;
		m_NextScene		= EScene::GameMain;
		break;
	case EScene::GameMain:
		m_pScene	= std::make_shared<CGame>( this );
		m_NowScene	= m_NextScene;
		m_NextScene	= EScene::ResultCheck;
		break;
	case EScene::ResultCheck:
		if( m_IsGameOver == true ){
			// ゲームオーバー.
			m_NextScene = EScene::GameOver;
		} else {
			// ゲームクリア.
			m_NextScene = EScene::GameClear;
		}
		// もう一度この関数を呼んで、次のシーンを作成する.
		NextSceneMove();
		break;
	case EScene::GameClear:
		m_pScene	= std::make_shared<CGameClear>( this );
		m_IsRetry	= false;
		m_NowScene	= m_NextScene;
		m_NextScene	= EScene::Title;
		break;
	case EScene::GameOver:
		m_pScene	= std::make_shared<CGameOver>( this );
		m_IsRetry	= false;
		m_NowScene	= m_NextScene;
		m_NextScene = EScene::Title;
		break;
	case EScene::Config:
		m_pScene		= std::make_shared<CTitle>( this );
		m_IsGameOver	= false;
		m_NowScene		= m_NextScene;
		m_NextScene		= EScene::GameMain;
		break;
	default:
		break;
	}
}

//=================================.
// ゲーム終了.
//=================================.
void CSceneManager::EndGameClose()
{
	m_IsGameEnd = true;
	PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
}

//=================================.
// 設定シーンへ移動.
//=================================.
void CSceneManager::ConfigSceneMove()
{
	m_pScene = std::make_shared<CConfig>( this );
	m_IsLoadEnd = false;
	m_NextScene	= EScene::Title;
}

//=================================.
// ゲームリトライ.
//=================================.
void CSceneManager::RetryGame()
{
	m_IsLoadEnd = false;
	m_IsRetry = true;
	m_pScene = std::make_shared<CGame>( this );
}

//=================================.
// エディットシーンに変更.
//=================================.
void CSceneManager::ChangeEditScene()
{
	if( m_IsEditSceneChangeActive == false ) return;
	if( m_IsChangeEditScene == false ){
		m_pCommand->Update();
	}

	if( m_pCommand->IsSuccess() && m_IsChangeEditScene == false ){
		CFade::SetFadeIn();
		CSoundManager::PlaySE( "Determination" );
		CSoundManager::FadeOutBGM( m_NowBGMName );
		m_IsChangeEditScene = true;
	}
	if( m_IsChangeEditScene == true ){
		// フェードイン状態かつフェード中なら処理しない.
		if(CFade::GetFadeState() != CFade::EFadeState::In) return;
		if(CFade::GetIsFade() == true) return;
		if(CSoundManager::GetBGMVolume( m_NowBGMName ) > 0.0f) return;
		while(CSoundManager::StopBGMThread( m_NowBGMName )== false);
		m_pScene	= std::make_shared<CEditor>( this );
		m_IsLoadEnd	= false;
		m_IsChangeEditScene = false;
		m_NextScene	= m_NowScene;
	}
}