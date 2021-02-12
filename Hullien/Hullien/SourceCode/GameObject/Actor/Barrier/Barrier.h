/**
* @file Barrier.h.
* @brief 女の子に張るバリアクラス.
* @author 福田玲也.
*/
#ifndef BARRIER_H
#define BARRIER_H

#include "..\Actor.h"

class CEffectManager;	// エフェクトクラス.
class CEventActor;		// イベントアクタークラス.

/********************************
*	バリアクラス.
**/
class CBarrier : public CActor
{
public:
	CBarrier();
	virtual ~CBarrier();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// エフェクト描画関数.
	virtual void EffectRender() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& pActor ) override;
	// 動作状態か.
	inline bool IsActive() const { return m_IsActive; }

private:
	// 当たり判定の設定.
	bool ColliderSetting();

private:
	std::shared_ptr<CEffectManager>	m_pEffect;		// エフェクト.
	bool m_IsEffectPlay;		// エフェクトが再生されたか.
	bool m_IsActive;			// 動作状態か.
	float m_ActiveCount;		// 動作カウント.
	float m_CollSphereRadius;	// 当たり判定の半径.
#if _DEBUG
	float m_ResizeCollTime;		// 当たり判定の半径のリサイズする時間(デバッグ用).
#endif	// #if _DEBUG.
};

#endif	// #ifndef BARRIER_H.