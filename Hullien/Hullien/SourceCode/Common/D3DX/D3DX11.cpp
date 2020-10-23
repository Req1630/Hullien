#include "D3DX11.h"

CDirectX11::CDirectX11()
	: m_hWnd					( nullptr )
	, m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pSwapChain				( nullptr )
	, m_pBackBuffer_TexRTV		( nullptr )
	, m_pBackBuffer_DSTex		( nullptr )
	, m_pBackBuffer_DSTexDSV	( nullptr )
{
}

CDirectX11::~CDirectX11()
{
}

//-----------------------------------.
// �C���X�^���X�̎擾.
//-----------------------------------.
CDirectX11* CDirectX11::GetInstance()
{
	static std::unique_ptr<CDirectX11> pInstance = 
		std::make_unique<CDirectX11>();
	return pInstance.get();
}

//-----------------------------------.
// DirectX11�\�z�֐�.
//-----------------------------------.
HRESULT CDirectX11::Create( HWND hWnd )
{
	GetInstance()->m_hWnd = hWnd;

	if( FAILED(GetInstance()->InitDevice11()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitTexRTV()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitDSTex()) )		return E_FAIL;
	if( FAILED(GetInstance()->InitViewports()) )	return E_FAIL;
	if( FAILED(GetInstance()->InitRasterizer()) )	return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// DirectX11����֐�.
//-----------------------------------.
HRESULT CDirectX11::Release()
{

	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTexDSV );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_DSTex );
	SAFE_RELEASE( GetInstance()->m_pBackBuffer_TexRTV );
	SAFE_RELEASE( GetInstance()->m_pSwapChain );
	SAFE_RELEASE( GetInstance()->m_pContext11 );
	SAFE_RELEASE( GetInstance()->m_pDevice11 );

	return S_OK;
}

//-----------------------------------.
// �N���A�o�b�N�o�b�t�@.
//-----------------------------------.
void CDirectX11::ClearBackBuffer()
{
	// �J���[�o�b�N�o�b�t�@.
	GetInstance()->m_pContext11->ClearRenderTargetView( 
		GetInstance()->m_pBackBuffer_TexRTV, GetInstance()->CLEAR_BACK_COLOR );;
}

//-----------------------------------.
// �X���b�v�`�F�[���v���[���g.
//-----------------------------------.
void CDirectX11::SwapChainPresent()
{
	GetInstance()->m_pSwapChain->Present( 0, 0 );
}

//-----------------------------------.
// BackBuffer�̐ݒ�.
//-----------------------------------.
void CDirectX11::SetBackBuffer()
{
	// �����_�[�^�[�Q�b�g�̐ݒ�.
	GetInstance()->m_pContext11->OMSetRenderTargets( 
		1, 
		&GetInstance()->m_pBackBuffer_TexRTV,
		GetInstance()->m_pBackBuffer_DSTexDSV );
	// �f�v�X�X�e���V���o�b�t�@.
	GetInstance()->m_pContext11->ClearDepthStencilView(
		GetInstance()->m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}

//-----------------------------------.
// �t���X�N���[���̐ݒ�.
//-----------------------------------.
bool CDirectX11::SetFullScreen( const bool& isOn )
{
	// ���݂̃X�N���[�������擾.
	BOOL isState = FALSE;
	GetInstance()->m_pSwapChain->GetFullscreenState( &isState, nullptr );
	if( isOn == true ){
		// ���݂̏�Ԃ��E�B���h�E��ԂȂ�.
		if( isState == FALSE ){
			// �t���X�N���[���ɕύX.
			GetInstance()->m_pSwapChain->SetFullscreenState( TRUE, nullptr );
		}
		return true;
	} else {
		// ���݂̏�Ԃ��t���X�N���[���Ȃ�.
		if( isState == TRUE ){
			// �E�B���h�E�ɕύX.
			GetInstance()->m_pSwapChain->SetFullscreenState( FALSE, nullptr );
		}
		return false;
	}
	return true;
}

//-----------------------------------.
// �f�o�C�X11�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitDevice11()
{
#if 0	// �A���`�G�C���A�X����.
	//�f�o�C�X�̐���
	HRESULT hr;
	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&m_pDevice11,
		NULL,
		&m_pContext11);
	if(FAILED(hr)){
		MessageBoxW(m_hWnd, L"D3D11CreateDevice", L"Err", MB_ICONSTOP);
	}

	for(int i=0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++){
		UINT Quality;
		if( SUCCEEDED(
			m_pDevice11->CheckMultisampleQualityLevels(
				DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality ))){
			if(0 < Quality){
				m_MSAASampleDesc.Count = i;
				m_MSAASampleDesc.Quality = Quality - 1;
			}
		}
	}

	//�C���^�[�t�F�[�X�擾
	IDXGIDevice1* hpDXGI = NULL;
	if(FAILED(m_pDevice11->QueryInterface(__uuidof(IDXGIDevice1), (void**)&hpDXGI))){
		MessageBoxW(m_hWnd, L"QueryInterface", L"Err", MB_ICONSTOP);
	}

	//�A�_�v�^�[�擾
	IDXGIAdapter* hpAdapter = NULL;
	if(FAILED(hpDXGI->GetAdapter(&hpAdapter))){
		MessageBoxW(m_hWnd, L"GetAdapter", L"Err", MB_ICONSTOP);
	}

	//�t�@�N�g���[�擾
	IDXGIFactory* hpDXGIFactory = NULL;
	hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&hpDXGIFactory);
	if(hpDXGIFactory == NULL){
		MessageBoxW(m_hWnd, L"GetParent", L"Err", MB_ICONSTOP);
	}

	//�X���b�v�`�F�C���쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width	= WND_W;
	swapChainDesc.BufferDesc.Height	= WND_H;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc	= m_MSAASampleDesc;
	swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount	= 1;
	swapChainDesc.OutputWindow	= m_hWnd;
	swapChainDesc.Windowed		= TRUE;
	swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if(FAILED(hpDXGIFactory->CreateSwapChain(m_pDevice11, &swapChainDesc, &m_pSwapChain))){
		MessageBoxW(m_hWnd, L"CreateSwapChain", L"Err", MB_ICONSTOP);
	}
