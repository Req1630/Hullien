#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

enum class enObjectTagList
{
	None,

	Player,				// プレイヤー.
	Girl,				// 女の子.

	Alien_A,			// 宇宙人A.
	Alien_B,			// 宇宙人B.
	Alien_C,			// 宇宙人C.
	Alien_D,			// 宇宙人D.

	LifeRecoveryItem,	// 体力回復アイテム.
	SPEffectTimeItem,	// 特殊能力回復速度UPアイテム.
	AttackUpItem,		// 攻撃力UPアイテム.
	MoveSpeedUpItem,	// 移動速度UPアイテム.

	LaserBeam,			// レーザービーム.
	Explosion,			// 爆発.
	Bariier,			// バリア.

	Max,

} typedef EObjectTag;

#endif	// #ifndef OBJECT_LIST_H.