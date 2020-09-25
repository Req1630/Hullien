#ifndef D3DX_H
#define D3DX_H

#include "..\..\Global.h"
#include <vector>

class CDirectX11
{
private: //----- �萔 ------.
	const float CLEAR_BACK_COLOR[4] = { 0.6f, 0.6f, 0.6f, 1.0f };	// �o�b�N�J���[.

public: //----- �֐� ------.
	CDirectX11();
	~CDirectX11();

	// �C���X�^���X�̎擾.
	static CDirectX11* GetInstance();

	// DirectX11�\�z�֐�.
	static HRESULT Create( HWND hWnd );
	// DirectX11����֐�.
	static HRESULT Release();

	// �N���A�o�b�N�o�b�t�@.
	static void ClearBackBuffer();
	// �X���b�v�`�F�[���v���[���g.
	static void SwapChainPresent();

	// �f�o�C�X11�̎擾�֐�.
	static ID3D11Device*		GetDevice() { return GetInstance()->m_pDevice11; }
	// �R���e�L�X�g11�̎擾�֐�.
	static ID3D11DeviceContext*	GetContext() { return GetInstance()->m_pContext11; }
	// �f�v�X�X�e���V���r���[�̎擾.
	static ID3D11DepthStencilView* GetDepthSV(){ return GetInstance()->m_pBackBuffer_DSTexDSV; }
	// BackBuffer�̐ݒ�.
	static void SetBackBuffer();

private: //----- �֐� ------.
		 // �f�o�C�X11�̍쐬.
	HRESULT InitDevice11();
	// �����_�[�^�[�Q�b�g�r���[�̍쐬.
	HRESULT InitTexRTV();
	// �X�e���V���r���[�̍쐬.
	HRESULT InitDSTex();
	// �r���[�|�[�g�̍쐬.
	HRESULT InitViewports();
	// ���X�^���C�U�̍쐬.
	HRESULT InitRasterizer();

private: //----- �ϐ� ------.
	HWND	m_hWnd;	// �E�B���h�E�n���h��.

	ID3D11Device*			m_pDevice11;	// �f�o�C�X11.
	ID3D11DeviceContext*	m_pContext11;	// �R���e�L�X�g11.

	IDXGISwapChain*			m_pSwapChain;			// �X���b�v�`�F�[��.

	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;	// �����_�[�^�[�Q�b�g�r���[.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;	// �X�e���V���r���[.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;	// �e�N�X�`���[2D.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDirectX11( const CDirectX11 & )				= delete;
	CDirectX11& operator = ( const CDirectX11 & )	= delete;
	CDirectX11( CDirectX11 && )						= delete;
	CDirectX11& operator = ( CDirectX11 && )		= delete;
};

#endif	// #ifndef D3DX_H.