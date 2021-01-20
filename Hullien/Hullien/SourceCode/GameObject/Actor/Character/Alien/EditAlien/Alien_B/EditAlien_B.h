#ifndef EDIT_ALIEN_B_H
#define EDIT_ALIEN_B_H

#include "..\EditAlien.h"

class CEditAlienB : public CEditAlien
{
	const char* MODEL_NAME = "b_s";
public:
	CEditAlienB();
	virtual	~CEditAlienB();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// 攻撃の再生.
	virtual void PlayAttack() override;

	// パラメーターの設定.
	virtual void SetParamter( const SAlienParam& param ) override;

private:
	// スポーン.
	virtual void Spawning() override;
	// 移動.
	virtual void Move() override;
	// 拐う.
	virtual void Abduct() override;
	// 怯み.
	virtual void Fright() override;
	// 死亡.
	virtual void Death() override;
	// 逃げる.
	virtual void Escape() override;

	// 攻撃関数.
	void Attack();

	// 当たり判定の設定.
	bool ColliderSetting();

private:
	float	m_RotAccValue;		// 回転加速値.
	bool	m_IsAttackSE;		// 攻撃SEを鳴らすか.
};

#endif	// #ifndef EDIT_ALIEN_B_H.
