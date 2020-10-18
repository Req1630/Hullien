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
		const float BULLET_MOVE_SPEED	= 0.5f;	// �e�̑��x.
		const int	BULLET_COUNT_MAX	= 30;	// �e�̍ő吔.
		const int	SHOT_INTERVAL_FRAME	= 20;	// �e�����Ԋu�t���[��.
	public:
		CEnemy();
		virtual ~CEnemy();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;

	private:
		// �ړ��֐�.
		virtual void Move() override;

		// �e������(�ꔭ����).
		virtual void BulletShot( const float& rot, const float& moveSpeed ) override;
		// �e������(����).
		void BulletShotAnyWay( 
			const float& rot, 
			const float& angle, 
			const float& moveSpeed,
			const int& bulletCount );

	private:
		int m_NowShotBulletCount;	// ���݂̌������e�̐�.
	};
}

#endif	// #ifndef STG_ENEMY_H.