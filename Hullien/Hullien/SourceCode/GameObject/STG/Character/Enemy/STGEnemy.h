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
	class CEnemy : public STG::CCharacter
	{
		inline static const char* DEAD_SE_NAME			= "STGEnemyDead";
		inline static const float E_WND_OUT_ADJ_SIZE	= 10.0f;
		inline static const float INIT_POSITION_Z		= -100.0f;	// 初期座標 : Z.
		inline static const float MOVE_SUB_VALUE		= 0.002f;	// 移動速度を引く値.
		inline static const float MOVE_SUB_POSITION_Z	= -10.0f;	// 移動速度を引いていく座標.
		inline static const float SHAKE_COUNT_MAX		= 10.0f;	// 揺れのカウント.
		inline static const float SHAKE_SUB_VALUE		= 0.4f;		// 揺れカウントを引く値.
		inline static const float SHAKE_SPEED			= 0.1f;		// 揺れの速度.
		inline static const float DEAD_SCALE_SUB_VALUE	= 0.03f;	// 死亡時のスケール減算値.
		inline static const float DEAD_ROTATION_SPEED	= 0.1f;		// 死亡時の回転速度.
		inline static const float ESCAPE_MOVE_DISTANCE	= 4.0f;		// 逃げる移動距離.
		inline static const float ESCAPE_COUNT_MAX		= 10*FPS;	// 逃げる時間.
		inline static const float		FONT_SIZE		= 2.0f;		// フォントサイズ.
		inline static const D3DXVECTOR3 FONT_ROTATION	=			// フォントの回転値.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};
		inline static const float DEAD_TARGET_POSITION_LENGTH	= 0.5f;		// 死亡時のターゲットとの距離.
		inline static const float DEAD_TARGET_MOVE_SPEED		= 0.045f;	// 死亡時のターゲットとの移動速度.
		inline static const float DEAD_MOVE_ACC_ADD_VALUE		= 0.1f;		// 死亡移動加速加算値.
		inline static const float DEAD_MOVE_ACC_VALUE_MAX		= 1.5f;		// 死亡移動加速度最大値.
		inline static const float DEAD_POSITION_Y_ADJ_VALUE		= 40.0f;	// 死亡座標の調整値.
		inline static const float DEAD_CAMERA_SHAKE_ADJ_VALUE	= 10.0f;	// 死亡カメラ揺れ値.

		const STG::SEnemyParam PARAMETER;	// パラメータ.

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
	};
}

#endif	// #ifndef STG_ENEMY_H.