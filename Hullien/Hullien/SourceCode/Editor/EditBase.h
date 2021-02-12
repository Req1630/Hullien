/**
* @file EditBase.h.
* @brief �G�f�B�b�g���N���X.
* @author ���c���.
*/
#ifndef EDIT_BASE_H
#define EDIT_BASE_H

#include "..\Utility\ImGuiManager\ImGuiManager.h"
#include <vector>

/***************************************
*	�G�f�B�b�g���N���X.
**/
class CEditBase
{
protected:
	inline static const ImVec2 WINDOW_SIZE		= { 510.0f, WND_H-70.0f };	// �E�B���h�E�T�C�Y.
	inline static const ImVec2 RENDER_POSITION	= { 0.0f, 70.0f };		// �`����W.

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
	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() = 0;

	// �J�������Z�b�g���Ă��邩�ǂ���.
	inline bool IsSetCamera(){ return m_IsSetCamera; }

protected:
	bool m_IsSetCamera;
};

#endif	// #ifndef EDIT_BASE_H.