#include "..\SceneList.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
}

CGameClear::~CGameClear()
{
}

//============================.
//	読込関数.
//============================.
bool CGameClear::Load()
{
	return true;
}

//============================.
//	更新関数.
//============================.
void CGameClear::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	描画関数.
//============================.
void CGameClear::Render()
{
}