/**
* @file PlayerEdit.h.
* @brief �v���C���[�̃p�����[�^�G�f�B�^�N���X.
* @author ���c���.
*/
#ifndef PLAYER_EDIT_H
#define PLAYER_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Player\Player.h"

class CEditAlienA;

/**********************************************
*	�v���C���[�̃p�����[�^�G�f�B�^�N���X.
*/
class CPlayerEdit : public CEditBase
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
public:
	CPlayerEdit();
	virtual ~CPlayerEdit();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;

private:
	// �p�����[�^�̐ݒ�.
	void PlayerParamEdit();
	// �A�j���[�V�����̈������蒲���̐ݒ�.
	void AnimAdjDrggingParamEdit();
	// �U�������̐ݒ�.
	void AttackAdjParamEdit();

private:
	std::unique_ptr<CPlayer>			m_pPlayer;
	std::shared_ptr<CEditAlienA>		m_pAlien;
	player::SPlayerParam				m_pPlayerParam;
	player::SAnimDraggingAdjParam		m_AnimAdjDrggingParam;
	player::SAttackAdjParam				m_AttackAdjParam;
};

#endif	// #ifndef PLAYER_EDIT_H.
