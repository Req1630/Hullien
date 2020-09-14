#ifndef ALIEN_PARAM_EDIT_H
#define ALIEN_PARAM_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\GameObject\Actor\Character\Alien\Alien.h"

#include <vector>

class CAlienParamEdit
{
	const ImVec2 WINDOW_SIZE = { 510.0f, WND_H-70.0f };	// �E�B���h�E�T�C�Y.
	const ImVec2 RENDER_POSITION = { 0.0f, 70.0f };		// �`����W.
	const char* FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";

public:
	CAlienParamEdit();
	~CAlienParamEdit();

	// �������֐�.
	bool Init();
	// �`��֐�.
	void Render();

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