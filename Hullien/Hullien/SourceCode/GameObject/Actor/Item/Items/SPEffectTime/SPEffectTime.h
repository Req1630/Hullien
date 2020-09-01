#ifndef SP_EFFECT_TIME_H
#define SP_EFFECT_TIME_H

#include "..\..\ItemBase.h"

class CSPEffectTimeItem : public CItemBase
{
	const char* MODEL_NAME = "item ability";
	const float* pSP_RECOVERY_VALUE;	// ����\�͉񕜗�(�O������|�C���^�Ŏ擾����).
	const float* pSP_RECOVERY_TIME;		// ����\�͉񕜎���(�O������|�C���^�Ŏ擾����).
public:
	CSPEffectTimeItem();
	CSPEffectTimeItem(
		const float* pSPRecoveryValue,
		const float* pSPRecoveryTime,
		const CItemBase::SParameter* pParam );
	virtual ~CSPEffectTimeItem();

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

#endif	// #ifndef SP_EFFECT_TIME_H.