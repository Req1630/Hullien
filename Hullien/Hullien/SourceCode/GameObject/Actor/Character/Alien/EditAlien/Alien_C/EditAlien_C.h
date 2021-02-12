/**
* @file EditAlien_C.h.
* @brief �G�f�B�^�p�F���lC�N���X.
* @author ���c���.
*/
#ifndef EDIT_ALIEN_C_H
#define EDIT_ALIEN_C_H

#include "..\EditAlien.h"

/******************************************
*	�G�f�B�^�p�F���lC�N���X.
**/
class CEditAlienC : public CEditAlien
{
	inline static const char* MODEL_NAME = "c_s";
public:
	CEditAlienC();
	virtual	~CEditAlienC();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;

	// �p�����[�^�[�̐ݒ�.
	virtual void SetParamter( const SAlienParam& param ) override;

private:
	// �X�|�[��.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct() override;
	// ����.
	virtual void Fright() override;
	// ���S.
	virtual void Death() override;
	// ������.
	virtual void Escape() override;
	// �����蔻��̐ݒ�.
	bool ColliderSetting();
};

#endif	// #ifndef EDIT_ALIEN_C_H.
