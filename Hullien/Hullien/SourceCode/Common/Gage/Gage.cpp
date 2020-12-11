#include "Gage.h"

CGage::CGage()
	: m_pVertexShader			( nullptr )
	, m_pPixelShader			( nullptr )
	, m_pVertexLayout			( nullptr )
	, m_pConstantBufferFrame	( nullptr )
	, m_pVertexBuffer			( nullptr )
	, m_pSampleLinear			( nullptr )
	, m_pTexture				( nullptr )
{
}

CGage::~CGage()
{
}

// 初期化.
HRESULT CGage::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( m_pDevice11 == nullptr ) return E_FAIL;
	if( m_pContext11 == nullptr ) return E_FAIL;

	if( FAILED( InitModel() ))		return E_FAIL;
	if( FAILED( InitTexture(TEXTURE_NAME) ))	return E_FAIL;
	if( FAILED( InitShader() ))		return E_FAIL;
	if( FAILED( InitSampler() ))	return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pConstantBufferFrame, sizeof(CBUFFER_PER_FRAME) ) )) return E_FAIL;

	return S_OK;
}

// 解放.
void CGage::Release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBufferFrame );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;
}

// レンダリング.
void CGage::Render()
{
	static float value = 0.5f;
	if( GetAsyncKeyState('K') & 0x8000 ) value += 0.01f;
	if( GetAsyncKeyState('L') & 0x8000 ) value -= 0.01f;
	if( value > 0.4f ) value = 0.4f;
	if( value < 0.0f ) value = 0.0f;
	m_vPos.x = 200.0f;
	m_vPos.y = 200.0f;

	//　ワールド行列, スケール行列, 回転行列, 平行移動行列.
	D3DXMATRIX mWVP, mScale, mRot, mTran;

	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// 回転行列を作成.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.y, m_vRot.x, m_vRot.z );
	// 平行移動行列.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );

	// ワールド行列作成.
	mWVP = mScale * mRot * mTran;


	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	CBUFFER_PER_FRAME cb;	// コンスタントバッファ.

	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){

		// ワールド行列を渡す.
		cb.mW	= mWVP;
		D3DXMatrixTranspose( &cb.mW, &cb.mW ); // 行列を転置する.

		cb.vColor = m_vColor;

		// ビューポートの幅,高さを渡す.
		cb.vViewPort.x	= static_cast<float>(WND_W);
		cb.vViewPort.y	= static_cast<float>(WND_H);

		cb.ImageWidth	= 169.0f;
		cb.DispPos		= m_vPos.x;
		cb.Value		= 1.0f;
		cb.RangeValue	= value;


		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
	}

	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// ピクセルシェーダ.

	// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// ピクセルシェーダー.

	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	// プリミティブ・トポロジーをセット.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// テクスチャをシェーダーに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pTexture );

	// レンダリング.
	SetDeprh( false );
	SetBlend( true );
	m_pContext11->Draw( 4, 0 );
	SetBlend( false );
	SetDeprh( true );
}

// モデル(ポリゴン)の作成.
HRESULT CGage::InitModel()
{
	float w = 169.0f;
	float h = 42.0f;

	// 板ポリ(四角形)の頂点を作成.
	VERTEX vertices[] =
	{
		// ポリゴンの中心を頂点とする.
		// 頂点座標(x,y,z)					UV座標(u,v)
		D3DXVECTOR3(0.0f,    h, 0.0f), D3DXVECTOR2(0.0f, 1.0f),	//頂点１(左下).
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),	//頂点２(左上).
		D3DXVECTOR3(   w,    h, 0.0f), D3DXVECTOR2(1.0f, 1.0f),	//頂点３(右下).
		D3DXVECTOR3(   w, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)	//頂点４(右上).
	};

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(VERTEX) * 4;		// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;						// CPUからはアクセスしない.
	bd.MiscFlags			= 0;						// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;						// 構造体のサイズ(未使用).

														// サブリソース構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	// サブリソース構造体.
	InitData.pSysMem = vertices;	// 板ポリの頂点をセット.

									// 頂点バッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ))) {
		_ASSERT_EXPR(false, L"頂点ﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}

	return S_OK;
}

// テクスチャの作成.
HRESULT CGage::InitTexture( const char* filename )
{
	// マスク用のテクスチャ作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,	// リソースを使用するデバイスのポインタ.
			filename,		// ファイル名.
			nullptr,
			nullptr,
			&m_pTexture,	// (out)テクスチャ.
			nullptr ))) {
		std::string err = filename;
		err += " : テクスチャ読み込み失敗";
		ERROR_MESSAGE(err);
		return E_FAIL;
	}

	return S_OK;
}


// シェーダーの作成.
HRESULT CGage::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSLからバーテックスシェーダーのブロブﾞを作成.
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME, 
		nullptr,
		nullptr,
		"VS_Main",
		"vs_5_0",
		uCompileFlag, 
		0, 
		nullptr, 
		&pCompiledShader, 
		&pErrors, 
		nullptr ))){
		_ASSERT_EXPR( false, L"hlsl読み込み失敗" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// 上記で作成したブロブから「バーテックスシェーダー」を作成.
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader ))){	//(out)ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
		_ASSERT_EXPR(false, L"ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}

	// 頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	// 頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = 0;

	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	numElements = sizeof(tmp) / sizeof(tmp[0]);	//要素数算出.
	memcpy_s( layout, sizeof(layout),
		tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);


	// 頂点インプットレイアウトを作成.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ))){	//(out)頂点インプットレイアウト.
		_ASSERT_EXPR( false, L"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// HLSLからピクセルシェーダーのブロブを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr, 
			nullptr, 
			"PS_Main", 
			"ps_5_0",
			uCompileFlag, 
			0, 
			nullptr, 
			&pCompiledShader, 
			&pErrors, 
			nullptr ))){
		_ASSERT_EXPR(false, L"hlsl読み込み失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// 上記で作成したブロブから「ピクセルシェーダー」を作成.
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader ))){	// (out)ピクセルシェーダー.
		_ASSERT_EXPR( false, L"ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	return S_OK;
}

// サンプラの作成.
HRESULT CGage::InitSampler()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// リニアフィルタ(線形補間).
														// POINT:高速だが粗い.
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// ラッピングモード(WRAP:繰り返し).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// MIRROR: 反転繰り返し.
	// CLAMP : 端の模様を引き伸ばす.
	// BORDER: 別途境界色を決める.
	// サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, 
		&m_pSampleLinear ))) {
		_ASSERT_EXPR( false, L"ｻﾝﾌﾟﾗ作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}

// コンスタントバッファ作成関数.
HRESULT CGage::CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size )
{
	D3D11_BUFFER_DESC cb;

	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= size;
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;
	if( FAILED( m_pDevice11->CreateBuffer(
		&cb, 
		nullptr,
		pConstantBuffer ))){
		return E_FAIL;
	}
	return S_OK;
}