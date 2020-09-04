#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\GameObject\Widget\SceneWidget\TItleWidget\TitleWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

CTitle::CTitle(CSceneManager* pSceneManager)
	: CSceneBase(pSceneManager)
	, m_pWidget				( nullptr )
	, m_IsChangeScene		( false )
{
	m_pWidget	= std::make_unique< CTitleWidget >();
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
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CTitle::Update()
{
	CSoundManager::ThreadPlayBGM("TestBGM");

	if (CFade::GetIsFade() == true) return;

	m_pWidget->Update();
	//�V�[���؂�ւ�.
	ChangeScene();

}

//============================.
//	�`��֐�.
//============================.
void CTitle::Render()
{
	if ( m_pWidget == nullptr ) return;
	m_pWidget->Render();
}

//============================.
	// �V�[���؂�ւ��֐�.
//============================.
void CTitle::ChangeScene()
{
	// �{�^���������ꂽ.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enSEPARATED)
	{
		if (m_IsChangeScene == true) return;
		CFade::SetFadeIn();
		CSoundManager::PlaySE("DeterminationSE");
		m_IsChangeScene = true;
	}

	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	// �ǂ̃V�[���ɑJ�ڂ��邩.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		CSoundManager::StopBGMThread("TestBGM");
		m_pSceneManager->NextSceneMove();
		break;
	case CTitleWidget::ESelectState::End:
		CSoundManager::StopBGMThread("TestBGM");
		// �E�B���h�E�����.
		PostMessage(m_pSceneManager->m_hWnd, WM_CLOSE, 0, 0);
		break;
	default:
		break;
	}
}

