/**
* @file STGEnemy.h.
* @brief STG�p�̓G�N���X.
* @author ���c���.
*/
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
	/*****************************************
	*	�G�N���X.
	**/
	class CEnemy : public STG::CCharacter
	{
		// ���S�����ۂ̃J�����A�b�v�p�����[�^.
		struct stDeadUpParam
		{
			float	MoveSpeed;			// �ړ����x.
			float	MoveAccValue;		// �ړ������l.
			float	CameraShakeCount;	// �J�����̗h��J�E���g.
			bool	IsMoveEnd;			// �ړ����I��������.
			bool	IsPlaySE;			// SE��炵����.

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

		// �S�e�̓��삪�I��������.
		inline bool IsAllBulletUpdateEnd() const { return m_IsAllBulletUpdateEnd == true && m_IsActive == false; }

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
		const STG::SEnemyParam PARAMETER;	// �p�����[�^.
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
		bool					m_IsAllBulletUpdateEnd;	// �S�e�̓��삪�I��������.
	};
}

#endif	// #ifndef STG_ENEMY_H.