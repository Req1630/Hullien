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

#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\Common\Fog\Fog.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_GameObjManager	( nullptr )
	, m_WidgetManager	( nullptr )
	, m_ContinueWidget	( nullptr )
//	, m_pSkyDome		( nullptr )
	, m_ChangeSceneState( EChangeSceneState::Clear )
	, m_IsChangeScene	( false )
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
//	m_pSkyDome				= std::make_unique<CSkyDome>();
	m_pEventManager			= std::make_unique<CEventManager>();
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
//	if( m_pSkyDome->Init() == false )		return false;
	
	CSoundManager::GetInstance()->m_fMaxBGMVolume = 0.5f;
	CSoundManager::SetBGMVolume("GameBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);
	CSoundManager::SetBGMVolume("DangerBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);

	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGame::Update()
{
	CFog::Update();	// �t�H�O�̍X�V.

	if (m_pEventManager->GetIsEventEnd() == false)
	{
		m_pEventManager->Update();
	}
	else
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


		if (m_GameObjManager->IsGameOver() == false)
		{
			m_GameObjManager->Update();
			m_WidgetManager->Update(m_GameObjManager.get());
		}
		else
		{
			if (CFade::GetIsFade() == true) return;
			UpdateContinue();
		}

#if 0	// ���̃V�[���ֈړ�.
		if (GetAsyncKeyState(VK_RETURN) & 0x0001
			|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
		{
			if (CFade::GetIsFade() == true) return;
			SetChangeScene(EChangeSceneState::Clear);
		}
#else 
		if (m_WidgetManager->IsGameFinish() == true)
		{
			if (m_IsChangeScene == false)
			{
				m_pEventManager->NextEventMove();
				m_IsChangeScene = true;
			}
			if (m_pEventManager->GetIsEventEnd() == true
				&& m_ChangeSceneState == EChangeSceneState::None)
			{
				SetChangeScene(EChangeSceneState::Clear);
			}

		}
#endif	// #if 0.
	}

	ChangeScene();
}

//============================.
//	�`��֐�.
//============================.
void CGame::Render()
{
	if (m_pEventManager->GetIsEventEnd() == false) return;

	// ���f���̕`��.
	ModelRender();

	m_GameObjManager->SpriteRender();
	m_WidgetManager->Render();

	if (m_GameObjManager->IsGameOver() == true)
	{
		//�v���C���[�̗̑͂�0�ɂȂ������擾.
		// �R���e�j���[.
		m_ContinueWidget->Render();
	}

	CEditRenderer::PushRenderProc( 
		[&]()
		{
			ImGui::Image( CSceneTexRenderer::GetTransBaffer(), ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetShadowBuffer()[1], ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetShadowBuffer()[2], ImVec2(800, 400) );
		});	
}

//============================.
// ���f���̕`��.
//============================.
void CGame::ModelRender()
{
	//--------------------------------------------.
	// �`��p�X1.
	//--------------------------------------------.
	// �[�x�e�N�X�`���ɉe�p�̐[�x����������.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_GameObjManager->Render();

	//--------------------------------------------.
	// �`��p�X2.
	//--------------------------------------------.
	// �G�t�F�N�g�Ȃǂ̕`��.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_GameObjManager->Render();

	//--------------------------------------------.
	// �`��p�X3.
	//--------------------------------------------.
	// G-Buffer��color, normal, depth����������.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_GameObjManager->Render();

	//--------------------------------------------.
	// �ŏI�`��.
	//--------------------------------------------.
	// G-Buffer���g�p���āA��ʂɕ`�悷��.

	CDirectX11::SetBackBuffer();
	CSceneTexRenderer::Render();
}

//============================.
// �R���e�j���[�����֐�.
//============================.
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

//============================.
// �V�[���؂�ւ��֐�.
//============================.
void CGame::ChangeScene()
{
	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetIsFade() == true) return;

	// �ǂ̃V�[���ɑJ�ڂ��邩.
	SelectScene();
}

//============================.
// �V�[���̑I��.
//============================.
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

//============================.
// �V�[���؂�ւ��ݒ�֐�.
//============================.
void CGame::SetChangeScene( const EChangeSceneState& changeState )
{
	m_ChangeSceneState = changeState;
	CFade::SetFadeIn();
	m_IsChangeScene = true;
}
