#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ClearWidget\ClearWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase			( pSceneManager )
	, m_pClearWidget		( nullptr )
	, m_IsChangeScene		( false )
{
	m_pClearWidget = std::make_unique<CClearWidget>();
	CFade::SetFadeOut();
}

CGameClear::~CGameClear()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGameClear::Load()
{
	if ( m_pClearWidget->Init() == false ) return false;
	CSoundManager::GetInstance()->m_fMaxBGMVolume = 0.7f;
	CSoundManager::SetBGMVolume("ClearBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);

	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGameClear::Update()
{
	CSoundManager::ThreadPlayBGM("ClearBGM");

	if (CFade::GetFadeState() == CFade::EFadeState::Out
		&& CFade::GetIsFade() == true) return;
	m_pClearWidget->Update();
	ChangeScene();
}

//============================.
//	�`��֐�.
//============================.
void CGameClear::Render()
{
	if (m_pClearWidget == nullptr) return;
	m_pClearWidget->Render();
}

//============================.
// �V�[���؂�ւ��֐�.
//============================.
void CGameClear::ChangeScene()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if(m_IsChangeScene == true) return;
		CFade::SetFadeIn();
		CSoundManager::PlaySE("Determination");
		CSoundManager::FadeOutBGM("ClearBGM");
		m_IsChangeScene = true;
	}

	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if(CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	if(CSoundManager::GetBGMVolume("ClearBGM") > 0.0f) return;
	while(CSoundManager::StopBGMThread("ClearBGM")== false);
	m_pSceneManager->NextSceneMove();
}
