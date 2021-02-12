/**
* @file AttackUp.h.
* @brief �U���̓A�b�v�A�C�e���N���X.
* @author ���c���.
*/
#ifndef ATTACK_UP_H
#define ATTACK_UP_H

#include "..\..\ItemBase.h"

/*******************************************
*	�U���̓A�b�v�A�C�e���N���X.
**/
class CAttackUpItem : public CItemBase
{
	inline static const char* MODEL_NAME					= "item attack";	// ���f����.
	inline static const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_attack";	// �G�t�F�N�g��.
	inline static const char* HIT_EFFECT_NAME				= "item_get_attack";	// �G�t�F�N�g��.

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
	const float* pATTACK_UP_POWER;	// �U����(�O������|�C���^�Ŏ擾����).
	const float* pATTACK_UP_TIME;	// �U����(�O������|�C���^�Ŏ擾����).
};

#endif	// #ifndef ATTACK_UP_H.