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

// 初期化関数.
bool STG::CBulletManager::Init()
{
	if( BulletInit() == false ) return false;
	// 衝突時、弾を消すかどうか設定.
	if( PARAMETER.BulletCollDisappear == (int)true ) for( auto& b : m_pBullets ) b->SetCollDisappear();
	// 弾の色を取得.
	m_BulletColor =
	{ 
		1.0f,
		PARAMETER.BulletCollDisappear*BULLET_COLOR, 
		PARAMETER.BulletCollDisappear*BULLET_COLOR
	};
	return true;
}

// 更新関数.
void STG::CBulletManager::Update()
{
	for( auto& b : m_pBullets ) b->Update();
}

// 描画関数.
void STG::CBulletManager::Render()
{
	for( auto& b : m_pBullets ){
		b->SetColor( m_BulletColor );
		b->Render();
	}
}

// 当たり判定.
void STG::CBulletManager::Collision( STG::CActor* pActor )
{
	// 弾の数だけ当たり判定を行う.
	for( auto& b : m_pBullets ) b->Collision( pActor );
}

// 弾を撃つ.
void STG::CBulletManager::Shot()
{
	// 弾が撃ち終わったので終了.
	if( m_IsShotEnd == true ) return;

	m_ShotCount++;	// ショットカウントを加算.
	// ショットカウントが一定値に達すれば、弾を撃つ.
	if( m_ShotCount != PARAMETER.ShotIntervalFrame ) return;

	float angle = m_ShotAngle;	// 弾の角度を設定.

	// 撃った弾が弾の最大数以下なら.
	if( m_NowShotBulletCount < PARAMETER.BulletCountMax ){
		// nWay数分ループ.
		for( int i = 0; i < PARAMETER.AnyBulletCountMax; i++ ){
			// 弾を撃つ.
			BulletShot( angle, PARAMETER.BulletSpeed );
			angle += PARAMETER.ShotAngle;	// nWayの角度を加算.
			m_NowShotBulletCount++;			// 現在撃った弾を加算.
		}
	} else {
		// 弾が規定値分撃ち終わった.
		m_IsShotEnd = true;
	}

	m_ShotCount = 0;
	m_ShotAngle += m_BulletAddAngle;	// 角度の加算.
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

// ランダムで撃つ.
void STG::CBulletManager::RandomShot()
{
}

// 初期角度の設定.
void STG::CBulletManager::SetInitAngle( const D3DXVECTOR3& pos, const D3DXVECTOR3& targetPos )
{
	m_ShotAngle = atan2(
		pos.x - targetPos.x,
		pos.z - targetPos.z );
	m_ShotAngle -=
		static_cast<float>(PARAMETER.AnyBulletCountMax-1) * 0.5f * PARAMETER.ShotAngle;	
}

// 弾を撃つ.
void STG::CBulletManager::BulletShot( const float& rot, const float& moveSpeed )
{
	for( auto& b : m_pBullets ){
		if( b->Shoot( m_vPosition, rot, moveSpeed ) == true ) return;
	}
	return;
}

// 弾の初期化.
bool STG::CBulletManager::BulletInit()
{
	m_pBullets.resize( PARAMETER.BulletCountMax );
	for( auto& b : m_pBullets ){
		b = std::make_shared<CBullet>( MODEL_NAME );
		if( b->Init() == false ) return false;
	}
	return true;
}