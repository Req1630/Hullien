#ifndef EVENT_ALIEN_H
#define EVENT_ALIEN_H

#include "..\EventCharacter.h"

/****************************************
*	イベント用宇宙人クラス.
**/
class CEventAlien : public CEventCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.

protected:
	const float MODEL_ALPHA_MAX = 1.0f;	// モデルアルファの最大値.

public:
	// 宇宙人のパラメータ.
	struct stEventAlienParam
	{
		float						RotationalSpeed;			//回転速度.
		float						MoveSpeed;				// 移動速度.
		float						ModelAlphaAddValue;	// モデルのアルファ値の加算する値.
		float						WaitTime;					//待機時間.
		float						ResearchLenght;			// 再検査する際の距離.
		D3DXVECTOR3		SphereAdjPos;				// スフィアの調整座標.
		float						SphereAdjRadius;			// スフィアの調整半径.

		stEventAlienParam()
			: RotationalSpeed		( 0.0f )
			, MoveSpeed				( 0.0f )
			, ModelAlphaAddValue	( 0.0f )
			, WaitTime					( 0.0f )
			, ResearchLenght			( 0.0f )
			, SphereAdjPos			( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius		( 0.0f )
		{}
	} typedef SEventAlienParam;

protected:
	// 宇宙人状態.
	enum class enEventAlienState
	{
		None,

		Spawn,		// スポーン.
		Move,			// 移動.
		Abduct,		// 拐う.
		Escape,		// 逃げる.
		BlowAway, // 吹き飛ぶ.
		Wait,			// 待機.

		Max,

	} typedef EEventAlienState;

	// 移動状態.
	enum class enMoveState
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;


public:
	CEventAlien();
	virtual ~CEventAlien();

	// 相手座標の設定.
	virtual void SetTargetPos(CEventActor& actor) override;

private:
	// スポーン.
	virtual bool Spawn(const D3DXVECTOR3& spawnPos) = 0;
	// モデルのアルファ値の取得.
	float GetModelAplha() const { return m_ModelAlpha; }
	// 連れ去っているかどうか.
	bool IsAbduct() const { return m_NowState == EEventAlienState::Abduct; }
	// 連れ去るUFOの座標の取得.
	void SetAbductUFOPosition(D3DXVECTOR3* pos) { m_pAbductUFOPosition = pos; }

protected:
	// 現在の状態の更新関数.
	void CurrentStateUpdate();
	// 女の子の座標を設定.
	void SetGirlPos(CEventActor& actor);
	// 移動ベクトル設定関数.
	virtual void SetMoveVector(const D3DXVECTOR3& targetPos);
	// 目的の座標へ回転.
	void TargetRotation();
	// 移動関数.
	virtual void VectorMove(const float& moveSpeed);
	// 待機関数.
	virtual void WaitMove();

	// スポーン中.
	virtual void Spawning();
	// 移動.
	virtual void Move() override;
	// 拐う.
	virtual void Abduct();
	// 逃げる.
	virtual void Escape();
	// 吹き飛ぶ.
	virtual void BlowAway();
	// 待機.
	virtual void	Wait();

	// 女の子との当たり判定.
	void GirlCollision(CEventActor* pActor);
	// バリアとの当たり判定.
	void BarrierCollision(CEventActor* pActor);

protected:
	D3DXVECTOR3		m_TargetRotation;				// 目標の回転情報.
	D3DXVECTOR3		m_BeforeMoveingPosition;		// 移動前の座標.
	D3DXVECTOR3*		m_pAbductUFOPosition;		// UFOの座標.
	SEventAlienParam	m_Parameter;						// パラメータ.
	EEventAlienState		m_NowState;						// 現在の状態.
	EMoveState			m_NowMoveState;				// 現在の移動状態.
	float						m_ModelAlpha;						// モデルのアルファ値.
	int							m_WaitCount;						// 待機カウント.
	bool						m_IsBarrierHit;						// バリアに当たっているか.

};


#endif	//#ifndef EVENT_ALIEN_H.