#else
	// �X���b�v�`�F�[���\����.
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferCount			= 1;								// �o�b�N�o�b�t�@�̐�.
	sd.BufferDesc.Width		= WND_W;							// �o�b�N�o�b�t�@�̕�.
	sd.BufferDesc.Height	= WND_H;							// �o�b�N�o�b�t�@�̍���.
	sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;		// �t�H�[�}�b�g(32�ޯĶװ).
	sd.BufferDesc.RefreshRate.Numerator		= 60;				// ���t���b�V�����[�g(����) ��FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;				// ���t���b�V�����[�g(���q).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �g����(�\����).
	sd.OutputWindow			= m_hWnd;							// �E�B���h�E�n���h��.
	sd.SampleDesc.Count		= 1;								// �}���`�T���v���̐�.
	sd.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	sd.Windowed				= TRUE;								// �E�B���h�E���[�h(�t���X�N���[������FALSE).

	//�쐬�����݂�@�\���x���̗D����w��.
	// (GPU���T�|�[�g����@�\��Ă̒�`).
	// D3D_FEATURE_LEVEL�񋓌^�̔z��.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 �� GPU���x��.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;	// �z��̗v�f��.

	auto createDevice = [&]( const D3D_DRIVER_TYPE& type ) -> HRESULT
	{
		return D3D11CreateDeviceAndSwapChain(
			nullptr,			// �r�f�I�A�_�v�^�ւ̃|�C���^.
			type,				// �쐬����Ãf�o�C�X�̎��.
			nullptr,			// �\�t�g�E�F�A ���X�^���C�U����������DLL�̃n���h��.
			0,					// �L���ɂ��郉���^�C�����C���[.
			&pFeatureLevels,	// �쐬�����݂�@�\���x���̏������w�肷��z��ւ̃|�C���^.
			1,					// ���̗v�f��.
			D3D11_SDK_VERSION,	// SDK�̃o�[�W����.
			&sd,				// �X���b�v�`�F�[���̏������p�����[�^�̃|�C���^.
			&m_pSwapChain,		// (out) �����_�����O�Ɏg�p����X���b�v�`�F�[��.
			&m_pDevice11,		// (out) �쐬���ꂽ�f�o�C�X.
			pFeatureLevel,		// �@�\���ق̔z��ɂ���ŏ��̗v�f��\���|�C���^.
			&m_pContext11 );	// (out) �f�o�C�X�R���e�L�X�g.
	};

	if( FAILED(createDevice(D3D_DRIVER_TYPE_HARDWARE)) ){
		if( FAILED(createDevice(D3D_DRIVER_TYPE_WARP)) ){
			if( FAILED(createDevice(D3D_DRIVER_TYPE_REFERENCE)) ){
				MessageBox( nullptr,
					"�f�o�C�X�ƃX���b�v�`�F�[���쐬���s",
					"Error", MB_OK );
				return E_FAIL;
			}
		}
	}
	pFeatureLevel = nullptr;

