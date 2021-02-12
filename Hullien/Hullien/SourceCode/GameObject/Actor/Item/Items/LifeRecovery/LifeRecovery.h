/**
* @file LifeRecovery.h.
* @brief �̗͉񕜃A�C�e���N���X.
* @author ���c���.
*/
#ifndef LIFE_RECOVERY_H
#define LIFE_RECOVERY_H

#include "..\..\ItemBase.h"

/*********************************************
*	�̗͉񕜃A�C�e���N���X.
**/
class CLifeRecoveryItem : public CItemBase
{
	inline static const char* MODEL_NAME					= "item cure";	// ���f����.
	inline static const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_cure";	// �G�t�F�N�g��.
	inline static const char* HIT_EFFECT_NAME				= "item_get_cure";	// �G�t�F�N�g��.
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
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& pActor ) override;

private:
	// ���������ۂ̌��ʂ�^����.
	virtual void GiveEffect( CActor* pActor ) override;
	// �G�t�F�N�g�̐ݒ�.
	virtual bool EffectSetting() override;

private:
	const float* pLIFE_RECOVERY_VALUE;	// �񕜗�(�O������|�C���^�Ŏ擾����).
};

#endif	// #ifndef LIFE_RECOVERY_H.