#include "D3DX9.h"

LPDIRECT3DDEVICE9 CDirectX9::m_pDevice9 = nullptr;

CDirectX9::CDirectX9()
	: m_hWnd	( nullptr )
{
}

CDirectX9::~CDirectX9()
{
}

//-----------------------------------.
// DirectX9�̍\�z.
//-----------------------------------.
HRESULT CDirectX9::Create( HWND hWnd )
{
	m_hWnd = hWnd;

	if( FAILED( CreateDevice9() )) return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// DirectX9�̉��.
//-----------------------------------.
void CDirectX9::Release()
{
	SAFE_RELEASE( m_pDevice9 );
	m_hWnd = nullptr;
}

//-----------------------------------.
// �f�o�C�X9�̍쐬.
//-----------------------------------.
HRESULT CDirectX9::CreateDevice9()
{
	LPDIRECT3D9 m_pD3d9 = nullptr; // Dx9�I�u�W�F�N�g.

	//�uDirect3D�v�I�u�W�F�N�g�̍쐬.
	m_pD3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3d9 == nullptr ){
		_ASSERT_EXPR( false, L"Dx9�I�u�W�F�N�g�쐬���s" );
		MessageBox( nullptr, "Dx9�I�u�W�F�N�g�쐬���s", "�x��", MB_OK );
		return E_FAIL;
	}

	// Diret3D �f�o�C�X�I�u�W�F�N�g�̍쐬.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g.
	d3dpp.BackBufferCount = 1;					// �o�b�N�o�b�t�@�̐�.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �X���b�v�G�t�F�N�g.
	d3dpp.Windowed = true;						// �E�B���h�E���[�h.
	d3dpp.EnableAutoDepthStencil = true;		// �X�e���V���L��.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// �X�e���V���̃t�H�[�}�b�g(16bit).

	auto createDevice = [&]( const D3DDEVTYPE& deviceType, const DWORD& behaviorFlags )
	{
		return m_pD3d9->CreateDevice(
			D3DADAPTER_DEFAULT, 
			deviceType, 
			m_hWnd,
			behaviorFlags,
			&d3dpp, 
			&m_pDevice9 );
	};

	// �f�o�C�X�쐬
	// (HALӰ��:�`��ƒ��_������GPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (HALӰ��:�`���GPU�A���_������CPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}
	MessageBox( nullptr, "HALӰ�ނ����޲��쐬�ł��܂���\nREFӰ�ނōĎ��s���܂�", "�x��", MB_OK );

	// (REFӰ��:�`���CPU�A���_������GPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_HARDWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	// (REFӰ��:�`��ƒ��_������CPU�ōs��).
	if( SUCCEEDED( createDevice( D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING ) )){
		SAFE_RELEASE( m_pD3d9 );
		return S_OK;
	}

	MessageBox( nullptr, "Direct3D���޲��쐬���s", "�x��", MB_OK );

	SAFE_RELEASE( m_pD3d9 );
	return E_FAIL;
}