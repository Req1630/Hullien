#include "BuletManager.h"

STG::CBulletManager::CBulletManager()
	: CBulletManager	( SBulletManagerParam() )
{
}

STG::CBulletManager::CBulletManager( const SBulletManagerParam& param )
	: PARAMETER					( param )
	, m_pBullets				()
	, m_BulletColor				( 0.0f, 0.0f, 0.0f )
	, m_BulletAddAngle			( param.BulletAngle )
	, m_BulletAngleAccValue		( 0.0f )
	, m_BulletAngleAddAccValue	( param.BulletAngleAddAccValue )
	, m_ShotAngle				( 0.0f )
	, m_ShotCount				( 0 )
	, m_NowShotBulletCount		( 0 )
	, m_IsShotEnd				( false )
{
}

STG::CBulletManager::~CBulletManager()
{
}

// �������֐�.
bool STG::CBulletManager::Init()
{
	if( BulletInit() == false ) return false;
	// �Փˎ��A�e���������ǂ����ݒ�.
	if( PARAMETER.BulletCollDisappear == (int)true ) for( auto& b : m_pBullets ) b->SetCollDisappear();
	// �e�̐F���擾.
	m_BulletColor =
	{ 
		1.0f,
		PARAMETER.BulletCollDisappear*BULLET_COLOR, 
		PARAMETER.BulletCollDisappear*BULLET_COLOR
	};
	return true;
}

// �X�V�֐�.
void STG::CBulletManager::Update()
{
	for( auto& b : m_pBullets ) b->Update();
}

// �`��֐�.
void STG::CBulletManager::Render()
{
	for( auto& b : m_pBullets ){
		b->SetColor( m_BulletColor );
		b->Render();
	}
}

// �����蔻��.
void STG::CBulletManager::Collision( STG::CActor* pActor )
{
	// �e�̐����������蔻����s��.
	for( auto& b : m_pBullets ) b->Collision( pActor );
}

// �e������.
void STG::CBulletManager::Shot()
{
	// �e�������I������̂ŏI��.
	if( m_IsShotEnd == true ) return;

	m_ShotCount++;	// �V���b�g�J�E���g�����Z.
	// �V���b�g�J�E���g�����l�ɒB����΁A�e������.
	if( m_ShotCount != PARAMETER.ShotIntervalFrame ) return;

	float angle = m_ShotAngle;	// �e�̊p�x��ݒ�.

	// �������e���e�̍ő吔�ȉ��Ȃ�.
	if( m_NowShotBulletCount < PARAMETER.BulletCountMax ){
		// nWay�������[�v.
		for( int i = 0; i < PARAMETER.AnyBulletCountMax; i++ ){
			// �e������.
			BulletShot( angle, PARAMETER.BulletSpeed );
			angle += PARAMETER.ShotAngle;	// nWay�̊p�x�����Z.
			m_NowShotBulletCount++;			// ���݌������e�����Z.
		}
	} else {
		// �e���K��l�������I�����.
		m_IsShotEnd = true;
	}

	m_ShotCount = 0;
	m_ShotAngle += m_BulletAddAngle;	// �p�x�̉��Z.
	m_BulletAddAngle += m_BulletAngleAccValue;
	m_BulletAngleAccValue += m_BulletAngleAddAccValue;

	if( fabsf(m_BulletAddAngle) > fabsf(PARAMETER.BulletAngle) ){
		if( m_BulletAngleAccValue > 0.0 ){
			m_BulletAddAngle = fabsf(PARAMETER.BulletAngle);
			m_BulletAngleAddAccValue = -m_BulletAngleAddAccValue;
			m_BulletAngleAccValue = 0.0f;
		} else {
			m_BulletAddAngle = -fabsf(PARAMETER.BulletAngle);
			m_BulletAngleAddAccValue = -m_BulletAngleAddAccValue;
			m_BulletAngleAccValue = 0.0f;
		}
	}
}

// �����_���Ō���.
void STG::CBulletManager::RandomShot()
{
}

// �����p�x�̐ݒ�.
void STG::CBulletManager::SetInitAngle( const D3DXVECTOR3& pos, const D3DXVECTOR3& targetPos )
{
	m_ShotAngle = atan2(
		pos.x - targetPos.x,
		pos.z - targetPos.z );
	m_ShotAngle -=
		static_cast<float>(PARAMETER.AnyBulletCountMax-1) * 0.5f * PARAMETER.ShotAngle;	
}

// �e������.
void STG::CBulletManager::BulletShot( const float& rot, const float& moveSpeed )
{
	for( auto& b : m_pBullets ){
		if( b->Shoot( m_vPosition, rot, moveSpeed ) == true ) return;
	}
	return;
}

// �e�̏�����.
bool STG::CBulletManager::BulletInit()
{
	m_pBullets.resize( PARAMETER.BulletCountMax );
	for( auto& b : m_pBullets ){
		b = std::make_shared<CBullet>( MODEL_NAME );
		if( b->Init() == false ) return false;
	}
	return true;
}