#include "D3DX11.h"

ID3D11Device* CDirectX11::m_pDevice11 = nullptr;
ID3D11DeviceContext* CDirectX11::m_pContext11 = nullptr;

CDirectX11::CDirectX11()
	: m_hWnd					( nullptr )
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
// DirectX11構築関数.
//-----------------------------------.
HRESULT CDirectX11::Create( HWND hWnd )
{
	m_hWnd = hWnd;

	if( FAILED(InitDevice11()) )	return E_FAIL;
	if( FAILED(InitTexRTV()) )		return E_FAIL;
	if( FAILED(InitDSTex()) )		return E_FAIL;
	if( FAILED(InitViewports()) )	return E_FAIL;
	if( FAILED(InitRasterizer()) )	return E_FAIL;

	return S_OK;
}

//-----------------------------------.
// DirectX11解放関数.
//-----------------------------------.
HRESULT CDirectX11::Release()
{
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pContext11);
	SAFE_RELEASE(m_pDevice11);

	return S_OK;
}

//-----------------------------------.
// クリアバックバッファ.
//-----------------------------------.
void CDirectX11::ClearBackBuffer()
{
	// カラーバックバッファ.
	m_pContext11->ClearRenderTargetView( m_pBackBuffer_TexRTV, CLEAR_BACK_COLOR );

	// デプスステンシルバッファ.
	m_pContext11->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}

//-----------------------------------.
// スワップチェーンプレゼント.
//-----------------------------------.
void CDirectX11::SwapChainPresent()
{
	m_pSwapChain->Present( 0, 0 ); 
}

//-----------------------------------.
// デバイス11の作成.
//-----------------------------------.
HRESULT CDirectX11::InitDevice11()
{
	// スワップチェーン構造体.
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferCount			= 1;								// バックバッファの数.
	sd.BufferDesc.Width		= WND_W;							// バックバッファの幅.
	sd.BufferDesc.Height	= WND_H;							// バックバッファの高さ.
	sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;		// フォーマット(32ﾋﾞｯﾄｶﾗｰ).
	sd.BufferDesc.RefreshRate.Numerator		= 60;				// リフレッシュレート(分母) ※FPS:60.
	sd.BufferDesc.RefreshRate.Denominator	= 1;				// リフレッシュレート(分子).
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 使い方(表示先).
	sd.OutputWindow			= m_hWnd;							// ウィンドウハンドル.
	sd.SampleDesc.Count		= 1;								// マルチサンプルの数.
	sd.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	sd.Windowed				= TRUE;								// ウィンドウモード(フルスクリーン時はFALSE).

	//作成を試みる機能レベルの優先を指定.
	// (GPUがサポートする機能ｾｯﾄの定義).
	// D3D_FEATURE_LEVEL列挙型の配列.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 の GPUレベル.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;	// 配列の要素数.

	auto createDevice = [&]( const D3D_DRIVER_TYPE& type ) -> HRESULT
	{
		return D3D11CreateDeviceAndSwapChain(
			nullptr,			// ビデオアダプタへのポインタ.
			type,				// 作成するﾃデバイスの種類.
			nullptr,			// ソフトウェア ラスタライザを実装するDLLのハンドル.
			0,					// 有効にするランタイムレイヤー.
			&pFeatureLevels,	// 作成を試みる機能レベルの順序を指定する配列へのポインタ.
			1,					// ↑の要素数.
			D3D11_SDK_VERSION,	// SDKのバージョン.
			&sd,				// スワップチェーンの初期化パラメータのポインタ.
			&m_pSwapChain,		// (out) レンダリングに使用するスワップチェーン.
			&m_pDevice11,		// (out) 作成されたデバイス.
			pFeatureLevel,		// 機能ﾚﾍﾞﾙの配列にある最初の要素を表すポインタ.
			&m_pContext11 );	// (out) デバイスコンテキスト.
	};

	if( FAILED(createDevice(D3D_DRIVER_TYPE_HARDWARE)) ){
		if( FAILED(createDevice(D3D_DRIVER_TYPE_WARP)) ){
			if( FAILED(createDevice(D3D_DRIVER_TYPE_REFERENCE)) ){
				MessageBox( nullptr,
					"デバイスとスワップチェーン作成失敗",
					"Error", MB_OK );
				return E_FAIL;
			}
		}
	}
	pFeatureLevel = nullptr;

	return S_OK;
}

