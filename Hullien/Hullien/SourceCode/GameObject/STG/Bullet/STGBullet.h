#ifndef STG_BULLET_H
#define STG_BULLET_H

#include "..\STGActor.h"

namespace STG
{
	class CBullet : public STG::CActor
	{
		const char* MODEL_NAME;				// モデル名 : 外部から取得.
		const float	COLLISION_SCALE = 1.0f;	// 当たり判定の大きさ.
		const float ATTACK_POWAR	= 1.0f;	// 攻撃力.
	public:
		CBullet();
		CBullet( const char* modelName );
		virtual ~CBullet();

		// 初期化関数.
		virtual bool Init() override;
		// 更新関数.
		virtual void Update() override;
		// 描画関数.
		virtual void Render() override;
		// 当たり判定.
		virtual void Collision( STG::CActor* pActor ) override;
		// 撃つ.
		virtual bool Shoot( 
			const D3DXVECTOR3& pos,
			const float& rot, 
			const float& moveSpeed );

		// 色の設定.
		void SetColor( const D3DXVECTOR3& color ){ m_Color = { color.x, color.y, color.z, 1.0f }; }
		// 撃ったか.
		inline bool IsShot() const { return m_IsActive; }
		// 当たった弾が消える設定.
		void SetCollDisappear(){ m_IsBulletToBulletCollJudge = true; }

		// 更新が終了したか.
		inline bool IsUpdateEnd() const { return m_IsUpdateEnd; }

	private:
		// 当たり判定の作成.
		bool CollisionInit();

	private:
		D3DXVECTOR4 m_Color;						// 弾の色.
		float		m_MoveSpeed;					// 移動速度.
		bool		m_IsBulletToBulletCollJudge;	// 弾どうしの当たり判定を行うか.
		bool		m_IsUpdateEnd;
	};
};

#endif	// #ifndef STG_BULLET_H