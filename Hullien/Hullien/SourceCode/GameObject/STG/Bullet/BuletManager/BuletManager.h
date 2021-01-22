#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "..\STGBullet.h"
#include "BulletManagerParam.h"

namespace STG
{
	class CBulletManager : public STG::CActor
	{
		inline static const char* MODEL_NAME	= "SpawnPoint";
		inline static const float BULLET_COLOR	= 0.5f;

		const SBulletManagerParam PARAMETER;
	public:
		CBulletManager();
		CBulletManager( const SBulletManagerParam& param );
		virtual ~CBulletManager();

		// ‰Šú‰»ŠÖ”.
		virtual bool Init() override;
		// XVŠÖ”.
		virtual void Update() override;
		// •`‰æŠÖ”.
		virtual void Render() override;
		// “–‚½‚è”»’è.
		virtual void Collision( STG::CActor* pActor ) override;
		// ’e‚ğŒ‚‚Â.
		void Shot();
		// ƒ‰ƒ“ƒ_ƒ€‚ÅŒ‚‚Â.
		void RandomShot();
		// ’e‚ªŒ‚‚¿I‚í‚Á‚½‚©‚Ç‚¤‚©.
		inline bool IsShotEnd() const { return m_IsShotEnd; }
		// ‰ŠúŠp“x‚Ìİ’è.
		void SetInitAngle( const D3DXVECTOR3& pos, const D3DXVECTOR3& targetPos );

		// ‘S’e‚Ì“®ì‚ªI—¹‚µ‚½‚©.
		inline bool IsAllBulletUpdateEnd() const { return m_IsAllBulletUpdateEnd; }

	private:
		// ’e‚ğŒ‚‚Â.
		void BulletShot( const float& rot, const float& moveSpeed );
		// ’e‚Ì‰Šú‰».
		bool BulletInit();

	private:
		std::vector<std::shared_ptr<CBullet>>	m_pBullets;	// ’e.
		D3DXVECTOR3	m_BulletColor;				// ’e‚ÌF.
		float		m_BulletAddAngle;			// ’e‚Ì‰ÁZŠp“x.
		float		m_BulletAngleAccValue;		// ’e‚ÌŠp“x‚Ì‰Á‘¬’l.
		float		m_BulletAngleAddAccValue;	// ’e‚ÌŠp“x‚Ì‰Á‘¬’l.
		float		m_ShotAngle;				// Œ»İ‚ÌŒ‚‚ÂŠp“x.
		int			m_ShotCount;				// ’e‚ğŒ‚‚ÂƒJƒEƒ“ƒg’l.
		int			m_NowShotBulletCount;		// Œ‚‚Á‚½’e‚Ì”.
		bool		m_IsShotEnd;				// ’e‚ªŒ‚‚¿I‚í‚Á‚½‚©.
		bool		m_IsAllBulletUpdateEnd;		// ’e‚Ì“®ì‚ª‘S•”I—¹‚µ‚½‚©.
	};
};

#endif	// #ifndef BULLET_MANAGER_H.