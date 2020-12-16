#include "Actor.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include "..\InvisibleWall\BoxStruct.h"

CActor::CActor()
	: m_pCollManager		( nullptr )
	, m_pBoxWall			( nullptr )
//	, m_HitStopCount		( 0 )
//	, m_HitStopTime			( 0 )
//	, m_IsHitStop			( false )
	, m_IsPossibleToHit		( false )
	, m_IsPlayCameraShake	( false )
{
}

CActor::~CActor()
{
}

// •Ç‚ÉÕ“Ë‚µ‚½‚©.
bool CActor::IsCrashedWall()
{
	if( m_pBoxWall == nullptr ) return false;
	if( m_pBoxWall->MaxPosition.x > m_vPosition.x && m_vPosition.x > m_pBoxWall->MinPosition.x &&
		m_pBoxWall->MaxPosition.y > m_vPosition.z && m_vPosition.z > m_pBoxWall->MinPosition.y ) return false;

	return true;
}

// •Ç‚ÉÕ“Ë‚µ‚½‚©.
bool CActor::IsCrashedWallX()
{
	if( m_pBoxWall == nullptr ) return false;
	if( m_pBoxWall->MaxPosition.x > m_vPosition.x && m_vPosition.x > m_pBoxWall->MinPosition.x ) return false;

	return true;
}

// •Ç‚ÉÕ“Ë‚µ‚½‚©.
bool CActor::IsCrashedWallZ()
{
	if( m_pBoxWall == nullptr ) return false;
	if( m_pBoxWall->MaxPosition.y > m_vPosition.z && m_vPosition.z > m_pBoxWall->MinPosition.y ) return false;

	return true;
}