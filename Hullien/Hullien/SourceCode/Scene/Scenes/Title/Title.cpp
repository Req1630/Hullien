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

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pWidget			( nullptr )
	, m_pConfigWidget	( nullptr )
	, m_IsChangeScene	( false )
	, m_IsDecision		( false )
	, m_IsNowConfig		( false )
{
	m_pWidget		= std::make_unique< CTitleWidget >();
	m_pConfigWidget	= std::make_unique<CConfigWidget>();
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
	if( m_pConfigWidget->Init() == false ) return false;
	CSoundManager::ThreadPlayBGM("TitleBGM");
	CSoundManager::FadeInBGM("TitleBGM");
	m_pSceneManager->SetNowBGMName("TitleBGM");
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CTitle::Update()
{
	if (CFade::GetIsFade() == true) return;
	if( m_IsNowConfig == false ){
		// ���肵�ĂȂ��ꍇUI�̍X�V.
		if( m_IsDecision == false ) m_pWidget->Update();
		//�V�[���؂�ւ�.
		ChangeScene();
	} else {
		m_pConfigWidget->Update();
		if( m_pConfigWidget->IsEndConfig()== true ){
			m_pConfigWidget->OffVolumeSeting();
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
	if( m_IsNowConfig == false ){
		if ( m_pWidget == nullptr ) return;
		m_pWidget->Render();
	} else {
		m_pConfigWidget->Render();
	}
}

//============================.
	// �V�[���؂�ւ��֐�.
//============================.
void CTitle::ChangeScene()
{
	// �{�^���������ꂽ.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		if (m_IsChangeScene == true) return;
		switch (m_pWidget->GetSelectState())
		{
		case CTitleWidget::ESelectState::Start:
			CSoundManager::PlaySE("Determination");
			CFade::SetFadeIn();
			CSoundManager::FadeOutBGM("TitleBGM");
			break;
		case CTitleWidget::ESelectState::Config:
			CSoundManager::PlaySE("Determination");
			m_IsNowConfig = true;
			break;
		case CTitleWidget::ESelectState::End:
			CSoundManager::PlaySE("CancelDetermination");
			CFade::SetFadeIn();
			CSoundManager::FadeOutBGM("TitleBGM");
			break;
		default:
			break;
		}
		m_IsChangeScene = true;
		m_IsDecision = true;
	}

	if (m_IsChangeScene == false) return;
	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	// �ǂ̃V�[���ɑJ�ڂ��邩.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while( CSoundManager::StopBGMThread("TitleBGM") == false);
		m_pSceneManager->NextSceneMove();
		break;
	case CTitleWidget::ESelectState::Config:
//		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
//		while( CSoundManager::StopBGMThread("TitleBGM") == false);
//		m_pSceneManager->ConfigSceneMove();
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

