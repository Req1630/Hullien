#ifndef ALIEN_PARAM_EDIT_H
#define ALIEN_PARAM_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Alien\Alien.h"

#include <vector>

class CAlienParamEdit : public CEditBase
{
	const char* FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";

	const char* A_MODEL_NAME = "a_s";
	const char* B_MODEL_NAME = "b_s";
	const char* C_MODEL_NAME = "c_s";
	const char* D_MODEL_NAME = "ultemate_s";
public:
	CAlienParamEdit();
	~CAlienParamEdit();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;

private:
	// �S�t�@�C���̓ǂݍ���.
	bool FileAllReading();
	// �t�@�C���̓ǂݍ���.
	bool FileReading( const char* fileName, CAlien::SAlienParam& param );
	// �t�@�C���̏�������.
	bool FileWriting( const char* fileName, CAlien::SAlienParam& param );
	// �X�|�[�����̕`��.
	void SpawnParamRender( const int& index );
	// ���f���̏�����.
	bool GetModel();

private:
	std::vector<CAlien::SAlienParam>				m_AlienParamList;
	std::vector<std::shared_ptr<CDX9SkinMesh>>		m_pSkinMeshs;
	std::vector<std::string>						m_AlienPathList;
	std::vector<std::string>						m_AlienNameList;
	std::vector<D3DXVECTOR4>						m_AlienNameColorList;
	int m_Index;

};

#endif	// #ifndef ALIEN_PARAM_EDIT_H.