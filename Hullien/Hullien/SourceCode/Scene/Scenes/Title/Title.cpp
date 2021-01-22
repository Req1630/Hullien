#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\GameObject\Widget\SceneWidget\TItleWidget\TitleWidget.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ConfigWidget\ConfigWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\GameObject\Widget\ButtonExplanation\ButtonExplanation.h"
#include "..\..\..\GameObject\Widget\SceneTransition\SceneTransition.h"
#include "..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\Editor\EditRenderer\EditRenderer.h"

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pWidget			( nullptr )
	, m_pButtonExp		( nullptr )
	, m_IsChangeScene	( false )
	, m_IsGameStart		( false )
	, m_IsDecision		( false )
	, m_IsNowConfig		( false )
{
	m_pWidget		= std::make_unique< CTitleWidget >();
	m_pButtonExp	= std::make_unique<CButtonExp>();
	CFade::SetFadeOut();
}

CTitle::~CTitle()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CTitle::Load()
{
	if ( m_pWidget->Init() == false ) return false;
	if( CConfigWidget::Init() == false ) return false;
	// �{�^������UI�̓ǂݍ���.
	if( m_pButtonExp->Init() == false ) return false;

	CConfigWidget::SetIsNowGameScene( false );
	CSoundManager::ThreadPlayBGM("TitleBGM");
	CSoundManager::FadeInBGM("TitleBGM");
	m_pSceneManager->SetNowBGMName("TitleBGM");
	CSceneTexRenderer::SetIsStartLoad( false );
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CTitle::Update()
{
	if (CFade::GetIsFade() == true) return;
	if( CSceneTransition::GetIsFade() == true ) return;

	if( m_IsNowConfig == false ){
		// ���肵�ĂȂ��ꍇUI�̍X�V.
		if( m_IsDecision == false ) m_pWidget->Update();
		//�V�[���؂�ւ�.
		ChangeScene();
	} else {
		CConfigWidget::Update();
		if( CConfigWidget::IsEndConfig()== true ){
			CConfigWidget::OffVolumeSeting();
			m_IsNowConfig = false;
			m_IsChangeScene = false;
			m_IsDecision = false;
		}
	}
}

//============================.
//	�`��֐�.
//============================.
void CTitle::Render()
{
	// �^�C�g���ł͉e��G�t�F�N�g��`�悳����\��͂Ȃ��̂�.
	//	G-Buffer�̂ݕ`�悳����.
	//--------------------------------------------.
	// �`��p�X1.
	//--------------------------------------------.
	// G-Buffer��color, normal, depth����������.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();

	// �^�C�g��UI�̕`��.
	if ( m_pWidget == nullptr ) return;
	m_pWidget->Render();

	if( m_IsNowConfig == false ){
		m_pButtonExp->SetIsCancelRender( false );
		m_pButtonExp->Render();
	}

	//--------------------------------------------.
	// �ŏI�`��.
	//--------------------------------------------.
	// G-Buffer���g�p���āA��ʂɕ`�悷��.
	//	Bloom�͎g�p���Ȃ��̂ŁA�����ɂ���.
	CSceneTexRenderer::Render( false );

	// �ݒ�UI�̕`��.
	if( m_IsNowConfig == true ) CConfigWidget::Render();

	CEditRenderer::PushRenderProc( 
		[&]()
		{
			ImGui::Image( CSceneTexRenderer::GetGBuffer()[0],		ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetGBuffer()[1],		ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetGBuffer()[2],		ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetTransBaffer(),		ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetBlurTexture(),		ImVec2(800, 400) );
			ImGui::Image( CSceneTexRenderer::GetTmpScreenTexture(),	ImVec2(800, 400) );
		});
}

//============================.
	// �V�[���؂�ւ��֐�.
//============================.
void CTitle::ChangeScene()
{
	// �{�^���������ꂽ.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		if (m_IsChangeScene == true || m_IsGameStart == true) return;
		switch (m_pWidget->GetSelectState())
		{
		case CTitleWidget::ESelectState::Start:
			CSoundManager::PlaySE("Determination");
			m_IsGameStart = true;
			break;
		case CTitleWidget::ESelectState::Config:
			CSoundManager::PlaySE("Determination");
			m_IsNowConfig = true;
			break;
		case CTitleWidget::ESelectState::End:
			CSoundManager::PlaySE("CancelDetermination");
			CFade::SetFadeIn();
			CSoundManager::FadeOutBGM("TitleBGM");
			m_IsChangeScene = true;
			break;
		default:
			break;
		}
		m_IsDecision = true;
	}
	if( m_IsGameStart == true ){
		CSoundManager::FadeOutBGM("TitleBGM");
//		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
//		while( CSoundManager::StopBGMThread("TitleBGM") == false);

		CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
		CSceneTexRenderer::SetGBuffer();

		m_pWidget->Render();
		
		CSceneTexRenderer::Render( false );
		CSceneTexRenderer::SetSaveScreen( true );

		m_pSceneManager->NextSceneMove();
		CSceneTransition::SetSrcTexture( CSceneTexRenderer::GetTmpScreenTexture() );
		CSceneTransition::SetTexture();
		return;
	}

	if (m_IsChangeScene == false) return;
	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	// �ǂ̃V�[���ɑJ�ڂ��邩.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		// �X�^�[�g�������������ς�����̂ŁA
		// �ȉ��̏����͂��Ȃ�.
//		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
//		while( CSoundManager::StopBGMThread("TitleBGM") == false);
//		m_pSceneManager->NextSceneMove();
		break;
	case CTitleWidget::ESelectState::Config:
		break;
	case CTitleWidget::ESelectState::End:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while (CSoundManager::StopBGMThread("TitleBGM") == false);
		// �E�B���h�E�����.
		m_pSceneManager->EndGameClose();
		break;
	default:
		break;
	}
}

