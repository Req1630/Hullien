/**
* @file MoveSpeedUp.h.
* @brief �ړ����x�A�b�v�A�C�e���N���X.
* @author ���c���.
*/
#ifndef MOVE_SPEED_UP_H
#define MOVE_SPEED_UP_H

#include "..\..\ItemBase.h"

/********************************************
*	�ړ����x�A�b�v�A�C�e���N���X.
**/
class CMoveSpeedUpItem : public CItemBase
{
	inline static const char* MODEL_NAME					= "item speed";	// ���f����.
	inline static const char* DROP_AND_AVTIVE_EFFECT_NAME	= "item_effect_speed";	// �G�t�F�N�g��.
	inline static const char* HIT_EFFECT_NAME				= "item_get_speed";	// �G�t�F�N�g��.

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
	const float* pMOVE_VALUE;	// �ړ���(�O������|�C���^�Ŏ擾����).
	const float* pMOVE_UP_TIME;	// �ړ���UP����(�O������|�C���^�Ŏ擾����).
};

#endif	// #ifndef MOVE_SPEED_UP_H.