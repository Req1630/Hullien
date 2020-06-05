#include "SceneManager.h"
#include "..\Scenes\SceneList.h"

CSceneManager::CSceneManager()
	: m_pScene		( nullptr )
	, m_NowScene	( EScene::Start )
	, m_NextScene	( EScene::Start )
	, m_IsLoadEnd	( false )
	, m_IsGameOver	( false )
{
	NextSceneMove();
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

	if( m_IsLoadEnd == false ){
		// 読み込みが終了していない場合.
		// 読み込みを行う.
		m_IsLoadEnd = m_pScene->Load();
	} else {
		// 読み込みが終了したら.
		m_pScene->Update();	// シーンの更新.
		m_pScene->Render();	// シーンの描画.
	}
	ChangeEditScene();	// エディットシーンに切り替える.
}

//=================================.
// 次のシーンに移動.
//=================================.
void CSceneManager::NextSceneMove()
{
	m_IsLoadEnd = false;
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
		m_NowScene	= m_NextScene;
		m_NextScene	= EScene::Title;
		break;
	case EScene::GameOver:
		m_pScene	= std::make_shared<CGameOver>( this );
		m_NowScene	= m_NextScene;
		m_NextScene = EScene::Title;
		break;
	default:
		break;
	}
}

//=================================.
// エディットシーンに変更.
//=================================.
void CSceneManager::ChangeEditScene()
{
	if( ( GetAsyncKeyState('E') & 0x0001 ) &&
		( GetAsyncKeyState('D') & 0x0001 ) ){
		if( !(GetAsyncKeyState(VK_BACK) & 0x0001 ) ) return;
		m_pScene	= std::make_shared<CEditor>( this );
		m_IsLoadEnd	= false;
		m_NextScene	= m_NowScene;
	}
}