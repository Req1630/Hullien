#ifndef EDIT_BASE_H
#define EDIT_BASE_H

#include "..\Utility\ImGuiManager\ImGuiManager.h"
#include <vector>

class CEditBase
{
protected:
	const ImVec2 WINDOW_SIZE		= { 510.0f, WND_H-70.0f };	// �E�B���h�E�T�C�Y.
	const ImVec2 RENDER_POSITION	= { 0.0f, 70.0f };		// �`����W.

public:
	CEditBase();
	virtual ~CEditBase();

	// �������֐�.
 	virtual bool Init() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;
	// ���f���̕`��.
	virtual void ModelRender() = 0;
};

#endif	// #ifndef EDIT_BASE_H.