#endif

	return S_OK;
}

//-----------------------------------.
// �����_�[�^�[�Q�b�g�r���[�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitTexRTV()
{
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:���Ɋ֘A�t�����ꂽGUID���擾.
		(LPVOID*)&pBackBuffer_Tex);	//(out)�o�b�N�o�b�t�@�e�N�X�`��.

									//���̃e�N�X�`���ɑ΂��ă����_�[�^�[�Q�b�g�r���[(RTV)���쐬.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.

								// �o�b�N�o�b�t�@�e�N�X�`�������.
	SAFE_RELEASE(pBackBuffer_Tex);

	return S_OK;
}

//-----------------------------------.
// �X�e���V���r���[�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitDSTex()
{
	int wnd_Width = WND_W;
	int wnd_Height = WND_H;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= wnd_Width;				// ��.
	descDepth.Height				= wnd_Height;				// ����.
	descDepth.MipLevels				= 1;						// �~�b�v�}�b�v���x��:1.
	descDepth.ArraySize				= 1;						// �z��:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32�r�b�g�t�H�[�}�b�g.
	descDepth.SampleDesc.Count		= 1;						// �}���`�T���v���̐�.
	descDepth.SampleDesc.Quality	= 0;						// �}���`�T���v���̃N�I���e�B.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// �g�p���@:�f�t�H���g.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// �[�x(�X�e���V���Ƃ��Ďg�p).
	descDepth.CPUAccessFlags		= 0;						// CPU����̓A�N�Z�X���Ȃ�.
	descDepth.MiscFlags				= 0;						// ���̑��̐ݒ�Ȃ�.
//	descDepth.SampleDesc			= m_MSAASampleDesc;


	// ���̃e�N�X�`���ɑ΂��ăf�v�X�X�e���V��(DSTex)���쐬.
	if( FAILED( m_pDevice11->CreateTexture2D( &descDepth, nullptr, &m_pBackBuffer_DSTex )) ){
		_ASSERT_EXPR( false, L"�f�v�X�X�e���V���쐬���s" );
		return E_FAIL;
	}
	// ���̃e�N�X�`���ɑ΂��ăf�u�X�X�e���V���r���[(DSV)���쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilView( m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV)) ){
		_ASSERT_EXPR( false, L"�f�v�X�X�e���V���r���[�쐬���s" );
		return E_FAIL;
	}
	// �����_�[�^�[�Q�b�g�����޽�X�e���V���r���[���p�C�v���C���ɃZ�b�g.
	m_pContext11->OMSetRenderTargets( 1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );
	return S_OK;
}

//-----------------------------------.
// �r���[�|�[�g�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitViewports()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)WND_W;	// ��.
	vp.Height	= (FLOAT)WND_H;	// ����.
	vp.MinDepth = 0.0f;			// �ŏ��[�x(��O).
	vp.MaxDepth = 1.0f;			// �ő�[�x(��).
	vp.TopLeftX = 0.0f;			// ����ʒux.
	vp.TopLeftY = 0.0f;			// ����ʒuy.

	m_pContext11->RSSetViewports( 1, &vp );
	return S_OK;
}

//-----------------------------------.
// ���X�^���C�U�̍쐬.
//-----------------------------------.
HRESULT CDirectX11::InitRasterizer()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode				= D3D11_FILL_SOLID;	// �h��Ԃ�(�\���b�h).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:�w�ʂ�`�悵�Ȃ�, FRONT:���ʂ�`�悵�Ȃ�.
	rdc.FrontCounterClockwise	= FALSE;			// �|���S���̕\�������肷��t���O.
	rdc.DepthClipEnable			= FALSE;			// �����ɂ��ẴN���b�s���O�L��.

	ID3D11RasterizerState* pRs = nullptr;
	if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &pRs )) ){
		_ASSERT_EXPR( false, L"���X�^���C�U�[�쐬���s" );
	}
	m_pContext11->RSSetState( pRs );
	SAFE_RELEASE( pRs );

	return S_OK;
}