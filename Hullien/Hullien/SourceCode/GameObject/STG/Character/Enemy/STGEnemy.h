#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"

class CFont;

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		const char* MODEL_NAME			= "a";
		const char* BULLET_MODEL_NAME	= "item cure";

		const float ANGLE_MAX = static_cast<float>(D3DXToRadian(360.0));	// �e�̍ő�.
		const float ANGLE_MIN = static_cast<float>(D3DXToRadian(0.0));		// �e�̍ő�.

		const float INIT_POSITION_Z		= -100.0f;
		const float MOVE_SUB_VALUE		= 0.002f;
		const float MOVE_SUB_POSITION_Z = -10.0f;

		const D3DXVECTOR3 FONT_ROTATION =	// �t�H���g�̉�]�l.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};

		const char* RENDER_CHAR				= "FukutaReiya";
		const int	SPAWN_TIME				= 10;		// �X�|�[���̎���.
		const float MOVE_SPEED				= 0.2f;		// �ړ����x.
		const float BULLET_MOVE_SPEED		= 0.1f;		// �e�̑��x.
		const float	BULLET_ANGLE			= static_cast<float>(D3DXToRadian(25.0));	// �e�̊p�x.
		const int	BULLET_COUNT_MAX		= 30;		// �e�̍ő吔.
		const int	ANY_BULLET_COUNT_MAX	= 15;		// �����e�����ۂ̍ő吔.
		const int	SHOT_INTERVAL_FRAME		= 20;		// �e�����Ԋu�t���[��.
		const int	SHOT_BULLET_COUNT		= 2;		// �ꔭ�ŏo��e�̐�.
		const float SHOT_ANGLE				= static_cast<float>(D3DXToRadian(5.0));	// �����̊p�x.
		const int	SHOT_NUMBER				= 2;	// �e�̔ԍ� 1 : �ꔭ����, 2 : ����.

		// ���݂̏��.
		enum enState
		{
			EState_None,

			EState_Spawn,	// �X�|�[��.
			EState_Move,	// �ړ�.
			EState_Shot,	// ����.
			EState_Escape,	// ������.
			EState_Dead,	// ���S.

			EState_Max,
		} typedef EState;
	public:
		CEnemy();
		virtual ~CEnemy();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;

	private:
		// �X�|�[��.
		void Spawn();
		// �ړ��֐�.
		virtual void Move() override;
		// �e������.
		void Shot();
		// ������.
		void Escape();
		// ���S.
		void Dead();

		// �e������(�ꔭ����).
		virtual void BulletShot( const float& rot, const float& moveSpeed ) override;
		// �e������(����).
		void BulletShotAnyWay( 
			const float& rot, 
			const float& angle, 
			const float& moveSpeed,
			const int& bulletCount );

		// �����蔻��̍쐬.
		bool CollisionInit();

	private:
		std::unique_ptr<CFont> m_pFont;		// �t�H���g�N���X.
		EState	m_NowState;					// ���݂̏��.
		float	m_MoveSpeed;				// �ړ����x.
		int		m_SpawnCount;				// �X�|�[���J�E���g.
		float	m_Angle;					// �p�x.
		int		m_NowShotBulletCount;		// ���݂̌������e�̐�.
	};
}

#endif	// #ifndef STG_ENEMY_H.