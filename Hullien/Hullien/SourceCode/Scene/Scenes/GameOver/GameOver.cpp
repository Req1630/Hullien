#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameOverWidget\GameOverWidget.h"

CGameOver::CGameOver( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
	m_pGameOverWidget = std::make_unique<CGameOverWidget>();
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
	m_pGameOverWidget->Update();


	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	�`��֐�.
//============================.
void CGameOver::Render()
{
	m_pGameOverWidget->Render();
}