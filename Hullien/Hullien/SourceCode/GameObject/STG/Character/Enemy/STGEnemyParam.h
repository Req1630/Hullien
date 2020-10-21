#ifndef STG_ENEMY_PARAM_H
#define STG_ENEMY_PARAM_H

#include <string>

namespace STG
{
	// �G�̃p�����[�^.
	struct stEnemyParam
	{
		float		SpawnTime;			// �o������.
		float		PositionX;			// ���W : X.
		std::string	Text;				// �e�L�X�g.
		float		TextSize;			// �e�L�X�g�T�C�Y.
		float		LifePoint;			// ���C�t.
		float		MoveSpeed;			// �ړ����x.
		float		BulletSpeed;		// �e�̑��x.
		int			BulletCollDisappear;// �e���Փ˂����ۂɏ����邩.
		float		BulletAngle;		// �e�̊p�x.
		int			BulletCountMax;		// �e�̍ő吔.
		int			AnyBulletCountMax;	// �����e�����ۂ̍ő吔.
		int			ShotIntervalFrame;	// �e�����Ԋu�t���[��.
		int			ShotBulletCount;	// �ꔭ�ŏo��e��.
		float		ShotAngle;			// �����̊p�x.
		int			ShotNumber;			// �e�̔ԍ�.

		stEnemyParam()
			: SpawnTime				( 0.0f )
			, PositionX				( 0.0f )
			, Text					( "" )
			, TextSize				( 0.0f )
			, LifePoint				( 0.0f )
			, MoveSpeed				( 0.0f )
			, BulletSpeed			( 0.0f )
			, BulletCollDisappear	( 0 )
			, BulletAngle			( 0.0f )
			, BulletCountMax		( 0 )
			, AnyBulletCountMax		( 0 )
			, ShotIntervalFrame		( 0 )
			, ShotBulletCount		( 0 )
			, ShotAngle				( 0.0f )
			, ShotNumber			( 0 )
		{}
	} typedef SEnemyParam;

	// �G�̃p�����[�^�ԍ�.
	enum enEnemyParamNo
	{
		EEnemyParamNo_None,

		EEnemyParamNo_SpawnTime,			// �o������.
		EEnemyParamNo_PositionX,			// ���W : X.
		EEnemyParamNo_Text,					// �e�L�X�g.
		EEnemyParamNo_TextSize,				// �e�L�X�g�T�C�Y.
		EEnemyParamNo_LifePoint,			// �̗�.
		EEnemyParamNo_MoveSpeed,			// �ړ����x.
		EEnemyParamNo_BulletSpeed,			// �e�̑��x.
		EEnemyParamNo_BulletCollDisappear,	// �e���Փ˂����ۂɏ����邩.
		EEnemyParamNo_BulletAngle,			// �e�̊p�x.
		EEnemyParamNo_BulletCountMax,		// �e�̍ő吔.
		EEnemyParamNo_AnyBulletCountMax,	// �����e�����ۂ̍ő吔.
		EEnemyParamNo_ShotIntervalFrame,	// �e�����Ԋu�t���[��.
		EEnemyParamNo_ShotBulletCount,		// �ꔭ�ŏo��e��.
		EEnemyParamNo_ShotAngle,			// �����̊p�x.
		EEnemyParamNo_ShotNumber,			// �e�̔ԍ�.

		EEnemyParamNo_Max,

		EEnemyParamNo_Start	= EEnemyParamNo_SpawnTime,
		EEnemyParamNo_End	= EEnemyParamNo_ShotNumber,
	} typedef EEnemyParamNo;
};
#endif	// #ifndef STG_ENEMY_PARAM_H.