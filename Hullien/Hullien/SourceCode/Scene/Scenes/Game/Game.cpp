#include "..\SceneList.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\Actor\Character\Player\Player.h"
#include "..\..\..\GameObject\Actor\Character\Girl\Girl.h"
#include "..\..\..\GameObject\Actor\Character\Alien\AlienManager\AlienManager.h"
#include "..\..\..\GameObject\Actor\Item\ItemManager\ItemManager.h"

CGame::CGame( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pGroundStage	( nullptr )
	, m_pPlayer			( nullptr )
	, m_pGirl			( nullptr )
	, m_pAlienManager	( nullptr )
	, m_pItemManager	( nullptr )
{
	m_pGroundStage = std::make_shared<CGroundStage>();
	m_pPlayer = std::make_shared<CPlayer>();
	m_pGirl = std::make_shared<CGirl>();
	m_pAlienManager = std::make_shared<CAlienManager>();
	m_pItemManager = std::make_shared<CItemManager>();
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
	if( m_pGirl->Init() == false ) return false;
	if( m_pAlienManager->Init() == false ) return false;
	if( m_pItemManager->Init() == false ) return false;
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CGame::Update()
{
	m_pPlayer->Update();	// �v���C���[�̍X�V.
	m_pGirl->Update();		// ���̎q�̍X�V.

	// �F���l�̍X�V.
	m_pAlienManager->Update( 
		m_pPlayer.get(),
		m_pGirl.get(),
		[&]( CActor* pActor )
		{
			// �v���C���[�A���̎q�̓����蔻��.
			m_pPlayer->Collision( pActor );
			m_pGirl->Collision( pActor );
		} );
	// �A�C�e�����X�g������΃A�C�e���𗎂Ƃ�.
	m_pItemManager->Drop( m_pAlienManager->GetDropItemList() );
	// �A�C�e���̍X�V.
	m_pItemManager->Update( m_pPlayer.get() );

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

	m_pGroundStage->Render();	// �X�e�[�W�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pGirl->Render();			// ���̎q�̕`��.
	m_pAlienManager->Render();	// �F���l�B�̕`��.
	m_pItemManager->Render();	// �A�C�e���̕`��.
}