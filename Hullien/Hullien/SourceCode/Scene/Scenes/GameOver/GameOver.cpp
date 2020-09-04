#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameOverWidget\GameOverWidget.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

CGameOver::CGameOver( CSceneManager* pSceneManager )
	: CSceneBase				( pSceneManager )
	, m_pGameOverWidget	( nullptr )
{
	m_pGameOverWidget = std::make_unique<CGameOverWidget>();
	CFade::SetFadeOut();
}

CGameOver::~CGameOver()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGameOver::Load()
{
	if (m_pGameOverWidget->Init() == false) return false;
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGameOver::Update()
{
	CSoundManager::ThreadPlayBGM("GameOverBGM");

	if (CFade::GetFadeState() == CFade::EFadeState::Out
		&& CFade::GetInstance()->GetIsFade() == true) return;
	m_pGameOverWidget->Update();


	// �ȉ��V�[���J��.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if (CFade::GetIsFade() == true) return;
		CFade::GetInstance()->SetFadeIn();
		CSoundManager::PlaySE("DeterminationSE");
	}

	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetIsFade() == true) return;
	CSoundManager::StopBGMThread("GameOverBGM");
	m_pSceneManager->NextSceneMove();

}

//============================.
//	�`��֐�.
//============================.
void CGameOver::Render()
{
	m_pGameOverWidget->Render();
}