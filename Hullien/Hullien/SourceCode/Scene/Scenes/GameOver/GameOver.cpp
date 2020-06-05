#include "..\SceneList.h"

CGameOver::CGameOver( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
}

CGameOver::~CGameOver()
{
}

//============================.
//	読込関数.
//============================.
bool CGameOver::Load()
{
	return true;
}

//============================.
//	更新関数.
//============================.
void CGameOver::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	描画関数.
//============================.
void CGameOver::Render()
{
}