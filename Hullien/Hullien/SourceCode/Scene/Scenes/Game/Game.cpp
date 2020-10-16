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
	, m_NowEventScene	( EEventSceneState::GameStart )
	, m_NextSceneState	( ENextSceneState::None )
	, m_IsChangeScene	( false )
{
	m_GameObjManager		= std::make_unique<CGameActorManager>();
	m_WidgetManager			= std::make_unique<CGameWidgetManager>();
	m_ContinueWidget		= std::make_unique<CContinueWidget>();
	m_pEventManager			= std::make_unique<CEventManager>();
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
	
	if (m_pSceneManager->GetRetry() == false)
	{
		m_NowEventScene = EEventSceneState::GameStart;
	}
	else
	{
		m_NowEventScene = EEventSceneState::Game;
		CFade::SetFadeOut();
	}

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

	switch (m_NowEventScene)
	{
	case EEventSceneState::Game:
		GameUpdate();
		break;
	case EEventSceneState::Continue:
		ContinueUpdate();
		break;
	case EEventSceneState::GameStart:
	case EEventSceneState::GameOver_Girl:
	case EEventSceneState::GameOver_Player:
	case EEventSceneState::Clear:
		m_pEventManager->Update();
		break;
	default:
		break;
	}

	// �C�x���g�V�[���̐؂�ւ�.
	ChangeEventScene();


#if 0	// ���̃V�[���ֈړ�.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if (CFade::GetIsFade() == true) return;
		SetChangeScene(EChangeSceneState::Clear);
	}
#else 
	// ���̃V�[���ֈړ�.
	NextSceneMove();
#endif	// #if 0.
}

//============================.
//	�`��֐�.
//============================.
void CGame::Render()
{
	switch (m_NowEventScene)
	{
	case EEventSceneState::Game:
		ModelRender();
		m_GameObjManager->SpriteRender();
		m_WidgetManager->Render();
		break;
	case EEventSceneState::Continue:
		if (m_GameObjManager->IsGameOver() == false) {
			m_pEventManager->Render();
		}
		else {
			ModelRender();
			m_GameObjManager->SpriteRender();
			m_WidgetManager->Render();
		}
		m_ContinueWidget->Render();
		break;
	case EEventSceneState::GameStart:
	case EEventSceneState::GameOver_Girl:
	case EEventSceneState::Clear:
		m_pEventManager->Render();
		break;
	default:
		break;
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

// �Q�[�������֐�.
void CGame::GameUpdate()
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

// �R���e�j���[�����֐�.
void CGame::ContinueUpdate()
{
	m_ContinueWidget->Update();

	if (m_ContinueWidget->GetIsDrawing() == true) return;
	switch (m_ContinueWidget->GetSelectState())
	{
	case CContinueWidget::ESelectState::Yes:
		if (GetAsyncKeyState(VK_RETURN) & 0x8000
			|| CXInput::B_Button() == CXInput::enPRESSED_MOMENT)
		{
			CSoundManager::PlaySE("Determination");
			m_NextSceneState = ENextSceneState::Game;
		}
		break;
	case CContinueWidget::ESelectState::No:
		if (GetAsyncKeyState(VK_RETURN) & 0x8000
			|| CXInput::B_Button() == CXInput::enPRESSED_MOMENT)
		{
			CSoundManager::PlaySE("End");
			m_NextSceneState = ENextSceneState::GameOver;
		}
		break;
	default:
		break;
	}
}

// �V�[���؂�ւ��֐�.
void CGame::ChangeEventScene()
{
	if (m_NowEventScene == EEventSceneState::Game)
	{
		// �v���C���[�����S�����ꍇ.
		if (m_GameObjManager->IsGameOver() == true)
		{
			m_NowEventScene = EEventSceneState::Continue;
			return;
		}
		// �Q�[���N���A�̏ꍇ.
		if (m_WidgetManager->IsGameFinish() == true)
		{
			SetNextScene(EEventSceneState::Clear);
		}
		else if (m_GameObjManager->IsReturnAlien() == true)
		{
			// ���̎q��UFO�܂ŘA�ꋎ��ꂽ�ꍇ.
			SetNextScene(EEventSceneState::GameOver_Girl, true);
		}
	}

	// �C�x���g���I�����Ă���΍X�V.
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

// ���̃V�[���Ɉڍs.
void CGame::NextSceneMove()
{
	switch (m_NextSceneState)
	{
	case ENextSceneState::Game:
		if (m_IsChangeScene == false)
		{
			CFade::SetFadeIn();
			m_IsChangeScene = true;
		}
		CSoundManager::FadeOutBGM("GameOverEvent");
		if(CSoundManager::GetIsPlayBGM("GameBGM") == true) CSoundManager::StopBGMThread("GameBGM");
		if(CSoundManager::GetIsPlayBGM("DangerBGM") == true) CSoundManager::StopBGMThread("DangerBGM");
		if(CFade::GetIsFade() == true) return;
		CSoundManager::StopBGMThread("GameOverEvent");
		CSoundManager::StopBGMThread("GameOverEvent");
		m_pSceneManager->RetryGame();
		break;
	case ENextSceneState::Clear:
		CSoundManager::StopBGMThread("GameOverEvent");
		m_pEventManager->NextEventMove();
		m_pSceneManager->NextSceneMove();
		break;
	case ENextSceneState::GameOver:
		if (m_IsChangeScene == false)
		{
			CFade::SetFadeIn();
			m_IsChangeScene = true;
		}
		CSoundManager::FadeOutBGM("GameOverEvent");
		if (CFade::GetIsFade() == true) return;
		if (CSoundManager::GetIsPlayBGM("GameBGM") == true) CSoundManager::StopBGMThread("GameBGM");
		if (CSoundManager::GetIsPlayBGM("DangerBGM") == true) CSoundManager::StopBGMThread("DangerBGM");
		CSoundManager::StopBGMThread("GameOverEvent");
		m_pSceneManager->OnGameOver();
		m_pEventManager->NextEventMove();
		m_pSceneManager->NextSceneMove();
		break;
	default:
		break;
	}
}

// ���̃V�[����ݒ�.
void CGame::SetNextScene(EEventSceneState state, bool GameOver)
{
	if (m_IsChangeScene == false)
	{
		CFade::SetFadeIn();
		m_IsChangeScene = true;
	}
	if (CFade::GetIsFade() == true) return;
	m_NowEventScene = state;
	CSoundManager::StopBGMThread("GameBGM");
	CSoundManager::StopBGMThread("DangerBGM");
	CSoundManager::StopBGMThread("StartEventBGM");
	if (GameOver == true)
	{
		m_pEventManager->OnGameOver();
	}
	m_pEventManager->NextEventMove();
}

