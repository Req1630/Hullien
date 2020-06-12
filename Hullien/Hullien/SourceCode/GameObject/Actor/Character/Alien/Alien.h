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
		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		SphereAdjRadius;	// �X�t�B�A�̒������a.

		stAlienParam()
			: RotationalSpeed		( 0.05f )
			, ModelAlphaAddValue	( 0.01f )
			, ModelAlphaSubValue	( 0.01f )
			, WaitTime				( 3 )
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
	virtual bool Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos ) = 0;

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