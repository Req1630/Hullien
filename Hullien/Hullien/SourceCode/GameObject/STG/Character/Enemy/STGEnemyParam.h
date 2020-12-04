#ifndef STG_ENEMY_PARAM_H
#define STG_ENEMY_PARAM_H

#include <string>

namespace STG
{
	// 敵のパラメータ.
	struct stEnemyParam
	{
		float		SpawnTime;			// 出現時間.
		float		PositionX;			// 座標 : X.
		std::string	Text;				// テキスト.
		float		TextSize;			// テキストサイズ.
		float		LifePoint;			// ライフ.
		float		MoveSpeed;			// 移動速度.

		stEnemyParam()
			: SpawnTime				( 0.0f )
			, PositionX				( 0.0f )
			, Text					( "" )
			, TextSize				( 0.0f )
			, LifePoint				( 0.0f )
			, MoveSpeed				( 0.0f )
		{}
	} typedef SEnemyParam;

	// 現在の状態.
	enum class enEnemyState : unsigned char
	{
		None,

		Spawn,	// スポーン.
		Move,	// 移動.
		Shot,	// 撃つ.
		Escape,	// 逃げる.
		Dead,	// 死亡.

		Max,
	} typedef EEnemyState;

	// 敵のパラメータ番号.
	enum enEnemyParamNo : unsigned char
	{
		EEnemyParamNo_None,

		EEnemyParamNo_SpawnTime,			// 出現時間.
		EEnemyParamNo_PositionX,			// 座標 : X.
		EEnemyParamNo_Text,					// テキスト.
		EEnemyParamNo_TextSize,				// テキストサイズ.
		EEnemyParamNo_LifePoint,			// 体力.
		EEnemyParamNo_MoveSpeed,			// 移動速度.
		EEnemyParamNo_BulletIndex,			// 使用する弾の番号.

		EEnemyParamNo_Max,

		EEnemyParamNo_Start	= EEnemyParamNo_SpawnTime,
		EEnemyParamNo_End	= EEnemyParamNo_BulletIndex,
	} typedef EEnemyParamNo;
};
#endif	// #ifndef STG_ENEMY_PARAM_H.