/**
* @file AttackUp.h.
* @brief 攻撃力アップアイテムクラス.
* @author 福田玲也.
*/
#ifndef ATTACK_UP_H
#define ATTACK_UP_H

#include "..\..\ItemBase.h"

/*******************************************
*	攻撃力アップアイテムクラス.
**/
class CAttackUpItem : public CItemBase
{
	inline static const char* MODEL_NAME					= "item attack";	// モデル名.
	inline static const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_attack";	// エフェクト名.
	inline static const char* HIT_EFFECT_NAME				= "item_get_attack";	// エフェクト名.

public:
	CAttackUpItem();
	CAttackUpItem(
		const float* pAttackUpPower,
		const float* pAttackUpTime,
		const CItemBase::SParameter* pParam );
	virtual ~CAttackUpItem();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// エフェクトの描画.
	virtual void EffectRender() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& pActor ) override;

private:
	// 当たった際の効果を与える.
	virtual void GiveEffect( CActor* pActor ) override;
	// エフェクトの設定.
	virtual bool EffectSetting() override;

private:
	const float* pATTACK_UP_POWER;	// 攻撃力(外部からポインタで取得する).
	const float* pATTACK_UP_TIME;	// 攻撃力(外部からポインタで取得する).
};

#endif	// #ifndef ATTACK_UP_H.