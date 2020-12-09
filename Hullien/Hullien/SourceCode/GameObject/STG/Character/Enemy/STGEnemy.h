#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"
#include "STGEnemyParam.h"

#include <random>

class CFont;	// �t�H���g�N���X.

namespace STG
{
	class CBulletManager;	// �e�Ǘ��N���X.
	struct stBulletManagerParam typedef SBulletManagerParam;
}

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		inline static const char* DEAD_SE_NAME			= "STGEnemyDead";
		inline static const float E_WND_OUT_ADJ_SIZE	= 10.0f;
		inline static const float INIT_POSITION_Z		= -100.0f;	// �������W : Z.
		inline static const float MOVE_SUB_VALUE		= 0.002f;	// �ړ����x�������l.
		inline static const float MOVE_SUB_POSITION_Z	= -10.0f;	// �ړ����x�������Ă������W.
		inline static const float SHAKE_COUNT_MAX		= 10.0f;	// �h��̃J�E���g.
		inline static const float SHAKE_SUB_VALUE		= 0.4f;		// �h��J�E���g�������l.
		inline static const float SHAKE_SPEED			= 0.1f;		// �h��̑��x.
		inline static const float DEAD_SCALE_SUB_VALUE	= 0.02f;	// ���S���̃X�P�[�����Z�l.
		inline static const float DEAD_ROTATION_SPEED	= 0.1f;		// ���S���̉�]���x.
		inline static const float ESCAPE_MOVE_DISTANCE	= 4.0f;		// ������ړ�����.
		inline static const float ESCAPE_COUNT_MAX		= 10*FPS;	// �����鎞��.
		inline static const float		FONT_SIZE		= 2.0f;		// �t�H���g�T�C�Y.
		inline static const D3DXVECTOR3 FONT_ROTATION	=			// �t�H���g�̉�]�l.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};
		inline static const float DEAD_TARGET_POSITION_LENGTH	= 0.5f;		// ���S���̃^�[�Q�b�g�Ƃ̋���.
		inline static const float DEAD_TARGET_MOVE_SPEED		= 0.045f;	// ���S���̃^�[�Q�b�g�Ƃ̈ړ����x.
		inline static const float DEAD_MOVE_ACC_ADD_VALUE		= 0.1f;		// ���S�ړ��������Z�l.
		inline static const float DEAD_MOVE_ACC_VALUE_MAX		= 1.5f;		// ���S�ړ������x�ő�l.
		inline static const float DEAD_POSITION_Y_ADJ_VALUE		= 10.0f;	// ���S���W�̒����l.
		inline static const float DEAD_CAMERA_SHAKE_ADJ_VALUE	= 10.0f;	// ���S�J�����h��l.

		const STG::SEnemyParam PARAMETER;	// �p�����[�^.

		// ���S�����ۂ̃J�����A�b�v�p�����[�^.
		struct stDeadUpParam
		{
			float	MoveSpeed;			// �ړ����x.
			float	MoveAccValue;		// �ړ������l.
			float	CameraShakeCount;	// �J�����̗h��J�E���g.
			bool	IsMoveEnd;			// �ړ����I��������.

			stDeadUpParam()
				: MoveSpeed			( 0.2f )
				, MoveAccValue		( 0.0f )
				, CameraShakeCount	( 40.0f )
				, IsMoveEnd			( false )
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

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;
		// �X�|�[�����Ԃ̎擾.
		inline float GetSpawnTime() const { return PARAMETER.SpawnTime; }
		// y���W�̐ݒ�.
		inline void SetPositionY( const float& posY ){ m_vPosition.y = posY; }

		// ���S������.
		inline bool IsDead() const { return m_DeadUpParam.CameraShakeCount <= 0.0f; }

		// �Ō�̃X�|�[���t���O�̐ݒ�.
		inline void SetLastSpawn(){ m_IsMySpawnLast = true; }

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
		// �X�|�[�����X�g�̍ۂ̎��S����.
		void SpawnLastDead();

		// �����������̗h��.
		void HitShake();

		// ���C�t�v�Z�֐�.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// �����_���ňړ��x�N�g��������.
		void SearchRandomMoveVector();

		// �����蔻��̍쐬.
		bool CollisionInit();

	private:
		std::vector<std::unique_ptr<CBulletManager>>	m_pGuns;
		std::unique_ptr<CFont>	m_pFont;				// �t�H���g�N���X.
		STG::EEnemyState		m_NowState;				// ���݂̏��.
		SDeadUpParam			m_DeadUpParam;			// ���S���̃J�����A�b�v�p�����[�^.
		D3DXVECTOR3				m_FontRotation;			// �t�H���g�̉�]�l.
		float					m_MoveSpeed;			// �ړ����x.
		float					m_MoveingDistance;		// �ړ�����.
		float					m_MoveingDistanceMax;	// �ړ�����.
		float					m_ShakeCount;			// �h��̃J�E���g.
		float					m_EscapeCount;			// ������J�E���g.
		int						m_SpawnCount;			// �X�|�[���J�E���g.
		bool					m_IsHitShake;			// �����������̗h������邩.
		bool					m_IsMySpawnLast;		// �������Ō�̃X�|�[��.
	};
}

#endif	// #ifndef STG_ENEMY_H.