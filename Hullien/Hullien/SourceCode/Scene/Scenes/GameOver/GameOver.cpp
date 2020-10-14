#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameOverWidget\GameOverWidget.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\XAudio2\SoundManager.h"

CGameOver::CGameOver( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pGameOverWidget	( nullptr )
	, m_IsChangeScene	( false )
{
	m_pGameOverWidget = std::make_unique<CGameOverWidget>();
}

CGameOver::~CGameOver()
{
}

//============================.
//	読込関数.
//============================.
bool CGameOver::Load()
{
	CFade::SetFadeOut();
	if (m_pGameOverWidget->Init() == false) return false;
	CSoundManager::GetInstance()->m_fMaxBGMVolume = 0.7f;
	CSoundManager::SetBGMVolume("GameOverBGM", CSoundManager::GetInstance()->m_fMaxBGMVolume);

	return true;
}

//============================.
//	更新関数.
//============================.
void CGameOver::Update()
{
	CSoundManager::ThreadPlayBGM("GameOverBGM");

	if (CFade::GetFadeState() == CFade::EFadeState::Out
		&& CFade::GetIsFade() == true) return;
	m_pGameOverWidget->Update();


	// 以下シーン遷移.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		if(m_IsChangeScene == true) return;
		CFade::GetInstance()->SetFadeIn();
		CSoundManager::PlaySE("DeterminationSE");
		m_IsChangeScene = true;
	}

	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetIsFade() == true) return;
	CSoundManager::StopBGMThread("GameOverBGM");
	m_pSceneManager->NextSceneMove();

}

//============================.
//	描画関数.
//============================.
void CGameOver::Render()
{
	m_pGameOverWidget->Render();
}