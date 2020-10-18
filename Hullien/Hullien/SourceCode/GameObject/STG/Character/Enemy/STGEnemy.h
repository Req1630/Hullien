#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		const char* MODEL_NAME			= "a";
		const char* BULLET_MODEL_NAME	= "item cure";
		const float MOVE_SPEED			= 0.2f;
		const float BULLET_MOVE_SPEED	= 0.5f;	// 弾の速度.
		const int	BULLET_COUNT_MAX	= 30;	// 弾の最大数.
		const int	SHOT_INTERVAL_FRAME	= 20;	// 弾を撃つ間隔フレーム.
	public:
		CEnemy();
		virtual ~CEnemy();

		// 初期化関数.
		virtual bool Init() override;
		// 更新関数.
		virtual void Update() override;
		// 描画関数.
		virtual void Render() override;

	private:
		// 移動関数.
		virtual void Move() override;

		// 弾を撃つ(一発ずつ).
		virtual void BulletShot( const float& rot, const float& moveSpeed ) override;
		// 弾を撃つ(複数).
		void BulletShotAnyWay( 
			const float& rot, 
			const float& angle, 
			const float& moveSpeed,
			const int& bulletCount );

	private:
		int m_NowShotBulletCount;	// 現在の撃った弾の数.
	};
}

#endif	// #ifndef STG_ENEMY_H.