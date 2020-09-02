#ifndef ATTACK_UP_H
#define ATTACK_UP_H

#include "..\..\ItemBase.h"

class CAttackUpItem : public CItemBase
{
	const char* MODEL_NAME = "item attack";
	const float* pATTACK_UP_POWER;	// �U����(�O������|�C���^�Ŏ擾����).
	const float* pATTACK_UP_TIME;	// �U����(�O������|�C���^�Ŏ擾����).

public:
	CAttackUpItem();
	CAttackUpItem(
		const float* pAttackUpPower,
		const float* pAttackUpTime,
		const CItemBase::SParameter* pParam );
	virtual ~CAttackUpItem();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& pActor ) override;

private:
	// ���������ۂ̌��ʂ�^����.
	virtual void GiveEffect( CActor* pActor ) override;
};

#endif	// #ifndef ATTACK_UP_H.