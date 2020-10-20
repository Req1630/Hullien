#ifndef STG_ENEMY_H
#define STG_ENEMY_H

#include "..\STGCharacter.h"
#include "STGEnemyParam.h"

class CFont;

namespace STG
{
	class CEnemy : public STG::CCharacter
	{
		inline static const char* BULLET_MODEL_NAME		= "item cure";
		inline static const float INIT_POSITION_Z		= -100.0f;	// �������W : Z.
		inline static const float MOVE_SUB_VALUE		= 0.002f;	// �ړ����x�������l.
		inline static const float MOVE_SUB_POSITION_Z	= -10.0f;	// �ړ����x�������Ă������W.

		inline static const D3DXVECTOR3 FONT_ROTATION =	// �t�H���g�̉�]�l.
		{
			static_cast<float>(D3DXToRadian(270)),
			0.0f, 
			static_cast<float>(D3DXToRadian(180))
		};

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