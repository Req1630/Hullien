#ifndef ALIEN_PARAM_EDIT_H
#define ALIEN_PARAM_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Alien\Alien.h"

#include <vector>

class CAlienParamEdit : public CEditBase
{
	const char* FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";

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

private:
	std::vector<CAlien::SAlienParam> m_AlienParamList;
	std::vector<std::string> m_AlienPathList;
	std::vector<std::string> m_AlienNameList;
	int m_Index;

};

#endif	// #ifndef ALIEN_PARAM_EDIT_H.