#ifndef MOVE_SPEED_UP_H
#define MOVE_SPEED_UP_H

#include "..\..\ItemBase.h"

class CMoveSpeedUpItem : public CItemBase
{
	const char* MODEL_NAME = "item speed";
	const float* pMOVE_VALUE;	// �ړ���(�O������|�C���^�Ŏ擾����).
	const float* pMOVE_UP_TIME;	// �ړ���UP����(�O������|�C���^�Ŏ擾����).

public:
	CMoveSpeedUpItem();
	CMoveSpeedUpItem(
		const float* pMoveValue,
		const float* pMoveTime,
		const CItemBase::SParameter* pParam );
	virtual ~CMoveSpeedUpItem();

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

#endif	// #ifndef MOVE_SPEED_UP_H.