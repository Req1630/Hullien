#ifndef STG_PLAYER_H
#define STG_PLAYER_H

#include "..\STGCharacter.h"

namespace STG
{
	class CPlayer : public STG::CCharacter
	{
		const char* MODEL_NAME = "a";
		const char* BULLET_MODEL_NAME = "item cure";
		const float MOVE_SPEED = 0.2f;
		const float BULLET_MOVE_SPEED	= 0.5f;	// �e�̑��x.
		const int	BULLET_COUNT_MAX	= 30;	// �e�̍ő吔.
		const int	SHOT_INTERVAL_FRAME	= 15;	// �e�����Ԋu�t���[��.
	public:
		CPlayer();
		virtual ~CPlayer();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// ����֐�.
		void Controller();

	private:
		// �ړ��֐�.
		virtual void Move() override;
		// �e��������֐�.
		void ShotController();

		
	private:
		D3DXVECTOR3 m_Direction;
	};
};

#endif	// #ifndef STG_PLAYER_H.