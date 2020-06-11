#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

public:
	// �F���l�p�����[�^.
	struct stAlienParam : public stParameter
	{
		float		RotationalSpeed;	// ��]���x.
		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		SphereAdjRadius;	// �X�t�B�A�̒������a.

		stAlienParam()
			: SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
		{}
	} typedef SAlienParam;

protected:
	// �F���l���.
	enum class enAlienState
	{
		None,

		Spawn,	// �X�|�[��.
		Move,	// �ړ�.
		Abduct,	// ����.
		Fright,	// ����.
		Death,	// ���S.
		Escape,	// ������.

		Max,

	} typedef EAlienState;

	// �ړ����.
	enum class enMoveState
	{
		None,

		Rotation,
		Move,
		Attack,

		Max,
	} typedef EMoveState;
public:
	CAlien();
	virtual ~CAlien();

	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �X�|�[��.
	virtual bool Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos ) = 0;

protected:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// �ړ��x�N�g���ݒ�֐�.
	void SetMoveVector( const D3DXVECTOR3& targetPos );
	// �ړI�̍��W�։�].
	void TargetRotation();
	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed );

	// �X�|�[����.
	virtual void Spawning() = 0;
	// �ړ�.
	virtual void Move() override {}
	// ����.
	virtual void Abduct() = 0;
	// ����.
	virtual void Fright() = 0;
	// ���S.
	virtual void Death() = 0;
	// ������.
	virtual void Escape() = 0;

protected:
	D3DXVECTOR3	m_GirlPosition;		// ���̎q�̍��W.
	D3DXVECTOR3	m_TargetRotation;	// �ڕW�̉�]���.
	EAlienState	m_NowState;			// ���݂̏��.
};

#endif	// #ifndef ALIEN_H.