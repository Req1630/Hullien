#ifndef BULLET_MANAGER_PARAM_H
#define BULLET_MANAGER_PARAM_H

#include "..\..\Character\Enemy\STGEnemyParam.h"

namespace STG
{
	// �e�Ǘ��p�����[�^.
	struct stBulletManagerParam
	{
		float		BulletSpeed;			// �e�̑��x.
		int			BulletCountMax;			// �e��.
		int			AnyBulletCountMax;		// nWay��.
		float		BulletAngle;			// �p�x.
		float		ShotAngle;				// nWay�p�x.
		float		BulletAngleAddAccValue;	// �p�x�̉������Z�l.
		int			ShotIntervalFrame;		// �e�����Ԋu.
		int			BulletCollDisappear;	// �e���Փ˂����ۂɏ����邩.

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

	// �e�̃p�����[�^�ԍ�.
	enum enBulletManagerParamNo : unsigned char
	{
		EBulletManagerParamNo_None,

		EBulletManagerParamNo_BulletSpeed,				// �e�̑��x.
		EBulletManagerParamNo_BulletCountMax,			// �e�̍ő吔.
		EBulletManagerParamNo_AnyBulletCountMax,		// �����e�����ۂ̍ő吔.
		EBulletManagerParamNo_BulletAngle,				// �e�̊p�x.
		EBulletManagerParamNo_ShotAngle,				// �����̊p�x.
		EBulletManagerParamNo_BulletAngleAddAccValue,	// �p�x�̉������Z�l.
		EBulletManagerParamNo_ShotIntervalFrame,		// �e�����Ԋu.
		EBulletManagerParamNo_BulletCollDisappear,		// �e���Փ˂����ۂɏ����邩.

		EBulletManagerParamNo_Max,

		EBulletManagerParamNo_Start	= EBulletManagerParamNo_BulletSpeed,
		EBulletManagerParamNo_End	= EBulletManagerParamNo_BulletCollDisappear,
	} typedef EBulletManagerParamNo;
};

#endif	// #ifndef BULLET_MANAGER_PARAM_H.