/**
* @file BuletManagerParam.h.
* @brief 弾管理情報.
* @author 福田玲也.
*/
#ifndef BULLET_MANAGER_PARAM_H
#define BULLET_MANAGER_PARAM_H

#include "..\..\Character\Enemy\STGEnemyParam.h"

namespace STG
{
	// 弾管理パラメータ.
	struct stBulletManagerParam
	{
		float		BulletSpeed;			// 弾の速度.
		int			BulletCountMax;			// 弾数.
		int			AnyBulletCountMax;		// nWay数.
		float		BulletAngle;			// 角度.
		float		ShotAngle;				// nWay角度.
		float		BulletAngleAddAccValue;	// 角度の加速加算値.
		int			ShotIntervalFrame;		// 弾を撃つ間隔.
		int			BulletCollDisappear;	// 弾が衝突した際に消えるか.

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

	} typedef SBulletManagerParam;

	// 弾のパラメータ番号.
	enum enBulletManagerParamNo : unsigned char
	{
		EBulletManagerParamNo_None,

		EBulletManagerParamNo_BulletSpeed,				// 弾の速度.
		EBulletManagerParamNo_BulletCountMax,			// 弾の最大数.
		EBulletManagerParamNo_AnyBulletCountMax,		// 複数弾を撃つ際の最大数.
		EBulletManagerParamNo_BulletAngle,				// 弾の角度.
		EBulletManagerParamNo_ShotAngle,				// 撃つ時の角度.
		EBulletManagerParamNo_BulletAngleAddAccValue,	// 角度の加速加算値.
		EBulletManagerParamNo_ShotIntervalFrame,		// 弾を撃つ間隔.
		EBulletManagerParamNo_BulletCollDisappear,		// 弾が衝突した際に消えるか.

		EBulletManagerParamNo_Max,

		EBulletManagerParamNo_Start	= EBulletManagerParamNo_BulletSpeed,
		EBulletManagerParamNo_End	= EBulletManagerParamNo_BulletCollDisappear,
	} typedef EBulletManagerParamNo;
};

#endif	// #ifndef BULLET_MANAGER_PARAM_H.