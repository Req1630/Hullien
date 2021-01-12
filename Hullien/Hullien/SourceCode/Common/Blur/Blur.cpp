#include "Blur.h"
#include "..\D3DX\D3DX11.h"

// ブラーシェーダーのエントリ名リスト.
const char* SHADER_ENTRY_NAME_LIST[] =
{
	"PS_HorizontalBlur",	// 横方向.
	"PS_VerticalBlur",		// 縦方向.
};

const int BLUR_TEXTURE_DISCOUNT_SIZE = 2;	// ブラーのテクスチャを割るサイズ.

CBlur::CBlur()
	: m_pBlurBufferRTV		( EBlurSmpleDir_Max )
	, m_pBlurBufferSRV		( EBlurSmpleDir_Max )
	, m_pBlurBufferTex		( EBlurSmpleDir_Max )
	, m_pVertexShader		( nullptr )
	, m_pPixelShaderList	( EBlurSmpleDir_Max )
	, m_pVertexLayout		( nullptr )
	, m_pConstantBuffer		( nullptr )
	, m_pVertexBuffer		( nullptr )
	, m_pSampleLinear		( nullptr )
	, m_WndWidth			( 0 )
	, m_WndHeight			( 0 )
{
}

CBlur::~CBlur()
{
	Release();
}

// 初期化.
HRESULT CBlur::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;
	m_WndWidth = CDirectX11::GetWndWidth();
	m_WndHeight = CDirectX11::GetWndHeight();

	if( FAILED( InitBlurTex() ))			return E_FAIL;
	if( FAILED( CreateShader() ))			return E_FAIL;
	if( FAILED( CreateModel() ))			return E_FAIL;
	if( FAILED( InitSample() ))				return E_FAIL;
	if( FAILED( CreateConstantBuffer() ))	return E_FAIL;

	return S_OK;
}

// 解放.
void CBlur::Release()
{
	for( auto& rtv : m_pBlurBufferRTV ) SAFE_RELEASE(rtv);
	for( auto& srv : m_pBlurBufferSRV ) SAFE_RELEASE(srv);
	for( auto& tex : m_pBlurBufferTex ) SAFE_RELEASE(tex);

	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	for( auto& ps : m_pPixelShaderList ) SAFE_RELEASE( ps );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}

// テクスチャをサンプリングする.
void CBlur::Sampling( ID3D11ShaderResourceView* tex )
{
	ID3D11ShaderResourceView* srv = tex;
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// サンプラのセット.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	int i = 0;
	for( auto& rtv : m_pBlurBufferRTV ){
		// レンダーターゲットの設定.
		m_pContext11->OMSetRenderTargets( 1, &rtv, CDirectX11::GetDepthSV() );
		// デプスステンシルバッファ.
		m_pContext11->ClearDepthStencilView( CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

		m_pContext11->PSSetShader( m_pPixelShaderList[i], nullptr, 0 );	// ピクセルシェーダ.
		m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
		m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
		m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.

		m_pContext11->PSSetShaderResources( 0, 1, &srv );
		m_pContext11->Draw( 4, 0 );

		srv = m_pBlurBufferSRV[i];
		i++;
	}
	ID3D11ShaderResourceView *const pSRV3[1] = { nullptr };
	m_pContext11->PSSetShaderResources( 0, 1, pSRV3 );
	ID3D11RenderTargetView* rtv[1] = { nullptr };
	m_pContext11->OMSetRenderTargets( 1, &rtv[0], nullptr );
}

// バッファのクリア.
void CBlur::ClearBuffer()
{
	for( auto& rtv : m_pBlurBufferRTV ){
		m_pContext11->ClearRenderTargetView( rtv, CLEAR_BACK_COLOR );
	}
}

// ブラー用の作成.
HRESULT CBlur::InitBlurTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= m_WndWidth/BLUR_TEXTURE_DISCOUNT_SIZE;	// 幅.
	texDesc.Height				= m_WndHeight/BLUR_TEXTURE_DISCOUNT_SIZE;	// 高さ.
	texDesc.MipLevels			= 1;								// ミップマップレベル:1.
	texDesc.ArraySize			= 1;								// 配列数:1.
	texDesc.Format				= DXGI_FORMAT_R11G11B10_FLOAT;		// 32ビットフォーマット.
	texDesc.SampleDesc.Count	= 1;								// マルチサンプルの数.
	texDesc.SampleDesc.Quality	= 0;								// マルチサンプルのクオリティ.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// 使用方法:デフォルト.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// レンダーターゲット、シェーダーリソース.
	texDesc.CPUAccessFlags		= 0;								// CPUからはアクセスしない.
	texDesc.MiscFlags			= 0;								// その他の設定なし.

	for( int i = 0; i < EBlurSmpleDir_Max; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pBlurBufferRTV[i],
			&m_pBlurBufferSRV[i],
			&m_pBlurBufferTex[i] ))) return E_FAIL;
	}

	return S_OK;
}

