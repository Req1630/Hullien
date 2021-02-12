/**
* @file STGCharacter.h.
* @brief STG用のキャラクター基底クラス.
* @author 福田玲也.
*/
#ifndef STG_CHARACTER_H
#define STG_CHARACTER_H

#include "..\STGActor.h"
#include <vector>

namespace STG
{
	/*******************************************
	*	STG用のキャラクター基底クラス.
	**/
	class CCharacter : public STG::CActor
	{
	public:
		CCharacter();
		virtual ~CCharacter();

		// 弾の取得.
		virtual std::vector<std::shared_ptr<CBullet>> GetBullets(){ return m_pBullets; }

	protected:
		// 移動関数.
		virtual void Move() = 0;
		// 弾の初期化.
		virtual bool BulletInit( 
			std::vector<std::shared_ptr<CBullet>>& bullets, 
			const int& bulletCount,
			const char* modelName );
		// 弾の更新.
		virtual void BulletUpdate();
		// 弾の描画.
		virtual void BulletRender( const D3DXVECTOR3& color = { 0.0f, 0.0f, 0.0f } );
		// 弾を撃つ.
		virtual bool BulletShot( const float& rot, const float& moveSpeed );

	protected:
		std::vector<std::shared_ptr<CBullet>> m_pBullets;
		int			m_ShotCount;	// 弾の撃った数.
		float		m_LifePoint;	// 体力.
	};

};

#endif	// #ifndef STG_CHARACTER_H.