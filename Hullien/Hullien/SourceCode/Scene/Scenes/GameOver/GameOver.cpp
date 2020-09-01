#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameOverWidget\GameOverWidget.h"

CGameOver::CGameOver( CSceneManager* pSceneManager )
	: CSceneBase				( pSceneManager )
	, m_pGameOverWidget	( nullptr )
{
	m_pGameOverWidget = std::make_unique<CGameOverWidget>();
	CFade::GetInstance()->SetFadeOut();
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
	CFade::GetInstance()->Update();

	if (CFade::GetInstance()->GetFadeState() == CFade::EFadeState::Out
		&& CFade::GetInstance()->GetIsFade() == true) return;
	m_pGameOverWidget->Update();


	// �ȉ��V�[���J��.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		CFade::GetInstance()->SetFadeIn();
	}

	if (CFade::GetInstance()->GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetInstance()->GetIsFade() == true) return;
	m_pSceneManager->NextSceneMove();

}

//============================.
//	�`��֐�.
//============================.
void CGameOver::Render()
{
	m_pGameOverWidget->Render();
}