#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ClearWidget\ClearWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase		   ( pSceneManager )
	, m_pClearWidget	   ( nullptr )
	, m_pFade			   ( nullptr )
	, m_IsChangeScene ( false )
{
	m_pClearWidget = std::make_unique<CClearWidget>();
	m_pFade = std::make_shared<CFade>();
	m_pFade->SetFadeOut();
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

	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGameClear::Update()
{
	m_pFade->Update();

	if (m_pFade->GetFadeState() == CWidget::EFadeState::Out
		&& m_pFade->GetIsFade() == true) return;
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
	if (GetAsyncKeyState(VK_RETURN) & 0x0001) {
		m_IsChangeScene = true;
		m_pFade->SetFadeIn();
	}
	// �t�F�[�h�C����Ԃ��t�F�[�h���Ȃ珈�����Ȃ�.
	if (m_pFade->GetFadeState() != CWidget::EFadeState::In) return;
	if (m_pFade->GetIsFade() == true) return;
	m_pSceneManager->NextSceneMove();
}
