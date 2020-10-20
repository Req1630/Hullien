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
	} typedef SEnemyParam;

	// �G�̃p�����[�^�ԍ�.
	enum enEnemyParamNo
	{
		EEnemyParamNo_None,

		EEnemyParamNo_SpawnTime,			// �o������.
		EEnemyParamNo_PositionX,			// ���W : X.
		EEnemyParamNo_pText,				// �e�L�X�g.
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