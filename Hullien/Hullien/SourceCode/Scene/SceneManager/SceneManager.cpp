#include "SceneManager.h"
#include "..\Scenes\SceneList.h"
#include "..\..\Utility\XInput\XInput.h"
#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\Editor\EditRenderer\EditRenderer.h"

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
//	�X�V�֐�.
//=================================.
void CSceneManager::Update()
{
	if( m_pScene == nullptr ) return;

	CXInput::StatsUpdate();	// �R���g���[���[�̍X�V.

	if( m_IsLoadEnd == false ){
		// �ǂݍ��݂��I�����Ă��Ȃ��ꍇ.
		// �ǂݍ��݂��s��.
		m_IsLoadEnd = m_pScene->Load();
	} else {
		// �ǂݍ��݂��I��������.
		m_pScene->Update();	// �V�[���̍X�V.
		m_pScene->Render();	// �V�[���̕`��.
		CEditRenderer::Render();
	}

	ChangeEditScene();	// �G�f�B�b�g�V�[���ɐ؂�ւ���.
}

//=================================.
// ���̃V�[���Ɉړ�.
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
			// �Q�[���I�[�o�[.
			m_NextScene = EScene::GameOver;
		} else {
			// �Q�[���N���A.
			m_NextScene = EScene::GameClear;
		}
		// ������x���̊֐����Ă�ŁA���̃V�[�����쐬����.
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
// �G�f�B�b�g�V�[���ɕύX.
//=================================.
void CSceneManager::ChangeEditScene()
{
	if( ( GetAsyncKeyState('E') & 0x8000 ) &&
		( GetAsyncKeyState('D') & 0x8000 ) ){
		if( !(GetAsyncKeyState('Q') & 0x8000 ) ) return;
		m_pScene	= std::make_shared<CEditor>( this );
		m_IsLoadEnd	= false;
		m_NextScene	= m_NowScene;
	}
}