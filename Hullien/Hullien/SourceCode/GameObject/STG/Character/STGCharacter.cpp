#include "STGCharacter.h"
#include "..\Bullet\STGBullet.h"

STG::CCharacter::CCharacter()
	: m_pBullets	()
	, m_ShotCount	( 0 )
{
}

STG::CCharacter::~CCharacter()
{
}

// �e�̏�����.
bool STG::CCharacter::BulletInit( 
	std::vector<std::shared_ptr<CBullet>>& bullets,
	const int& bulletCount,
	const char* modelName )
{
	m_pBullets.resize( bulletCount );
	for( auto& b : bullets ){
		b = std::make_shared<CBullet>( modelName );
		if( b->Init() == false ) return false;
	}
	return true;
}

// �e�̍X�V.
void STG::CCharacter::BulletUpdate()
{
	for( auto& b : m_pBullets ) b->Update();
}

// �e�̕`��.
void STG::CCharacter::BulletRender()
{
	for( auto& b : m_pBullets ) b->Render();
}

// �e������.
void STG::CCharacter::BulletShot( const float& rot, const float& moveSpeed )
{
	for( auto& b : m_pBullets ){
		if( b->Shoot( m_vPosition, rot, moveSpeed ) == true ) return;
	}
}