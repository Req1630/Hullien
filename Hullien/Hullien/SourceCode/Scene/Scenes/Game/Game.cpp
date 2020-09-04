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
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_GameObjManager	( nullptr )
	, m_WidgetManager	( nullptr )
	, m_ContinueWidget	( nullptr )
	, m_ChangeSceneState( EChangeSceneState::Clear )
	, m_IsChangeScene	( false )
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
	CFade::SetFadeOut();

}

CGame::~CGame()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGame::Load()
{
	if( m_GameObjManager->Init() == false )	return false;
	if( m_WidgetManager->Init() == false )	return false;
	if( m_ContinueWidget->Init() == false )	return false;

	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGame::Update()
{
	if (CFade::GetIsFade() == true) return;

	//if (m_GameObjManager->IsDanger() == false)
	//{
	//	CSoundManager::ThreadPlayBGM("GameBGM");
	//	CSoundManager::StopBGMThread("DangerBGM");
	//}
	//else
	//{
	//	CSoundManager::ThreadPlayBGM("DangerBGM");
	//	CSoundManager::StopBGMThread("GameBGM");
	//}


	if (m_GameObjManager->IsGameOver() == false)
	{
		m_GameObjManager->Update();
		m_WidgetManager->Update(m_GameObjManager.get());
	}
	else
	{
		UpdateContinue();
	}

#if 0	// ���̃V�[���ֈړ�.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if (CFade::GetIsFade() == true) return;
		SetChangeScene(EChangeSceneState::Clear);
	}
#else 
	if(m_WidgetManager->IsGameFinish() == true )
	{
		if (m_IsChangeScene == false)
		{
			SetChangeScene(EChangeSceneState::Clear);
		}
	}
#endif	// #if 0.

	ChangeScene();

}

//============================.
//	�`��֐�.
//============================.
void CGame::Render()
{
	m_GameObjManager->Render();
	m_GameObjManager->SpriteRender();
	m_WidgetManager->Render();

	if (m_GameObjManager->IsGameOver() == true)
	{
		//�v���C���[�̗̑͂�0�ɂȂ������擾.
		// �R���e�j���[.
		m_ContinueWidget->Render();
	}
}

// �R���e�j���[�����֐�.
void CGame::UpdateContinue()
{
	//�v���C���[�̗̑͂�0�ɂȂ������擾.
	// �R���e�j���[.
	m_ContinueWidget->Update();

	if (m_ContinueWidget->GetIsDrawing() == true) return;
	switch (m_ContinueWidget->GetSelectState())
	{
	case CContinueWidget::ESelectState::Yes:
		if (GetAsyncKeyState(VK_RETURN) & 0x0001 
			|| CXInput::B_Button() == CXInput::enSEPARATED)
		{
			if (m_IsChangeScene == true) return;
			CSoundManager::PlaySE("DeterminationSE");
			SetChangeScene(EChangeSceneState::Game);
		}
		break;
	case CContinueWidget::ESelectState::No:
		if (GetAsyncKeyState(VK_RETURN) & 0x0001
			|| CXInput::B_Button() == CXInput::enSEPARATED)
		{
			if (m_IsChangeScene == true) return;
			CSoundManager::PlaySE("End");
			SetChangeScene( EChangeSceneState::GameOver );
		}
		break;
	default:
		break;
	}
}

// �V�[���؂�ւ��֐�.
void CGame::ChangeScene()
{
	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetIsFade() == true) return;

	// �ǂ̃V�[���ɑJ�ڂ��邩.
	SelectScene();
}

// �V�[���̑I��.
void CGame::SelectScene()
{
	if (m_GameObjManager->IsDanger() == false)
	{
		CSoundManager::StopBGMThread("GameBGM");
	}
	else
	{
		CSoundManager::StopBGMThread("DangerBGM");
	}

	switch (m_ChangeSceneState)
	{
	case EChangeSceneState::Game:
		// ���g���C����.
		m_pSceneManager->RetryGame();
		break;
	case EChangeSceneState::Clear:
		// ���̃V�[���ֈڍs.
		m_pSceneManager->NextSceneMove();
		break;
	case EChangeSceneState::GameOver:
		// �Q�[���I�[�o�[�̐ݒ�.
		m_pSceneManager->OnGameOver();
		// ���̃V�[���ֈڍs.
		m_pSceneManager->NextSceneMove();
		break;
	default:
		break;
	}
}

// �V�[���؂�ւ��ݒ�֐�.
void CGame::SetChangeScene( const EChangeSceneState& changeState )
{
	m_ChangeSceneState = changeState;
	CFade::SetFadeIn();
	m_IsChangeScene = true;
}
