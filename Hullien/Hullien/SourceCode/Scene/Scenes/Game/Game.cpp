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
//	�Ǎ��֐�.
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
//	�X�V�֐�.
//============================.
void CGame::Update()
{
	CFog::Update();	// �t�H�O�̍X�V.


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
#ifdef _DEBUG
	if( GetAsyncKeyState(VK_F4) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
#endif	// #ifdef _DEBUG.
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
// ���f���̕`��.
//============================.
void CGame::ModelRender()
{
	//--------------------------------------------.
	// �`��p�X1.
	//--------------------------------------------.
	// �[�x�e�N�X�`���ɉe�p�̐[�x����������.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Shadow );
	m_pGameObjManager->Render();

	//--------------------------------------------.
	// �`��p�X2.
	//--------------------------------------------.
	// �G�t�F�N�g�Ȃǂ̕`��.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::Trans );
	CSceneTexRenderer::SetTransBuffer();
	m_pGameObjManager->Render();

	//--------------------------------------------.
	// �`��p�X3.
	//--------------------------------------------.
	// G-Buffer��color, normal, depth����������.

	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	m_pGameObjManager->Render();

	//--------------------------------------------.
	// �ŏI�`��.
	//--------------------------------------------.
	// G-Buffer���g�p���āA��ʂɕ`�悷��.
	CSceneTexRenderer::Render( !CSceneTransition::GetIsFade() );
}

// �Q�[�������֐�.
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

	// �ݒ蒆����Ȃ����.
	if( m_IsConfig == false && CFade::GetIsFade() == false ){
		// �X�^�[�g�{�^���������Đݒ��ʂ�\��������.
		if( CInput::IsMomentPress( EKeyBind::Start ) == true ){
			m_IsConfig = true;
			m_pGameObjManager->AnimationStop();	// �A�j���[�V�������~����.
		}
	}

	if( m_IsConfig == false ){
		m_pGameObjManager->Update();
		m_pWidgetManager->Update(m_pGameObjManager.get());
	} else {
		ConfigUpdate();		
	}
}

// �R���e�j���[�����֐�.
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

// �ݒ��ʂ̍X�V.
void CGame::ConfigUpdate()
{
	CConfigWidget::Update();	// �ݒ�UI�X�V.
	// �ݒ肪�I��������.
	if( CConfigWidget::IsEndConfig() == true ){
		m_pGameObjManager->ResumeAnimation();	// �A�j���[�V�������ĊJ.
		CConfigWidget::OffVolumeSeting();
		m_IsConfig = false;	// �ݒ�I��.
	}

	// �ݒ肪�I�����Ă���ΏI������.
	if( m_IsConfig == false ) return;
	// �^�C�g���֖߂邩,
	if( CConfigWidget::IsReturnToTitle() == true ){
		CConfigWidget::OffVolumeSeting();
		m_NextSceneState = ENextSceneState::Title;
	}
}

// �V�[���؂�ւ��֐�.
void CGame::ChangeEventScene()
{
	if (m_NowEventScene == EEventSceneState::Game)
	{
		// �v���C���[�����S�����ꍇ.
		if (m_pGameObjManager->IsGameOver() == true)
		{
			m_NowEventScene = EEventSceneState::Continue;
			return;
		}
		// �Q�[���N���A�̏ꍇ.
		if (m_pWidgetManager->IsGameFinish() == true)
		{
			SetNextScene(EEventSceneState::Clear);
		}
		else if (m_pGameObjManager->IsReturnAlien() == true)
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
	bool isCange = false;
	switch (m_NextSceneState)
	{
	case ENextSceneState::Game:
		CFade::SetFadeIn();
		StopAllBGM();	// BGM�̒�~.
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
		StopAllBGM();	// BGM�̒�~.
		if (CFade::GetIsFade() == true) return;
		m_pSceneManager->OnGameOver();
		m_pEventManager->NextEventMove();
		m_pSceneManager->OnEditSceneChangeActive();
		m_pSceneManager->NextSceneMove();
		isCange = true;
		break;
	case ENextSceneState::Title:
		CFade::SetFadeIn();
		StopAllBGM();	// BGM�̒�~.
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

// ���̃V�[����ݒ�.
void CGame::SetNextScene(EEventSceneState state, bool GameOver)
{
	CFade::SetFadeIn();
	StopAllBGM();	// BGM�̒�~.

	if (CFade::GetIsFade() == true) return;
	m_NowEventScene = state;
	if (GameOver == true)
	{
		m_pEventManager->OnGameOver();
	}
	m_pEventManager->NextEventMove();
}

// BGM�̒�~.
void CGame::StopBGM(const char* name)
{
	// BGM���Ȃ��Ă��Ȃ���ΏI��.
	if (CSoundManager::GetIsPlayBGM(name) == false) return;
	// BGM�t�F�[�h�A�E�g.
	CSoundManager::FadeOutBGM(name);

	// �X�v���C�g�t�F�[�h���I�����Ă��Ȃ���Ώ������Ȃ�.
	if (CFade::GetIsFade() == true) return;
	// BGM���~�߂�.
	while( CSoundManager::StopBGMThread(name) == false);
}

// �S�Ă�BGM��~.
void CGame::StopAllBGM()
{
	// StopBGM �Ŏ~�߂��Ă���ꍇ�A
	// StopBGMThread ���@�\���Ȃ��̂ň�u�����炷.
	CSoundManager::SetBGMVolume( "DangerBGM", 0.0f );
	CSoundManager::SetBGMVolume( "GameBGM", 0.0f );
	CSoundManager::AgainPlayBGM("DangerBGM");
	CSoundManager::AgainPlayBGM("GameBGM");

	StopBGM("StartEventBGM");
	StopBGM("GameBGM");
	StopBGM("DangerBGM");
	StopBGM("GameOverEvent");
}
