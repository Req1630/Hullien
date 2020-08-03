#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ClearWidget\ClearWidget.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
	m_pClearWidget = std::make_unique<CClearWidget>();
}

CGameClear::~CGameClear()
{
}

//============================.
//	読込関数.
//============================.
bool CGameClear::Load()
{

	if ( m_pClearWidget->Init() == false ) return false;

	return true;
}

//============================.
//	更新関数.
//============================.
void CGameClear::Update()
{
	m_pClearWidget->Update();

	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	描画関数.
//============================.
void CGameClear::Render()
{
	if (m_pClearWidget == nullptr) return;
	m_pClearWidget->Render();
}