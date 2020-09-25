#ifndef EVENT_ALIEN_H
#define EVENT_ALIEN_H

#include "..\EventCharacter.h"

/****************************************
*	�C�x���g�p�F���l�N���X.
**/
class CEventAlien : public CEventCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

protected:
	const float MODEL_ALPHA_MAX = 1.0f;	// ���f���A���t�@�̍ő�l.

public:
	// �F���l�̃p�����[�^.
	struct stEventAlienParam
	{
		float						RotationalSpeed;			//��]���x.
		float						MoveSpeed;				// �ړ����x.
		float						ModelAlphaAddValue;	// ���f���̃A���t�@�l�̉��Z����l.
		float						WaitTime;					//�ҋ@����.
		float						ResearchLenght;			// �Č�������ۂ̋���.
		D3DXVECTOR3		SphereAdjPos;				// �X�t�B�A�̒������W.
		float						SphereAdjRadius;			// �X�t�B�A�̒������a.

		stEventAlienParam()
			: RotationalSpeed		( 0.0f )
			, MoveSpeed				( 0.0f )
			, ModelAlphaAddValue	( 0.0f )
			, WaitTime					( 0.0f )
			, ResearchLenght			( 0.0f )
			, SphereAdjPos			( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius		( 0.0f )
		{}
	} typedef SEventAlienParam;

protected:
	// �F���l���.
	enum class enEventAlienState
	{
		None,

		Spawn,		// �X�|�[��.
		Move,			// �ړ�.
		Abduct,		// ����.
		Escape,		// ������.
		BlowAway, // �������.
		Wait,			// �ҋ@.

		Max,

	} typedef EEventAlienState;

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
	CEventAlien();
	virtual ~CEventAlien();

	// ������W�̐ݒ�.
	virtual void SetTargetPos(CEventActor& actor) override;

private:
	// �X�|�[��.
	virtual bool Spawn(const D3DXVECTOR3& spawnPos) = 0;
	// ���f���̃A���t�@�l�̎擾.
	float GetModelAplha() const { return m_ModelAlpha; }
	// �A�ꋎ���Ă��邩�ǂ���.
	bool IsAbduct() const { return m_NowState == EEventAlienState::Abduct; }
	// �A�ꋎ��UFO�̍��W�̎擾.
	void SetAbductUFOPosition(D3DXVECTOR3* pos) { m_pAbductUFOPosition = pos; }

protected:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// ���̎q�̍��W��ݒ�.
	void SetGirlPos(CEventActor& actor);
	// �ړ��x�N�g���ݒ�֐�.
	virtual void SetMoveVector(const D3DXVECTOR3& targetPos);
	// �ړI�̍��W�։�].
	void TargetRotation();
	// �ړ��֐�.
	virtual void VectorMove(const float& moveSpeed);
	// �ҋ@�֐�.
	virtual void WaitMove();

	// �X�|�[����.
	virtual void Spawning();
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct();
	// ������.
	virtual void Escape();
	// �������.
	virtual void BlowAway();
	// �ҋ@.
	virtual void	Wait();

	// ���̎q�Ƃ̓����蔻��.
	void GirlCollision(CEventActor* pActor);
	// �o���A�Ƃ̓����蔻��.
	void BarrierCollision(CEventActor* pActor);

protected:
	D3DXVECTOR3		m_TargetRotation;				// �ڕW�̉�]���.
	D3DXVECTOR3		m_BeforeMoveingPosition;		// �ړ��O�̍��W.
	D3DXVECTOR3*		m_pAbductUFOPosition;		// UFO�̍��W.
	SEventAlienParam	m_Parameter;						// �p�����[�^.
	EEventAlienState		m_NowState;						// ���݂̏��.
	EMoveState			m_NowMoveState;				// ���݂̈ړ����.
	float						m_ModelAlpha;						// ���f���̃A���t�@�l.
	int							m_WaitCount;						// �ҋ@�J�E���g.
	bool						m_IsBarrierHit;						// �o���A�ɓ������Ă��邩.

};


#endif	//#ifndef EVENT_ALIEN_H.