//-----------------------------------.
// レンダーターゲットビューの作成.
//-----------------------------------.
HRESULT CDirectX11::InitTexRTV()
{
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けされたGUIDを取得.
		(LPVOID*)&pBackBuffer_Tex);	//(out)ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬに対してﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ(RTV)を作成.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.

	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬを解放.
	SAFE_RELEASE(pBackBuffer_Tex);

	return S_OK;
}

//-----------------------------------.
// ステンシルビューの作成.
//-----------------------------------.
HRESULT CDirectX11::InitDSTex()
{
	int wnd_Width = WND_W;
	int wnd_Height = WND_H;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= wnd_Width;				// 幅.
	descDepth.Height				= wnd_Height;				// 高さ.
	descDepth.MipLevels				= 1;						// ﾐｯﾌﾟﾏｯﾌﾟﾚﾍﾞﾙ:1.
	descDepth.ArraySize				= 1;						// 配列数:1.
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;	// 32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.
	descDepth.SampleDesc.Count		= 1;						// ﾏﾙﾁｻﾝﾌﾟﾙの数.
	descDepth.SampleDesc.Quality	= 0;						// ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	descDepth.Usage					= D3D11_USAGE_DEFAULT;		// 使用方法:ﾃﾞﾌｫﾙﾄ.
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;	// 深度(ｽﾃﾝｼﾙとして使用).
	descDepth.CPUAccessFlags		= 0;						// CPUからはｱｸｾｽしない.
	descDepth.MiscFlags				= 0;						// その他の設定なし.


	// そのﾃｸｽﾁｬに対してﾃﾞﾌﾟｽｽﾃﾝｼﾙ(DSTex)を作成.
	if( FAILED( m_pDevice11->CreateTexture2D( &descDepth, nullptr, &m_pBackBuffer_DSTex )) ){
		_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
		return E_FAIL;
	}
	// そのﾃｸｽﾁｬに対してﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ(DSV)を作成.
	if( FAILED( m_pDevice11->CreateDepthStencilView( m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV)) ){
		_ASSERT_EXPR( false, L"デプスステンシルビュー作成失敗" );
		return E_FAIL;
	}
	// ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰとﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰをﾊﾟｲﾌﾟﾗｲﾝにｾｯﾄ.
	m_pContext11->OMSetRenderTargets( 1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV );
	return S_OK;
}

//-----------------------------------.
// ビューポートの作成.
//-----------------------------------.
HRESULT CDirectX11::InitViewports()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (FLOAT)WND_W;	// 幅.
	vp.Height	= (FLOAT)WND_H;	// 高さ.
	vp.MinDepth = 0.0f;			// 最小深度(手前).
	vp.MaxDepth = 1.0f;			// 最大深度(奥).
	vp.TopLeftX = 0.0f;			// 左上位置x.
	vp.TopLeftY = 0.0f;			// 左上位置y.

	m_pContext11->RSSetViewports( 1, &vp );
	return S_OK;
}

//-----------------------------------.
// ラスタライザの作成.
//-----------------------------------.
HRESULT CDirectX11::InitRasterizer()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode				= D3D11_FILL_SOLID;	// 塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode				= D3D11_CULL_NONE;	// BACK:背面を描画しない, FRONT:正面を描画しない.
	rdc.FrontCounterClockwise	= FALSE;			// ﾎﾟﾘｺﾞﾝの表裏を決定するﾌﾗｸﾞ.
	rdc.DepthClipEnable			= FALSE;			// 距離についてのｸﾘｯﾋﾟﾝｸﾞ有効.

	ID3D11RasterizerState* pRs = nullptr;
	if( FAILED( m_pDevice11->CreateRasterizerState( &rdc, &pRs )) ){
		_ASSERT_EXPR( false, L"ラスタライザー作成失敗" );
	}
	m_pContext11->RSSetState( pRs );
	SAFE_RELEASE( pRs );

	return S_OK;
}