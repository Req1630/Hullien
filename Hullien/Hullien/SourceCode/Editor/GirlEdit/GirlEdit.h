/**
* @file GirlEdit.h.
* @brief ���̎q�̃p�����[�^�G�f�B�^�N���X.
* @author ���c���.
*/
#ifndef GIRL_EDIT_H
#define GIRL_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Girl\Girl.h"

/***********************************************
*	���̎q�̃p�����[�^�G�f�B�^�N���X.
**/
class CGirlEdit : public CEditBase
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Girl\\Girl.bin";
public:
	CGirlEdit();
	virtual ~CGirlEdit();

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
	std::unique_ptr<CGirl>	m_pGirl;
	SGirlParam				m_GirlParam;
};


#endif	// #ifndef GIRL_EDIT_H.