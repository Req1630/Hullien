#ifndef BULLET_MANAGER_PARAM_H
#define BULLET_MANAGER_PARAM_H

#include "..\..\Character\Enemy\STGEnemyParam.h"

namespace STG
{
	// ’eŠÇ—ƒpƒ‰ƒ[ƒ^.
	struct stBulletManagerParam
	{
		float		BulletSpeed;			// ’e‚Ì‘¬“x.
		int			BulletCountMax;			// ’e”.
		int			AnyBulletCountMax;		// nWay”.
		float		BulletAngle;			// Šp“x.
		float		ShotAngle;				// nWayŠp“x.
		float		BulletAngleAddAccValue;	// Šp“x‚Ì‰Á‘¬‰ÁZ’l.
		int			ShotIntervalFrame;		// ’e‚ğŒ‚‚ÂŠÔŠu.
		int			BulletCollDisappear;	// ’e‚ªÕ“Ë‚µ‚½Û‚ÉÁ‚¦‚é‚©.

		stBulletManagerParam()
			: BulletSpeed				( 0.0f )
			, BulletCountMax			( 0 )
			, AnyBulletCountMax			( 0 )
			, BulletAngle				( 0.0f )
			, ShotAngle					( 0.0f )
			, BulletAngleAddAccValue	( 0.0f )
			, ShotIntervalFrame			( 0 )
			, BulletCollDisappear		( 0 )
		{}

		stBulletManagerParam operator = ( const STG::stEnemyParam p )
		{
			this->BulletSpeed			= p.BulletSpeed;
			this->BulletCountMax		= p.BulletCountMax;
			this->AnyBulletCountMax		= p.AnyBulletCountMax;
			this->BulletAngle			= p.BulletAngle;
			this->ShotAngle				= p.ShotAngle;
			this->ShotIntervalFrame		= p.ShotIntervalFrame;
			this->BulletCollDisappear	= p.BulletCollDisappear;
			
			return *this;
		}

	} typedef SBulletManagerParam;

	// ’e‚Ìƒpƒ‰ƒ[ƒ^”Ô†.
	enum enBulletManagerParamNo : unsigned char
	{
		EBulletManagerParamNo_None,

		EBulletManagerParamNo_BulletSpeed,				// ’e‚Ì‘¬“x.
		EBulletManagerParamNo_BulletCountMax,			// ’e‚ÌÅ‘å”.
		EBulletManagerParamNo_AnyBulletCountMax,		// •¡”’e‚ğŒ‚‚ÂÛ‚ÌÅ‘å”.
		EBulletManagerParamNo_BulletAngle,				// ’e‚ÌŠp“x.
		EBulletManagerParamNo_ShotAngle,				// Œ‚‚Â‚ÌŠp“x.
		EBulletManagerParamNo_BulletAngleAddAccValue,	// Šp“x‚Ì‰Á‘¬‰ÁZ’l.
		EBulletManagerParamNo_ShotIntervalFrame,		// ’e‚ğŒ‚‚ÂŠÔŠu.
		EBulletManagerParamNo_BulletCollDisappear,		// ’e‚ªÕ“Ë‚µ‚½Û‚ÉÁ‚¦‚é‚©.

		EBulletManagerParamNo_Max,

		EBulletManagerParamNo_Start	= EBulletManagerParamNo_BulletSpeed,
		EBulletManagerParamNo_End	= EBulletManagerParamNo_BulletCollDisappear,
	} typedef EBulletManagerParamNo;
};

#endif	// #ifndef BULLET_MANAGER_PARAM_H.