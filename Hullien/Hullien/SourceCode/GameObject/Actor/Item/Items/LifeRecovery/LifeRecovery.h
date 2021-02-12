/**
* @file LifeRecovery.h.
* @brief 体力回復アイテムクラス.
* @author 福田玲也.
*/
#ifndef LIFE_RECOVERY_H
#define LIFE_RECOVERY_H

#include "..\..\ItemBase.h"

/*********************************************
*	体力回復アイテムクラス.
**/
class CLifeRecoveryItem : public CItemBase
{
	inline static const char* MODEL_NAME					= "item cure";	// モデル名.
	inline static const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_cure";	// エフェクト名.
	inline static const char* HIT_EFFECT_NAME				= "item_get_cure";	// エフェクト名.
public:
	CLifeRecoveryItem();
	CLifeRecoveryItem( 
		const float* pLifeRecoveryValue, 
		const CItemBase::SParameter* pParam );
	virtual ~CLifeRecoveryItem();

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
	const float* pLIFE_RECOVERY_VALUE;	// 回復力(外部からポインタで取得する).
};

#endif	// #ifndef LIFE_RECOVERY_H.