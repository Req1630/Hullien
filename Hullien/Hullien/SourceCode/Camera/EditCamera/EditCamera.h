#ifndef EDIT_CAMERA_H
#define EDIT_CAMERA_H

#include "..\Camera.h"
#include <memory>

class CMouse;	// �}�E�X�N���X.

/****************************************
* �G�f�B�^�p�J����.
**/
class CEditCamera : public CCamera
{
public:
	CEditCamera();
	virtual ~CEditCamera();

	// �X�V�֐�.
	void Updata();
	// �E�B���h�E�n���h���̎擾.
	void SetHWND( HWND hWnd ){ m_hWnd = hWnd; }

private:
	// �}�E�X�̍X�V.
	void MouseUpdate();

private:
	HWND					m_hWnd;		// �E�B���h�E�n���h��.
	std::unique_ptr<CMouse> m_pMouse;	// �}�E�X�N���X.
	D3DXVECTOR2				m_Radian;	// ���W�A��.
};

#endif	// #ifndef EDIT_CAMERA_H.