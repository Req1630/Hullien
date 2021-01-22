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

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;
		// �e������.
		void Shot();
		// �����_���Ō���.
		void RandomShot();
		// �e�������I��������ǂ���.
		inline bool IsShotEnd() const { return m_IsShotEnd; }
		// �����p�x�̐ݒ�.
		void SetInitAngle( const D3DXVECTOR3& pos, const D3DXVECTOR3& targetPos );

		// �S�e�̓��삪�I��������.
		inline bool IsAllBulletUpdateEnd() const { return m_IsAllBulletUpdateEnd; }

	private:
		// �e������.
		void BulletShot( const float& rot, const float& moveSpeed );
		// �e�̏�����.
		bool BulletInit();

	private:
		std::vector<std::shared_ptr<CBullet>>	m_pBullets;	// �e.
		D3DXVECTOR3	m_BulletColor;				// �e�̐F.
		float		m_BulletAddAngle;			// �e�̉��Z�p�x.
		float		m_BulletAngleAccValue;		// �e�̊p�x�̉����l.
		float		m_BulletAngleAddAccValue;	// �e�̊p�x�̉����l.
		float		m_ShotAngle;				// ���݂̌��p�x.
		int			m_ShotCount;				// �e�����J�E���g�l.
		int			m_NowShotBulletCount;		// �������e�̐�.
		bool		m_IsShotEnd;				// �e�������I�������.
		bool		m_IsAllBulletUpdateEnd;		// �e�̓��삪�S���I��������.
	};
};

#endif	// #ifndef BULLET_MANAGER_H.