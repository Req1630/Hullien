#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"

class CExplosion : public CActor
{
public:
	// 爆発パラメーター.
	struct stExplosionParam
	{
		float		ExplosionSpeed;		// 爆発速度.
		float		AttackPower;		// 爆発力.
		float		SphereMaxRadius;	// スフィアの最大半径.
		D3DXVECTOR3	SphereAdjPos;		// スフィアの調整用座標.

		stExplosionParam()
			: ExplosionSpeed	( 0.0f )
			, AttackPower		( 0.0f )
			, SphereMaxRadius	( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		{}
	} typedef SExplosionParam;


public:
	CExplosion();
	virtual ~CExplosion();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// パラメーターの設定.
	void SetExplosionParam( const SExplosionParam& param ){ m_Param = param; }

private:
	// 当たり判定の設定.
	bool ColliderSetting();

private:
	SExplosionParam	m_Param;		// 爆発パラメーター.
	float	m_CollSphereRadius;		// 当たり判定の半径.
};

#endif	// #ifndef EXPLOSION_H.