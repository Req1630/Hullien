#include "..\SceneList.h"

CGameClear::CGameClear( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
}

CGameClear::~CGameClear()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGameClear::Load()
{
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGameClear::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	�`��֐�.
//============================.
void CGameClear::Render()
{
}