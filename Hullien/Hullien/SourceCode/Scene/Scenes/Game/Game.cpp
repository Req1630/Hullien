#include "..\SceneList.h"
#include "..\..\..\GameObject\Actor\ActorManager\GameActorManager.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_GameObjManager	( nullptr )
{
	m_GameObjManager = std::make_unique<CGameActorManager>();
}

CGame::~CGame()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGame::Load()
{
	if( m_GameObjManager->Init() == false ) return false;
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGame::Update()
{
	m_GameObjManager->Update();

#if 0	// ���̃V�[���ֈړ�.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
#endif	// #if 0.
}

//============================.
//	�`��֐�.
//============================.
void CGame::Render()
{
	m_GameObjManager->Render();
}