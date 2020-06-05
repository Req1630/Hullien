#ifndef D3DX9_H
#define D3DX9_H

#include "..\..\Global.h"

/*********************************
*	DirectX9
*/
class CDirectX9
{
public:
	CDirectX9();
	~CDirectX9();

	// DirectX9�̍\�z.
	HRESULT Create( HWND hWnd );
	// DirectX9�̉��.
	void Release();

	// �f�o�C�X�̎擾�֐�.
	static LPDIRECT3DDEVICE9 GetDevice(){ return m_pDevice9; }

private:
	// �f�o�C�X9�̍\�z.
	HRESULT CreateDevice9();

private:
	static LPDIRECT3DDEVICE9	m_pDevice9;	// �f�o�C�X9.
	HWND	m_hWnd;	// �E�B���h�E�n���h��.
};

#endif	// #ifndef D3DX9_H.