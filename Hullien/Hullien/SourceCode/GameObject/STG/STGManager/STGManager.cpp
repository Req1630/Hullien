#include "STGManager.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\Character\Player\STGPlayer.h"
#include "..\Character\Enemy\STGEnemyManager\STGEnemyManager.h"
#include "..\Bullet\STGBullet.h"

CSTGManager::CSTGManager()
	: m_pCamera			( nullptr )
	, m_pPlayer			( nullptr )
	, m_pEnemyManager	( nullptr )
{
	m_pCamera		= std::make_shared<CCamera>();
	m_pPlayer		= std::make_shared<STG::CPlayer>();
	m_pEnemyManager	= std::make_unique<STG::CEnemyManager>();
}

CSTGManager::~CSTGManager()
{
}

// �������֐�.
bool CSTGManager::Init()
{
	if( m_pPlayer->Init()		== false ) return false;
	if( m_pEnemyManager->Init()	== false ) return false;
	return true;
}

// �X�V�֐�.
void CSTGManager::Update()
{
	m_pPlayer->Controller();// �v���C���[�̑���.
	m_pPlayer->Update();	// �v���C���[�̍X�V.

	m_pEnemyManager->Update( [&]( STG::CActor* pEnemy )
	{
		// �v���C���[�̍��W��ݒ�.
		pEnemy->SetTargetPos( m_pPlayer->GetPosition() );

		// �G�̍X�V.
		pEnemy->Update();

		// �v���C���[�A�G�̓����蔻��.
		m_pPlayer->Collision( pEnemy );
		pEnemy->Collision( m_pPlayer.get() );

		// �e���m�̓����蔻��.
		for( auto& pb : m_pPlayer->GetBullets() ){
			for( auto& eb : pEnemy->GetBullets() ){
				eb->Collision( pb.get() );
			}
		}
	});

	m_pCamera->SetPosition( { 0.0f, 150.0f, 10.0f } );
	m_pCamera->SetLookPosition( { 0.0f, 0.0f, 0.0f } );
	CCameraManager::SetCamera( m_pCamera );
}

// �`��֐�.
void CSTGManager::Render()
{
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pEnemyManager->Render();	// �G�̕`��.
}