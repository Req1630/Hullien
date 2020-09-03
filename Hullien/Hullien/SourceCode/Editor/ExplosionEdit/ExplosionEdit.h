#ifndef EXPLOSION_EDIT_H
#define EXPLOSION_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\GameObject\Actor\Explosion\Explosion.h"
#include <vector>

class CExplosionEdit
{
	const ImVec2 WINDOW_SIZE = { 420.0f, 250.0f };	// �E�B���h�E�T�C�Y.
	const ImVec2 RENDER_POSITION = { 260.0f, 380.0f };		// �`����W.
	const char* FILE_PATH = "Data\\GameParam\\Alien\\Explosion\\Explosion.bin";

public:
	CExplosionEdit();
	~CExplosionEdit();

	// �������֐�.
	bool Init();
	// �`��֐�.
	void Render();

private:
	// �t�@�C���̓ǂݍ���.
	bool FileReading();
	// �t�@�C���̏�������.
	bool FileWriting();

private:
	CExplosion::SExplosionParam m_ExplosionParam;	// �����p�����[�^�[.
};

#endif	// #ifndef EXPLOSION_EDIT_H.