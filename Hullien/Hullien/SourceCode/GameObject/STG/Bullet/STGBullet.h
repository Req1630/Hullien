/**
* @file STGBullet.h.
* @brief STG�p�̒e�N���X.
* @author ���c���.
*/
#ifndef STG_BULLET_H
#define STG_BULLET_H

#include "..\STGActor.h"

namespace STG
{
	/**************************************
	*	�e�N���X.
	**/
	class CBullet : public STG::CActor
	{
		static constexpr float	COLLISION_SCALE = 1.0f;	// �����蔻��̑傫��.
		static constexpr float	ATTACK_POWAR	= 1.0f;	// �U����.
	public:
		CBullet();
		CBullet( const char* modelName );
		virtual ~CBullet();

		// �������֐�.
		virtual bool Init() override;
		// �X�V�֐�.
		virtual void Update() override;
		// �`��֐�.
		virtual void Render() override;
		// �����蔻��.
		virtual void Collision( STG::CActor* pActor ) override;
		// ����.
		virtual bool Shoot( 
			const D3DXVECTOR3& pos,
			const float& rot, 
			const float& moveSpeed );

		// �F�̐ݒ�.
		void SetColor( const D3DXVECTOR3& color ){ m_Color = { color.x, color.y, color.z, 1.0f }; }
		// ��������.
		inline bool IsShot() const { return m_IsActive; }
		// ���������e��������ݒ�.
		void SetCollDisappear(){ m_IsBulletToBulletCollJudge = true; }

		// �X�V���I��������.
		inline bool IsUpdateEnd() const { return m_IsUpdateEnd; }

	private:
		// �����蔻��̍쐬.
		bool CollisionInit();

	private:
		const char* MODEL_NAME;						// ���f���� : �O������擾.
		D3DXVECTOR4 m_Color;						// �e�̐F.
		float		m_MoveSpeed;					// �ړ����x.
		bool		m_IsBulletToBulletCollJudge;	// �e�ǂ����̓����蔻����s����.
		bool		m_IsUpdateEnd;
	};
};

#endif	// #ifndef STG_BULLET_H