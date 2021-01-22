#ifndef STG_PLAYER_H
#define STG_PLAYER_H

#include "..\STGCharacter.h"

namespace STG
{
	class CPlayer : public STG::CCharacter
	{
		const char* MODEL_NAME				= "ufo";
		const char* BULLET_MODEL_NAME		= "SpawnPoint";
		const char* SHOT_SE_NAME			= "STGshot";
		const char* HIT_SE_NAME				= "STGHit";
		const float STANDARD_MODEL_COLOR	= 1.0f;
		const float LIFE_POINT_MAX			= 5.0f;						// ���C�t�̍ő吔.
		const float MOVE_SPEED				= 0.4f;						// �ړ����x.
		const float BULLET_MOVE_SPEED		= 0.8f;						// �e�̑��x.
		const int	BULLET_COUNT_MAX		= 30;						// �e�̍ő吔.
		const int	SHOT_INTERVAL_FRAME		= 15;						// �e�����Ԋu�t���[��.
		const D3DXVECTOR3 BULLET_COLOR		= { 0.4f, 1.2f, 0.4f };		// �e�̐F.
		const D3DXVECTOR3 INIT_POSITION		= { 0.0f, 0.0f, 70.0f };	// �������W.
		const float MODEL_SCALE_MAX			= 0.2f;						// ���f���̍ő�T�C�Y.
		const float MODEL_SCALE_MIN			= 0.0f;						// ���f���̍ŏ��T�C�Y.

		const float COLLISION_SCALE			= 3.0f;		// �����蔻��̑傫��.
		const float SPAWN_END_POS_Z			= 40.0f;	// �X�|�[���̏I�����W.
		const float SPAWN_SPEED_SUB			= 0.005f;	// �X�|�[���̈ړ����x�̌��Z�l.

		const float INIT_MOVE_COUNT			= (MOVE_SPEED / SPAWN_SPEED_SUB)*2.0f;
		const float INIT_MOVE_ROT_ADD		= static_cast<float>(D3DX_PI) / (INIT_MOVE_COUNT*0.5f);

		const float OUT_POSITION_X			= 100.0f;	// ��ʊO�̍��W : X.
		const float OUT_POSITION_Z			= 55.0f;	// ��ʊO�̍��W : Z.
		const float DEAD_SPEED				= 0.02f;	// ���S���x.

		const float	INVINCIBLE_BLINKING_VALUE_MIN	= 0.7f;		// �_�ł̍ŏ��l
		const int	INVINCIBLE_BLINKING_SPEED		= 6;		// �_�ł̑��x.
		const int	INVINCIBLE_TIME					= 2*FPS;	// ���G����.

	public:
		CPlayer();
		virtual ~CPlayer();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;
		// ����֐�.
		void Controller();

	private:
		// �X�|�[���ړ�.
		void SpawnMove();
		// �ړ��֐�.
		virtual void Move() override;
		// ���S�㏈��.
		void DeadUpdate();
		// �e��������֐�.
		void ShotController();

		// ���C�t�v�Z�֐�.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// ��ʊO�ɍs�������̏���.
		void OutDispMove();

		// �����蔻��̍쐬.
		bool CollisionInit();
		
	private:
		D3DXVECTOR3 m_Direction;			// ����.
		int			m_InvincibleTimeCount;	// ���G����.
		float		m_SpawnMoveSpeed;		// �X�|�[�����̈ړ����x.
		float		m_InitMoveCount;		// �����ړ��J�E���g.
		bool		m_IsDead;				// ���S������.
		bool		m_IsInitMoveEnd;		// �����ړ����I��������.
	};
};

#endif	// #ifndef STG_PLAYER_H.