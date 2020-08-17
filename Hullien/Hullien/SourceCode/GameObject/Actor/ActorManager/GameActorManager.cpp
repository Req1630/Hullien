#include "GameActorManager.h"
#include "..\Character\Player\Player.h"
#include "..\Character\Girl\Girl.h"
#include "..\Character\Alien\AlienManager\AlienManager.h"
#include "..\Barrier\Barrier.h"
#include "..\Item\ItemManager\ItemManager.h"
#include "..\..\GroundStage\GroundStage.h"
#include "..\..\Widget\SceneWidget\GameWidget\Warning\Warning.h"

CGameActorManager::CGameActorManager()
	: m_pGroundStage	( nullptr )
	, m_pPlayer			( nullptr )
	, m_pGirl			( nullptr )
	, m_pAlienManager	( nullptr )
	, m_pItemManager	( nullptr )
	, m_pBarrier		( nullptr )
	, m_pWarning		(nullptr)
	, m_ObjPositionList	()
	, m_ObjPosListCount	( 0 )
{
	m_pGroundStage	= std::make_shared<CGroundStage>();
	m_pPlayer		= std::make_shared<CPlayer>();
	m_pGirl			= std::make_shared<CGirl>();
	m_pAlienManager	= std::make_shared<CAlienManager>();
	m_pItemManager	= std::make_shared<CItemManager>();
	m_pBarrier		= std::make_shared<CBarrier>();
	m_pWarning = std::make_shared<CWarning>();
}

CGameActorManager::~CGameActorManager()
{
}

// �������֐�.
bool CGameActorManager::Init()
{
	if( m_pGroundStage->Init()	== false ) return false;	// �n�ʂ̏�����.
	if( m_pPlayer->Init()		== false ) return false;	// �v���C���[�̏�����.
	if( m_pGirl->Init()			== false ) return false;	// ���̎q�̏�����.
	if( m_pAlienManager->Init()	== false ) return false;	// �F���l�Ǘ��̏�����.
	if( m_pItemManager->Init()	== false ) return false;	// �A�C�e���Ǘ��̏�����.
	if (m_pWarning->Init() == false) return false;

	return true;
}

// �X�V�֐�.
void CGameActorManager::Update()
{
	m_ObjPosListCount = 0;	// �J�E���g�̏�����.

	// �v���C���[�̍X�V.
	m_pPlayer->Update();
	SetPositionList( m_pPlayer.get() );	// ���W���X�g�̐ݒ�.

	// �o���A���g���邩.
	if( m_pPlayer->IsSpecialAbility() == true ) m_pBarrier->Init();	// �o���A�̏�����.

	// ���̎q�̍X�V.
	m_pGirl->Update();
	SetPositionList( m_pGirl.get() );	// ���W���X�g�̐ݒ�.
	m_pBarrier->SetTargetPos( *m_pGirl.get() );	// ���̎q�̍��W���擾.

	// ���̎q���߂܂�����.
//	if ( m_pGirl->IsDanger() == true ) m_pWarning->Init();	// �x���̏�����.

	// �o���A�̍X�V.
	m_pBarrier->Update();

	// �F���l�̍X�V.
	m_pAlienManager->Update( 
		[&]( CActor* pActor )
		{
			// ���W�̐ݒ�.
			pActor->SetTargetPos( *m_pPlayer.get() );
			pActor->SetTargetPos( *m_pGirl.get() );

			// �F���l�̍X�V����.
			pActor->Update();
			SetPositionList( pActor );	// ���W���X�g�̐ݒ�.

			// �v���C���[�A���̎q�̓����蔻��.
			m_pPlayer->Collision( pActor );
			m_pGirl->Collision( pActor );

			// �F���l�̓����蔻��.
			pActor->Collision( m_pPlayer.get() );
			pActor->Collision( m_pGirl.get() );
			pActor->Collision( m_pBarrier.get() );
		} );
	// �A�C�e�����X�g������΃A�C�e���𗎂Ƃ�.
	m_pItemManager->Drop( m_pAlienManager->GetDropItemList() );
	// �A�C�e���̍X�V.
	m_pItemManager->Update( 
		[&]( CActor* pActor )
		{
			pActor->Update();					// �A�C�e���̍X�V.
			SetPositionList( pActor );			// ���W���X�g�̐ݒ�.
			pActor->Collision( m_pGirl.get() );	// �A�C�e���̓����蔻��.
		} );

	// �x���̍X�V.
	m_pWarning->SetGirlState( m_pGirl.get() );		// ���̎q�̏��ݒ�.
	m_pWarning->Update();							// ���̎q�̏��ݒ�.
}

// �`��֐�.
void CGameActorManager::Render()
{
	m_pGroundStage->Render();	// �X�e�[�W�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pGirl->Render();			// ���̎q�̕`��.
	m_pAlienManager->Render();	// �F���l�B�̕`��.
	m_pItemManager->Render();	// �A�C�e���̕`��.
	m_pBarrier->Render();		// �o���A�̕`��.
	m_pWarning->Render();		//�x���̕`��.
}

// ���W���X�g�̐ݒ�.
void CGameActorManager::SetPositionList( CActor* pActor )
{
	if( pActor == nullptr ) return;
	m_ObjPosListCount++;	// �I�u�W�F�N�g���̉��Z.
	if( static_cast<int>(m_ObjPositionList.size()) < m_ObjPosListCount ){
		// ���X�g����ǉ�.
		m_ObjPositionList.emplace_back( pActor->GetObjectTag(), pActor->GetPosition() );
	} else {
		m_ObjPositionList[m_ObjPosListCount-1] = { pActor->GetObjectTag(), pActor->GetPosition() };
	}
}