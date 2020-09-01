#ifndef LIFE_RECOVERY_H
#define LIFE_RECOVERY_H

#include "..\..\ItemBase.h"

class CLifeRecoveryItem : public CItemBase
{
	const char* MODEL_NAME = "item cure";
	const float* pLIFE_RECOVERY_VALUE;	// �񕜗�(�O������|�C���^�Ŏ擾����).
public:
	CLifeRecoveryItem();
	CLifeRecoveryItem( 
		const float* pLifeRecoveryValue, 
		const CItemBase::SParameter* pParam );
	virtual ~CLifeRecoveryItem();

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

#endif	// #ifndef LIFE_RECOVERY_H.