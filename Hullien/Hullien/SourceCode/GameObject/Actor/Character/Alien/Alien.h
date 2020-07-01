#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

protected:
	const float MODEL_ALPHA_MAX = 1.0f;	// ���f���A���t�@�̍ő�l.


public:
	// �F���l�p�����[�^.
	struct stAlienParam : public stParameter
	{
		float		RotationalSpeed;	// ��]���x.

		float		ModelAlphaAddValue;	// ���f���̃A���t�@�l�̉��Z����l.
		float		ModelAlphaSubValue;	// ���f���̃A���t�@�l�̌��Z����l.

		int			WaitTime;			// �ҋ@����.

		float		PlayerAimLenght;	// �v���C���[��_���͈�.

		float		AttackLenght;		// �U�����鋗��.
		float		AttackRotInitPower;	// �U�����̏�����]��.
		float		AttackRotPower;		// �U�����̉�]��.
		float		AttackRotAddValue;	// �U�����̉�]���Z�l.
		float		AttackMoveSpeed;	// �U���ړ����x.
		float		AttackMoveRange;	// �U���ړ��͈�.

		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		SphereAdjRadius;	// �X�t�B�A�̒������a.

		stAlienParam()
			: RotationalSpeed		( 0.0f )

			, ModelAlphaAddValue	( 0.0f )
			, ModelAlphaSubValue	( 0.0f )

			, WaitTime				( 0 )

			, PlayerAimLenght		( 0.0f )

			, AttackLenght			( 0.0f )
			, AttackRotInitPower	( 0.0f )
			, AttackRotPower		( 0.0f )
			, AttackRotAddValue		( 0.0f )
			, AttackMoveSpeed		( 0.0f )
			, AttackMoveRange		( 0.0f )

			, SphereAdjPos			( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius		( 0.0f )
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
		Wait,

		Max,
	} typedef EMoveState;

public:
	CAlien();
	virtual ~CAlien();

	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �X�|�[��.
	virtual bool Spawn( const SAlienParam& param, const D3DXVECTOR3& spawnPos ) = 0;

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&)>& ) override;
	// ���f���̃A���t�@�l�̎擾.
	float GetModelAplha() const { return m_ModelAlpha; }
	// �A�ꋎ���Ă��邩�ǂ���.
	bool IsAbduct() const { return m_NowState == EAlienState::Abduct; }
	// �ق��̉F���l���A�ꋎ���Ă��邩�ݒ�.
	void SetOtherAbduct( bool* pisAbduct ){ m_pIsAlienOtherAbduct = pisAbduct; }
	// �A�ꋎ��UFO�̍��W�̎擾.
	void SetAbductUFOPosition( D3DXVECTOR3* pos ){ m_pAbductUFOPosition = pos; }

	// �������邩�ǂ���.
	bool IsDelete() const { return m_IsDelete; }
protected:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// �ړ��x�N�g���ݒ�֐�.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// �ړI�̍��W�։�].
	void TargetRotation();
	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed );
	// �ҋ@�֐�.
	virtual void WaitMove();

	// �X�|�[����.
	virtual void Spawning();
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct();
	// ����.
	virtual void Fright();
	// ���S.
	virtual void Death();
	// ������.
	virtual void Escape();

	// ���̎q�Ƃ̓����蔻��.
	void GirlCollision( CActor* pActor );

protected:
	D3DXVECTOR3		m_TargetPosition;		// ���̎q�̍��W.
	D3DXVECTOR3		m_TargetRotation;		// �ڕW�̉�]���.
	D3DXVECTOR3*	m_pAbductUFOPosition;	// UFO�̍��W.
	SAlienParam		m_Parameter;			// �p�����[�^.
	EAlienState		m_NowState;				// ���݂̏��.
	EMoveState		m_NowMoveState;			// ���݂̈ړ����.
	float			m_ModelAlpha;			// ���f���̃A���t�@�l.
	int				m_WaitCount;			// �ҋ@�J�E���g.
	bool*			m_pIsAlienOtherAbduct;	// ���̉F���l���A�ꋎ���Ă��邩�ǂ���.

	bool			m_IsDelete;			// �������邩�ǂ���.
};

#endif	// #ifndef ALIEN_H.