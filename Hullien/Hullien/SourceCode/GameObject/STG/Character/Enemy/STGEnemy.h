/**
* @file STGEnemy.h.
* @brief STG用の敵クラス.
* @author 福田玲也.
*/
#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"
#include "STGEnemyParam.h"

#include <random>

class CFont;	// フォントクラス.

namespace STG
{
	class CBulletManager;	// 弾管理クラス.
	struct stBulletManagerParam typedef SBulletManagerParam;
}

namespace STG
{
	/*****************************************
	*	敵クラス.
	**/
	class CEnemy : public STG::CCharacter
	{
		// 死亡した際のカメラアップパラメータ.
		struct stDeadUpParam
		{
			float	MoveSpeed;			// 移動速度.
			float	MoveAccValue;		// 移動加速値.
			float	CameraShakeCount;	// カメラの揺れカウント.
			bool	IsMoveEnd;			// 移動が終了したか.
			bool	IsPlaySE;			// SEを鳴らしたか.

			stDeadUpParam()
				: MoveSpeed			( 0.2f )
				, MoveAccValue		( 0.0f )
				, CameraShakeCount	( 40.0f )
				, IsMoveEnd			( false )
				, IsPlaySE			( false )
			{}

			inline float GetShakeValue() const
			{
				return sinf( static_cast<float>(D3DX_PI) * 2.0f / 3.0f * this->CameraShakeCount ) *
					( this->CameraShakeCount*0.005f );
			}

		} typedef SDeadUpParam;

	public:
		CEnemy();
		CEnemy( const STG::SEnemyParam& param );
		CEnemy( 
			const STG::SEnemyParam& param, 
			const std::vector<SBulletManagerParam>& bulletParams );
		virtual ~CEnemy();

		// 初期化関数.
		virtual bool Init() override;
		// 更新関数.
		virtual void Update() override;
		// 描画関数.
		virtual void Render() override;
		// 当たり判定.
		virtual void Collision( STG::CActor* pActor ) override;
		// スポーン時間の取得.
		inline float GetSpawnTime() const { return PARAMETER.SpawnTime; }
		// y座標の設定.
		inline void SetPositionY( const float& posY ){ m_vPosition.y = posY; }

		// 死亡したか.
		inline bool IsDead() const { return m_DeadUpParam.CameraShakeCount <= 0.0f; }

		// 最後のスポーンフラグの設定.
		inline void SetLastSpawn(){ m_IsMySpawnLast = true; }

		// 全弾の動作が終了したか.
		inline bool IsAllBulletUpdateEnd() const { return m_IsAllBulletUpdateEnd == true && m_IsActive == false; }

	private:
		// スポーン.
		void Spawn();
		// 移動関数.
		virtual void Move() override;
		// 弾を撃つ.
		void Shot();
		// 逃げる.
		void Escape();
		// 死亡.
		void Dead();
		// スポーンラストの際の死亡処理.
		void SpawnLastDead();

		// 当たった時の揺れ.
		void HitShake();

		// ライフ計算関数.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// ランダムで移動ベクトルを検索.
		void SearchRandomMoveVector();

		// 当たり判定の作成.
		bool CollisionInit();

	private:
		const STG::SEnemyParam PARAMETER;	// パラメータ.
		std::vector<std::unique_ptr<CBulletManager>>	m_pGuns;
		std::unique_ptr<CFont>	m_pFont;				// フォントクラス.
		STG::EEnemyState		m_NowState;				// 現在の状態.
		SDeadUpParam			m_DeadUpParam;			// 死亡時のカメラアップパラメータ.
		D3DXVECTOR3				m_FontRotation;			// フォントの回転値.
		float					m_MoveSpeed;			// 移動速度.
		float					m_MoveingDistance;		// 移動距離.
		float					m_MoveingDistanceMax;	// 移動距離.
		float					m_ShakeCount;			// 揺れのカウント.
		float					m_EscapeCount;			// 逃げるカウント.
		int						m_SpawnCount;			// スポーンカウント.
		bool					m_IsHitShake;			// 当たった時の揺れをするか.
		bool					m_IsMySpawnLast;		// 自分が最後のスポーン.
		bool					m_IsAllBulletUpdateEnd;	// 全弾の動作が終了したか.
	};
}

#endif	// #ifndef STG_ENEMY_H.