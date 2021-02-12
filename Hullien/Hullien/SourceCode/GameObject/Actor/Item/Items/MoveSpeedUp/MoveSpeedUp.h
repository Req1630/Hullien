/**
* @file MoveSpeedUp.h.
* @brief 移動速度アップアイテムクラス.
* @author 福田玲也.
*/
#ifndef MOVE_SPEED_UP_H
#define MOVE_SPEED_UP_H

#include "..\..\ItemBase.h"

/********************************************
*	移動速度アップアイテムクラス.
**/
class CMoveSpeedUpItem : public CItemBase
{
	inline static const char* MODEL_NAME					= "item speed";	// モデル名.
	inline static const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_speed";	// エフェクト名.
	inline static const char* HIT_EFFECT_NAME				= "item_get_speed";	// エフェクト名.

public:
	CMoveSpeedUpItem();
	CMoveSpeedUpItem(
		const float* pMoveValue,
		const float* pMoveTime,
		const CItemBase::SParameter* pParam );
	virtual ~CMoveSpeedUpItem();

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
	const float* pMOVE_VALUE;	// 移動力(外部からポインタで取得する).
	const float* pMOVE_UP_TIME;	// 移動力UP時間(外部からポインタで取得する).
};

#endif	// #ifndef MOVE_SPEED_UP_H.