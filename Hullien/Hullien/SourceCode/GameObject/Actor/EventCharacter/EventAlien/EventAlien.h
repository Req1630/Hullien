#ifndef EVENT_ALIEN_H
#define EVENT_ALIEN_H

#include "..\EventCharacter.h"

/****************************************
*	�C�x���g�p�F���l�N���X.
**/
class CEventAlien : public CEventCharacter
{
protected:
	const float MODEL_ALPHA_MAX = 1.0f;	// ���f���A���t�@�̍ő�l.

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

public:
	CEventAlien();
	virtual ~CEventAlien();

	// ������W�̐ݒ�.
	virtual void SetTargetPos(CActor& actor) override;

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
	void SetGirlPos(CActor& actor);

	// �X�|�[����.
	virtual void Spawning();
	// �ړ�.
	virtual void Move();
	// ����.
	virtual void Abduct();
	// ������.
	virtual void Escape();
	// �������.
	virtual void BlowAway();

	// ���̎q�Ƃ̓����蔻��.
	void GirlCollision(CActor* pActor);
	// �o���A�Ƃ̓����蔻��.
	void BarrierCollision(CActor* pActor);

protected:
	D3DXVECTOR3*		m_pAbductUFOPosition;		// UFO�̍��W.
	EEventAlienState		m_NowState;						// ���݂̏��.
	bool						m_IsBarrierHit;						// �o���A�ɓ������Ă��邩.
	float						m_Speed;							// �ړ����x,

};


#endif	//#ifndef EVENT_ALIEN_H.