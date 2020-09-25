#ifndef D3DX_H
#define D3DX_H

#include "..\..\Global.h"
#include <vector>

class CDirectX11
{
public: //----- �萔 ------.
	const int MAX_CASCADE = 4;	// �J�X�P�[�h�̍ő吔.

private: //----- �萔 ------.
	const float CLEAR_BACK_COLOR[4] = { 0.6f, 0.6f, 0.6f, 1.0f };	// �o�b�N�J���[.

	const int G_BUFFER_NUM = 3;	// �o�b�t�@�̐�.

	enum enGBUFFER
	{
		enGBUFFER_COLOR,
		enGBUFFER_NORMAL,
		enGBUFFER_Z_DEPTH,
		enGBUFFER_MAX,

	} typedef EGBUFFER;

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

	// �[�x�o�b�t�@�̐ݒ�.
	static void SetZBuffer( int i );
	// G-Buffer�̐ݒ�.
	static void SetGBuufer();
	// BackBuffer�̐ݒ�.
	static void SetBackBuffer();
	// TransBuffer�̐ݒ�.
	static void SetTransBuffer();

	// �[�x�o�b�t�@�e�N�X�`�����擾.
	static std::vector<ID3D11ShaderResourceView*> GetZBuffer(){ return GetInstance()->m_pZBufferSRV; }
	// G-Buffer�e�N�X�`�����擾.
	static std::vector<ID3D11ShaderResourceView*> GetGBuffer(){ return GetInstance()->m_pGBufferSRV; }
	// TransBuffer�e�N�X�`�����擾.
	static ID3D11ShaderResourceView* GetTransBaffer(){ return GetInstance()->m_pTransBufferSRV; }

private: //----- �֐� ------.
		 // �f�o�C�X11�̍쐬.
	HRESULT InitDevice11();
	// �����_�[�^�[�Q�b�g�r���[�̍쐬.
	HRESULT InitTexRTV();
	// �X�e���V���r���[�̍쐬.
	HRESULT InitDSTex();
	// Z-Buffer�̍쐬.
	HRESULT InitZBufferTex();
	// G-Buffer�̍쐬.
	HRESULT InitGBufferTex();
	// 
	HRESULT InitTransBufferTex();
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

	std::vector<ID3D11RenderTargetView*>	m_pZBufferRTV;	// �[�x�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[.
	std::vector<ID3D11ShaderResourceView*>	m_pZBufferSRV;	// �[�x�o�b�t�@�̃X�e���V���r���[.
	std::vector<ID3D11Texture2D*>			m_pZBufferTex;	// �[�x�o�b�t�@�̃e�N�X�`���[2D.

	std::vector<ID3D11RenderTargetView*>	m_pGBufferRTV;	// G-Buffer�̃����_�[�^�[�Q�b�g�r���[.
	std::vector<ID3D11ShaderResourceView*>	m_pGBufferSRV;	// G-Buffer�̃V�F�[�_�[���\�[�X�r���[.
	std::vector<ID3D11Texture2D*>			m_pGBufferTex;	// G-Buffer�̃e�N�X�`��2D.

	ID3D11RenderTargetView*		m_pTransBufferRTV;	// �������`��p�̃����_�[�^�[�Q�b�g�r���[.
	ID3D11ShaderResourceView*	m_pTransBufferSRV;	// �������`��p�̃V�F�[�_�[���\�[�X�r���[.
	ID3D11Texture2D*			m_pTransBufferTex;	// �������`��p�̃e�N�X�`��2D.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CDirectX11( const CDirectX11 & )				= delete;
	CDirectX11& operator = ( const CDirectX11 & )	= delete;
	CDirectX11( CDirectX11 && )						= delete;
	CDirectX11& operator = ( CDirectX11 && )		= delete;
};

#endif	// #ifndef D3DX_H.