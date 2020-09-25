#ifndef EVENT_GIRL_H
#define EVENT_GIRL_H

#include "..\EventCharacter.h"

/****************************************
*	イベント用女の子クラス.
**/
class CEventGirl : public CEventCharacter
{
	const char* MODEL_NAME = "Towa_s";	// モデル名.
	const char* MODEL_TEMP_NAME = "yuri-dy";	// 仮モデル名.

	// パラメータ.
	struct stGirlParam
	{
		float		SearchCollRadius;	// 索敵用のスフィアの半径.
		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		SphereAdjRadius;	// スフィアの調整半径.

		stGirlParam()
			: SearchCollRadius(10.0f)
			, SphereAdjPos(0.0f, 0.0f, 0.0f)
			, SphereAdjRadius(0.0f)
		{}
	} typedef SGirlParam;


	// 現在の状態.
	enum class enNowState
	{
		None,

		Protected,	// 守られている.
		Abduct,		// 連れ去れている.
		Move,			// 移動.
		Wait,			// 待機.

		Max,

	} typedef ENowState;

	// 移動状態.
	enum class enMoveState
	{
		None,

		Rotation,	// 回転.
		Move,			// 移動.
		Wait,			// 待機.

		Max,
	} typedef EMoveState;

public:
	CEventGirl();
	virtual ~CEventGirl();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision(CEventActor* pActor) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos(CEventActor& actor) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos(CActor& actor) override;
	
	// 女の子の状態設定関数.
	void SetNowState(const ENowState& state) { m_NowState = state; }

private:
	// 移動関数.
	virtual void Move() override;
	// 目的の場所に向けて回転.
	void TargetRotation();
	// 目的の場所に向けて移動.
	void TargetMove();

	// 索敵の当たり判定.
	void SearchCollision(CEventActor* pActor);

	// 当たり判定の作成.
	bool  ColliderSetting();

private:
	SGirlParam m_Parameter;	// 女の子のパラメーター.
	std::shared_ptr<CCollisionManager>	m_pSearchCollManager;	// 索敵用の当たり判定.
	D3DXVECTOR3	m_OldPosition;		// 前回の座標.
	ENowState	m_NowState;				// 現在の状態.
	EMoveState	m_NowMoveState;	// 現在の移動状態.

};

#endif //#ifndef EVENT_GIRL_H.
