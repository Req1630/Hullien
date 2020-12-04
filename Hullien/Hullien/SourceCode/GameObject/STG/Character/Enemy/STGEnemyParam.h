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

		stEnemyParam()
			: SpawnTime				( 0.0f )
			, PositionX				( 0.0f )
			, Text					( "" )
			, TextSize				( 0.0f )
			, LifePoint				( 0.0f )
			, MoveSpeed				( 0.0f )
		{}
	} typedef SEnemyParam;

	// ���݂̏��.
	enum class enEnemyState : unsigned char
	{
		None,

		Spawn,	// �X�|�[��.
		Move,	// �ړ�.
		Shot,	// ����.
		Escape,	// ������.
		Dead,	// ���S.

		Max,
	} typedef EEnemyState;

	// �G�̃p�����[�^�ԍ�.
	enum enEnemyParamNo : unsigned char
	{
		EEnemyParamNo_None,

		EEnemyParamNo_SpawnTime,			// �o������.
		EEnemyParamNo_PositionX,			// ���W : X.
		EEnemyParamNo_Text,					// �e�L�X�g.
		EEnemyParamNo_TextSize,				// �e�L�X�g�T�C�Y.
		EEnemyParamNo_LifePoint,			// �̗�.
		EEnemyParamNo_MoveSpeed,			// �ړ����x.
		EEnemyParamNo_BulletIndex,			// �g�p����e�̔ԍ�.

		EEnemyParamNo_Max,

		EEnemyParamNo_Start	= EEnemyParamNo_SpawnTime,
		EEnemyParamNo_End	= EEnemyParamNo_BulletIndex,
	} typedef EEnemyParamNo;
};
#endif	// #ifndef STG_ENEMY_PARAM_H.