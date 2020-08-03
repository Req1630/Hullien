#ifndef SPAWN_EDIT_H
#define SPAWN_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include <vector>

class CDX9StaticMesh;
struct stSpawnUFOParam;

class CSpawnEdit
{
	const ImVec2 WINDOW_SIZE		= { 450.0f, 350.0f };	// �E�B���h�E�T�C�Y.
	const ImVec2 RENDER_POSITION	= { 260.0f, 20.0f };		// �`����W.
	const char* FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
public:
	CSpawnEdit();
	~CSpawnEdit();

	// �������֐�.
	bool Init();
	// �`��֐�.
	void Render();

private:
	// �t�@�C���̓ǂݍ���.
	bool FileReading();
	// �t�@�C���̏�������.
	bool FileWriting();
	// �C���f�b�N�X�̕`��.
	void IndexRender();
	// �X�|�[�����̕`��.
	void SpawnParamRender( const int& index );

private:
	std::vector<stSpawnUFOParam> m_SpawnPramList;
	std::shared_ptr<CDX9StaticMesh> m_pStaticMesh;
	bool m_IsSucceeded;
	int m_SucceedRenderCount;
	int m_Index;
};

#endif	// #ifndef SPAWN_EDIT_H.