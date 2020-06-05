#include "..\SceneList.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pGroundStage	( nullptr )
{
	m_pGroundStage = std::make_shared<CGroundStage>();
}

CGame::~CGame()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CGame::Load()
{
	if( m_pGroundStage->Init() == false ) return false;

	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGame::Update()
{


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
	m_pGroundStage->Render();
}