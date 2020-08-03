#ifndef EACH_ITEM_EFFECT_STRUCT_H
#define EACH_ITEM_EFFECT_STRUCT_H

// �e�A�C�e���̌��ʍ\����.
struct stEachItemEffect
{
	// �񕜃A�C�e��.
	float LifeRecoveryValue;	// �񕜗�.

	// ����\�͉񕜑��xUP�A�C�e��.
	float SPRecoveryValue;	// ����\�͉񕜗�.
	float SPRecoveryTime;	// ����\�͉񕜎���.

	// �U����UP�A�C�e��.
	float AttackPower;	// �U����.
	float AttackUpTime;	// �U����UP����.

	// �ړ����xUP�A�C�e��.
	float MovePower;	// �ړ����x.
	float MoveUpTime;	// �ړ����xUP����.

	stEachItemEffect()
		: LifeRecoveryValue	( 0.0f )
		, SPRecoveryValue	( 0.0f )
		, SPRecoveryTime	( 0.0f )
		, AttackPower		( 0.0f )
		, AttackUpTime		( 0.0f )
		, MovePower			( 0.0f )
		, MoveUpTime		( 0.0f )
	{}
} typedef SEachItemEffect;

#endif	// #ifndef EACH_ITEM_EFFECT_STRUCT_H.