#include "..\SceneList.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pGroundStage	( nullptr )
	, m_pPlayer			( nullptr )
	, m_pAlienManager	( nullptr )
{
	m_pGroundStage = std::make_shared<CGroundStage>();
	m_pPlayer = std::make_shared<CPlayer>();
	m_pAlienManager = std::make_shared<CAlienManager>();
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
	if( m_pPlayer->Init() == false ) return false;
	if( m_pAlienManager->Init() == false ) return false;
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGame::Update()
{
	// �v���C���[�̓����蔻��֐�.
	auto playerCollProc = [&]( CActor* pActor )
	{
		m_pPlayer->Collision( pActor );
	};

	m_pPlayer->Update();
	m_pAlienManager->Update( m_pPlayer.get(), playerCollProc );

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
	m_pPlayer->Render();
	m_pAlienManager->Render();
}