// シェーダ作成.
HRESULT CBlur::CreateShader()
{
	ID3DBlob* pCompileVS = nullptr;
	ID3DBlob* pCompilePS = nullptr;
	ID3DBlob* pErrorMsg = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG
	//----------------------------.
	// 頂点シェーダー.
	//----------------------------.
	if( FAILED(
		D3DX10CompileFromFile( 
			SHADER_NAME, 
			nullptr, 
			nullptr, 
			"VS_Main",
			"vs_5_0", 
			uCompileFlag,
			0,
			nullptr,
			&pCompileVS, 
			&pErrorMsg,
			nullptr ))){
		ERROR_MESSAGE( "vs hlsl Loading Failure." );
		return E_FAIL;
	}
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompileVS->GetBufferPointer(),
			pCompileVS->GetBufferSize(),
			nullptr,
			&m_pVertexShader ))){
		ERROR_MESSAGE( "vs hlsl Creating Failure." );
		return E_FAIL;
	}
	//----------------------------.
	// ピクセルシェーダー.
	//----------------------------.
	for( int i = 0; i < EBlurSmpleDir_Max; i++ ){
		if( FAILED(
			D3DX10CompileFromFile( 
				SHADER_NAME, 
				nullptr, 
				nullptr, 
				SHADER_ENTRY_NAME_LIST[i], 
				"ps_5_0", 
				uCompileFlag, 
				0,
				nullptr,
				&pCompilePS,
				&pErrorMsg,
				nullptr ))){
			ERROR_MESSAGE( (char*)pErrorMsg->GetBufferPointer()  );
			return E_FAIL;
		}
		if( FAILED(
			m_pDevice11->CreatePixelShader(
				pCompilePS->GetBufferPointer(),
				pCompilePS->GetBufferSize(),
				nullptr,
				&m_pPixelShaderList[i] ))){
			ERROR_MESSAGE( "ps hlsl Creating Failure." );
			return E_FAIL;
		}
	}
	//----------------------------.
	//	頂点インプット.
	//----------------------------.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int numElements = sizeof(layout)/sizeof(layout[0]);	// 要素数算出.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompileVS->GetBufferPointer(),
			pCompileVS->GetBufferSize(),
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "vs layout Creating Failure." );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompileVS );
	SAFE_RELEASE( pCompilePS );

	return S_OK;
}

// サンプラの作成.
HRESULT CBlur::InitSample()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter				= D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;		// UV値が[0,1]を超えたら,[0,1]に設定する.
	samDesc.AddressV			= D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.ComparisonFunc		= D3D11_COMPARISON_NEVER;
	samDesc.MipLODBias			= 0;
	samDesc.MaxAnisotropy		= 1;
	samDesc.ComparisonFunc		= D3D11_COMPARISON_NEVER;
	samDesc.BorderColor[ 0 ]	= 0.0f;
	samDesc.BorderColor[ 1 ]	= 0.0f;
	samDesc.BorderColor[ 2 ]	= 0.0f;
	samDesc.BorderColor[ 3 ]	= 0.0f;
	samDesc.MinLOD				= -D3D11_FLOAT32_MAX;
	samDesc.MaxLOD				= D3D11_FLOAT32_MAX;
	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) ){
		ERROR_MESSAGE( "SamplerState creation failed" );
		return E_FAIL;
	}
	return S_OK;
}

