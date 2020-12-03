#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "..\STGBullet.h"
#include "BulletManagerParam.h"

namespace STG
{
	class CBulletManager : public STG::CActor
	{
		inline static const char* MODEL_NAME	= "SpawnPoint";
		inline static const float BULLET_COLOR	= 0.4f;

		const SBulletManagerParam PARAMETER;
	public:
		CBulletManager();
		CBulletManager( const SBulletManagerParam& param );
		virtual ~CBulletManager();

		// 初期化関数.
		virtual bool Init() override;
		// 更新関数.
		virtual void Update() override;
		// 描画関数.
		virtual void Render() override;
		// 当たり判定.
		virtual void Collision( STG::CActor* pActor ) override;
		// 弾を撃つ.
		void Shot();
		// ランダムで撃つ.
		void RandomShot();
		// 弾が撃ち終わったかどうか.
		inline bool IsShotEnd(){ return m_IsShotEnd; }
		// 初期角度の設定.
		void SetInitAngle( const D3DXVECTOR3& pos, const D3DXVECTOR3& targetPos );

	private:
		// 弾を撃つ.
		void BulletShot( const float& rot, const float& moveSpeed );
		// 弾の初期化.
		bool BulletInit();

	private:
		std::vector<std::shared_ptr<CBullet>>	m_pBullets;	// 弾.
		D3DXVECTOR3	m_BulletColor;			// 弾の色.
		float		m_BulletAddAngle;		// 弾の加算角度.
		float		m_BulletAngleAccValue;	// 弾の角度の加速値.
		float		m_BulletAngleAddAccValue;	// 弾の角度の加速値.
		float		m_ShotAngle;			// 現在の撃つ角度.
		int			m_ShotCount;			// 弾を撃つカウント値.
		int			m_NowShotBulletCount;	// 撃った弾の数.
		int			m_AngleReverseCount;	// 角度反転用カウント値.
		bool		m_IsShotEnd;			// 弾が撃ち終わったか.
	};
};

#endif	// #ifndef BULLET_MANAGER_H.