/**
* @file Alien.h.
* @brief 宇宙人基底クラス.
* @author 福田玲也.
*/
#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"
#include "AlienParam.h"
#include "..\..\Item\ItemList.h"

#include <random>

class CArm;				// アームクラス.
class CEffectManager;	// エフェクトクラス.

/**********************************
*	宇宙人基底クラス,
**/
class CAlien : public CCharacter
{
protected:
	inline static const char* HIT_EEFECT_NAME		= "hiteffekt";		// ヒットエフェクト.
	inline static const char* SPAWN_EFFECT_NAME		= "spawnEffect";	// スポーンエフェクト.
	inline static const char* DEAD_EFFECT_NAME		= "uvtest";			// 死亡エフェクト.
	inline static const char* ATTACK_EFFECT_NAME	= "b_attackeffkt";	// 攻撃エフェクト.
	static constexpr float TOLERANCE_RADIAN			= static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.
	static constexpr float BARRIER_HIT_MOVE_SPEED	= -5.0f;									// バリアと衝突時の移動速度.
	static constexpr float SCALE_MIN				= 0.0f;										// モデルの最小値.
	static constexpr float SCALE_MAX				= 1.0f;										// モデルの最大値.
	static constexpr float DEATH_SCALE_PI			= 6.0f*static_cast<float>(D3DX_PI);			// 死亡時のスケールの範囲.
	static constexpr float POSITION_HEIGHT_MIN		= 0.0f;										// 座標の最小高さ.

	static constexpr float HIT_EFFECT_SCALE			= 2.0f;				// ヒットエフェクトのサイズ.
	static constexpr float HIT_EFFECT_HEIGHT		= 4.0f;				// ヒットエフェクトの高さ.

	static constexpr float SPAWN_EFFECT_ADD_SIZE	= 0.2f;				// スポーンエフェクト調整加算値.

	static constexpr int	POSSIBLE_TIME = 80;
	static constexpr int	MAX_HIT_COUNT = 3;

	const SAlienParam*	pPARAMETER;	// パラメータのポインタ : 外部から取得.

public:
	CAlien();
	CAlien( const SAlienParam* pParam );
	virtual ~CAlien();

	// エフェクトの描画.
	virtual void EffectRender() override;

	// 相手座標の設定.
	virtual void SetTargetPos( CActor& actor ) override;
	// ベクトルの取得.
	virtual void SetVector( const D3DXVECTOR3& vec ) override;
	// スポーン.
	virtual bool Spawn( const D3DXVECTOR3& spawnPos ) = 0;
	// アニメーションを止める.
	virtual void StopAnimation() override;
	// アニメーションを再開する.
	virtual void ResumeAnimation() override;

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
	// 連れ去っているかどうか.
	inline bool IsAbduct() const { return m_NowState == alien::EAlienState::Abduct; }
	// どのアイテムを持っているか取得.
	inline EItemList GetAnyItem() const { return m_HasAnyItem; }
	// ほかの宇宙人が連れ去っているか設定.
	inline void SetOtherAbduct( bool* pisAbduct ){ m_pIsAlienOtherAbduct = pisAbduct; }
	// 連れ去るUFOの座標の取得.
	inline void SetAbductUFOPosition( D3DXVECTOR3* pos ){ m_pAbductUFOPosition = pos; }
	// アイテムの設定.
	inline void SetItem( const EItemList& item ){ m_HasAnyItem = item; }
	// 爆発するか.
	inline bool IsExplosion() const { return m_IsExplosion; }
	// 死亡かどうか.
	inline bool IsDead() const { return m_NowState == alien::EAlienState::Death; }
	// 消去するかどうか.
	inline bool IsDelete() const { return m_IsDelete; }
	// マザーシップに昇っているか.
	inline bool IsRisingMotherShip() const { return m_NowState == alien::EAlienState::RisingMotherShip; }

protected:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// 女の子の座標を設定.
	void SetGirlPos( CActor& actor );
	// 座標設定関数.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

	// 移動ベクトル設定関数.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// 目的の座標へ回転.
	void TargetRotation();
	// 移動関数.
	virtual void VectorMove( const float& moveSpeed );
	// 待機関数.
	virtual void WaitMove();

	// スポーン中.
	virtual void Spawning();
	// 移動.
	virtual void Move() override;
	// 拐う.
	virtual void Abduct();
	// ノックバック.
	virtual void KnockBack();
	// 怯み.
	virtual void Fright();
	// 死亡.
	virtual void Death();
	// 逃げる.
	virtual void Escape();
	// マザーシップに昇っている.
	virtual void RisingMotherShip();

	// 女の子との当たり判定.
	void GirlCollision( CActor* pActor );
	// バリアとの当たり判定.
	void BarrierCollision( CActor* pActor );

	// アニメーションコントローラーの取得.
	bool GetAnimationController();

	// アニメーションフレームの設定.
	virtual bool SetAnimFrameList() override;
	// エフェクトの設定.
	virtual bool EffectSetting();

protected:
	std::unique_ptr<CArm>		m_pArm;			// アームクラス.
	LPD3DXANIMATIONCONTROLLER	m_pAC;			// アニメーションコントローラー.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;
	D3DXVECTOR3				m_TargetPosition;			// 女の子の座標.
	D3DXVECTOR3				m_TargetRotation;			// 目標の回転情報.
	D3DXVECTOR3				m_KnockBackVector;			// ノックバックの移動ベクトル.
	D3DXVECTOR3				m_BeforeMoveingPosition;	// 移動前の座標.
	D3DXVECTOR3*			m_pAbductUFOPosition;		// UFOの座標.
	alien::EAlienState		m_NowState;					// 現在の状態.
	alien::EMoveState		m_NowMoveState;				// 現在の移動状態.
	EItemList				m_HasAnyItem;				// どのアイテムを持っているか.
	float					m_LifePoint;				// 体力.
	float					m_MoveSpeed;				// 移動速度.
	float					m_DeathScale;				// 死亡時の大きさ.
	float					m_DeathCount;				// 死亡カウント.
	int						m_KnockBackCount;			// ノックバックカウント.
	int						m_FrightCount;				// 怯みカウント.
	int						m_HitCount;					// ヒットカウント.
	int						m_WaitCount;				// 待機カウント.
	int						m_PossibleCount;			// 当たり判定無効カウント.
	bool*					m_pIsAlienOtherAbduct;		// 他の宇宙人が連れ去っているかどうか.
	bool					m_IsFirght;					// 怯み状態か.
	bool					m_IsBarrierHit;				// バリアに当たっているか.
	bool					m_IsRisingMotherShip;		// マザーシップに上っているか.
	bool					m_IsExplosion;				// 爆発するか.
	bool					m_IsDelete;					// 消去するかどうか.

};

#endif	// #ifndef ALIEN_H.