// コンスタントバッファの作成.
HRESULT CBlur::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth			= sizeof(C_BUFFER);
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;

	UINT width = m_WndWidth/BLUR_TEXTURE_DISCOUNT_SIZE;
	UINT height = m_WndHeight/BLUR_TEXTURE_DISCOUNT_SIZE;
	if( FAILED(  m_pDevice11->CreateBuffer( &cbDesc, nullptr, &m_pConstantBuffer ))){
		ERROR_MESSAGE( "Buffer creation failed" );
		return E_FAIL;
	}

	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;

	// バッファ内のデータの書き換え開始時にMap.
	if( FAILED( m_pContext11->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
		ERROR_MESSAGE( "ConstantBuffer Map failed" );
		return E_FAIL;
	}

	C_BUFFER cb;	// コンスタントバッファ.
	D3DXMatrixIdentity( &cb.mW );
	D3DXMatrixTranspose( &cb.mW, &cb.mW );
	// ビューポートの幅,高さを渡す.
	cb.vViewPort.x = static_cast<float>(width);
	cb.vViewPort.y = static_cast<float>(height);

	cb.vPixelSize.x = 1.0f / static_cast<float>(width);
	cb.vPixelSize.y = 1.0f / static_cast<float>(height);

	cb.vRatio.x = static_cast<float>(m_WndWidth/width);
	cb.vRatio.y = static_cast<float>(m_WndHeight/height);

	// メモリ領域をコピー.
	memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
	m_pContext11->Unmap( m_pConstantBuffer, 0 );

	return S_OK;
}

// モデル作成.
HRESULT CBlur::CreateModel()
{
	float width		= static_cast<float>(m_WndWidth/BLUR_TEXTURE_DISCOUNT_SIZE);
	float height	= static_cast<float>(m_WndHeight/BLUR_TEXTURE_DISCOUNT_SIZE);

	// 板ポリ(四角形)の頂点を作成.
	VERTEX vertices[] =
	{
		// 頂点座標(x,y,z)				 
		D3DXVECTOR3( 0.0f,	height,	0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3( 0.0f,	0.0f,	0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( width, height,	0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( width, 0.0f,	0.0f ),	D3DXVECTOR2( 1.0f, 0.0f ),
	};
	// 最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(VERTEX) * uVerMax;	// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;	// CPUからはアクセスしない.
	bd.MiscFlags			= 0;	// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;	// 構造体のサイズ(未使用).

	// サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	// 板ポリの頂点をセット.

	// 頂点バッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, 
		&InitData, 
		&m_pVertexBuffer ))){
		ERROR_MESSAGE( "頂点ﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}

// バッファの作成.
HRESULT CBlur::CreateBufferTex(
	const D3D11_TEXTURE2D_DESC	texDesc,
	ID3D11RenderTargetView**	ppRTV,
	ID3D11ShaderResourceView**	ppSRV,
	ID3D11Texture2D**			ppTex )
{
	// そのテクスチャに対してデプスステンシル(DSTex)を作成.
	if( FAILED( m_pDevice11->CreateTexture2D( &texDesc, nullptr, ppTex )) ){
		_ASSERT_EXPR( false, L"テクスチャデスク作成失敗" );
		return E_FAIL;
	}
	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = texDesc.Format;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	// RenderTargetView作成.
	if( FAILED( m_pDevice11->CreateRenderTargetView( *ppTex, &rtvDesc, ppRTV ) )){
		_ASSERT_EXPR( false, L"RenderTargetView作成失敗" );
		return E_FAIL;
	}

	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset( &srvDesc, 0, sizeof( srvDesc ) );
	srvDesc.Format              = rtvDesc.Format;
	srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	// テクスチャ作成時と同じフォーマット
	if( FAILED( m_pDevice11->CreateShaderResourceView( *ppTex, &srvDesc, ppSRV ) )){
		_ASSERT_EXPR( false, L"デプスステンシル作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}