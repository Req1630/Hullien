#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"
#include "STGEnemyParam.h"

#include <random>

class CFont;

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		inline static const float E_WND_OUT_ADJ_SIZE	= 10.0f;
		inline static const char* BULLET_MODEL_NAME		= "SpawnPoint";
		inline static const float INIT_POSITION_Z		= -100.0f;	// �������W : Z.
		inline static const float MOVE_SUB_VALUE		= 0.002f;	// �ړ����x�������l.
		inline static const float MOVE_SUB_POSITION_Z	= -10.0f;	// �ړ����x�������Ă������W.
		inline static const float SHAKE_COUNT_MAX		= 10.0f;	// �h��̃J�E���g.
		inline static const float SHAKE_SUB_VALUE		= 0.4f;		// �h��J�E���g�������l.
		inline static const float SHAKE_SPEED			= 0.08f;	// �h��̑��x.
		inline static const float DEAD_SCALE_SUB_VALUE	= 0.02f;	// ���S���̃X�P�[�����Z�l.
		inline static const float DEAD_ROTATION_SPEED	= 0.1f;		// ���S���̉�]���x.
		inline static const float ESCAPE_MOVE_DISTANCE	= 4.0f;
		inline static const D3DXVECTOR3 FONT_ROTATION =	// �t�H���g�̉�]�l.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};
		inline static const float  FONT_SIZE = 2.0f;	// �t�H���g�T�C�Y.

		const STG::SEnemyParam PARAMETER;	// �p�����[�^.

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
		CEnemy( const STG::SEnemyParam& param );
		virtual ~CEnemy();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;
		// �X�|�[�����Ԃ̎擾.
		float GetSpawnTime() const { return PARAMETER.SpawnTime; }

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

		// �����������̗h��.
		void HitShake();

		// �e������(�ꔭ����).
		virtual void BulletShot( const float& rot, const float& moveSpeed ) override;
		// �e������(����).
		void BulletShotAnyWay(
			const float& rot,
			const float& angle, 
			const float& moveSpeed,
			const int& bulletCount );

		// ���C�t�v�Z�֐�.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// �����_���ňړ��x�N�g��������.
		void SearchRandomMoveVector();

		// �����蔻��̍쐬.
		bool CollisionInit();

	private:
		std::unique_ptr<CFont> m_pFont;		// �t�H���g�N���X.
		D3DXVECTOR3	m_FontRotation;			// �t�H���g�̉�]�l.
		EState		m_NowState;				// ���݂̏��.
		float		m_MoveSpeed;			// �ړ����x.
		float		m_LifePoint;			// �̗�.
		float		m_MoveingDistance;		// �ړ�����.
		float		m_MoveingDistanceMax;	// �ړ�����.
		int			m_SpawnCount;			// �X�|�[���J�E���g.
		float		m_ShotAngle;			// ���Ƃ��̊p�x.
		int			m_NowShotBulletCount;	// ���݂̌������e�̐�.
		bool		m_IsHitShake;			// �����������̗h������邩.
		float		m_ShakeCount;			// �h��̃J�E���g.
	};
}

#endif	// #ifndef STG_ENEMY_H.