#ifndef STG_PLAYER_H
#define STG_PLAYER_H

#include "..\STGCharacter.h"

namespace STG
{
	class CPlayer : public STG::CCharacter
	{
		const char* MODEL_NAME = "ufo_mini";
		const char* BULLET_MODEL_NAME = "item cure";
		const float MOVE_SPEED = 0.4f;
		const float BULLET_MOVE_SPEED	= 0.8f;	// �e�̑��x.
		const int	BULLET_COUNT_MAX	= 30;	// �e�̍ő吔.
		const int	SHOT_INTERVAL_FRAME	= 15;	// �e�����Ԋu�t���[��.
		const D3DXVECTOR3 INIT_POSITION = { 0.0f, 0.0f, 70.0f };
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
		// �e��������֐�.
		void ShotController();

		// ���C�t�v�Z�֐�.
		virtual void LifeCalculation( const std::function<void(float&)>& ) override;

		// �����蔻��̍쐬.
		bool CollisionInit();
		
	private:
		D3DXVECTOR3 m_Direction;
		float		m_SpawnMoveSpeed;
	};
};

#endif	// #ifndef STG_PLAYER_H.