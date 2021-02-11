#include "InvisibleWallShader.h"
#include "..\D3DX\D3DX11.h"
#include "..\..\Camera\CameraManager\CameraManager.h"

namespace
{
	const char* SHADER_NAME		= "Data\\Shader\\InvisibleWall.hlsl";	// シェーダー名.
	const char* TEXTURE_NAME	= "Data\\Mesh\\shield.png";				// 壁のテクスチャ.
};

CInvisibleWallShader::CInvisibleWallShader()
	: m_pVertexShader	( nullptr )
	, m_pPixelShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pTexture		( nullptr )
	, m_pMaskTexture	( nullptr )
	, m_pSampleLinear	( nullptr )
	, m_pConstantBuffer	( nullptr )
	, m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
{
}

CInvisibleWallShader::~CInvisibleWallShader()
{
	Release();
}

// 初期化.
HRESULT CInvisibleWallShader::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const D3DXVECTOR3& vMaxPoint,
	const D3DXVECTOR3& vMinPoint )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( FAILED( CreateShader() ))		return E_FAIL;
	if( FAILED( InitSample() ))			return E_FAIL;
	if( FAILED( CreateModel( vMaxPoint, vMinPoint) ))	return E_FAIL;
	if( FAILED( InitTexture( TEXTURE_NAME ) )) return E_FAIL;

	D3D11_BUFFER_DESC cb;

	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= sizeof(C_BUFFER);
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;

	return m_pDevice11->CreateBuffer( &cb, nullptr, &m_pConstantBuffer );

	return S_OK;
}

// 解放.
void CInvisibleWallShader::Release()
{
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pMaskTexture );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}

// 描画.
void CInvisibleWallShader::Render()
{
	// ワールド行列.
	D3DXMATRIX mWorld, mWVP, mVPT;
	D3DXMATRIX mRot, mTran, mScale;

	// 回転.
	D3DXMATRIX mYaw, mPitch, mRoll;
	D3DXMatrixRotationX( &mPitch, m_vRot.x );
	D3DXMatrixRotationY( &mYaw, m_vRot.y );
	D3DXMatrixRotationZ( &mRoll, m_vRot.z );
	mRot = mYaw * mPitch * mRoll;
	// 平行移動.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );
	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );
	// ワールド行列
	mWorld = mScale * mRot * mTran;
	mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();


	D3DXMATRIX mT = 
	{
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f
	};
	mVPT = CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix() * mT;

	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map( 
		m_pConstantBuffer, 
		0, 
		D3D11_MAP_WRITE_DISCARD, 
		0, 
		&pData ))){

		C_BUFFER cb;	// コンスタントバッファ.
		cb.mWVP = mWVP;
		cb.mW = mWorld;
		cb.mVPT = mVPT;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP ); // 行列を転置する.
		D3DXMatrixTranspose( &cb.mW, &cb.mW ); // 行列を転置する.
		D3DXMatrixTranspose( &cb.mVPT, &cb.mVPT ); // 行列を転置する.

		cb.Color = {};
		cb.PlayerPos = { m_PlayerPosition, 0.0f };

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		// バッファ内のデータの書き換え終了時にUnmap.
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );// ピクセルシェーダ.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.

	m_pContext11->PSSetShaderResources( 0, 1, &m_pTexture );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pMaskTexture );
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// サンプラのセット.

	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	SetBlend( true );
	m_pContext11->DrawIndexed( 36, 0, 0 );
	SetBlend( false );
}

// シェーダ作成.
HRESULT CInvisibleWallShader::CreateShader()
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
	if( FAILED(
		D3DX10CompileFromFile( 
			SHADER_NAME, 
			nullptr, 
			nullptr, 
			"PS_Main", 
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
			&m_pPixelShader ))){
		ERROR_MESSAGE( "ps hlsl Creating Failure." );
		return E_FAIL;
	}
	//----------------------------.
	//	頂点インプット.
	//----------------------------.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
HRESULT CInvisibleWallShader::InitSample()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter				= D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.AddressU			= D3D11_TEXTURE_ADDRESS_MIRROR;		// UV値が[0,1]を超えたら,[0,1]に設定する.
	samDesc.AddressV			= D3D11_TEXTURE_ADDRESS_MIRROR;
	samDesc.AddressW			= D3D11_TEXTURE_ADDRESS_MIRROR;
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

// モデル作成.
HRESULT CInvisibleWallShader::CreateModel( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint )
{
	//板ﾎﾟﾘ(四角形)の頂点を作成.
	VERTEX vertices[] =
	{
		//頂点座標(x,y,z).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点１(左上奥).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点２(右上奥).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点３(左下奥).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点４(右下奥).

		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点１(左上手前).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点２(左下手前).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点３(右上手前).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点４(右下手前).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点１(左上手前).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点２(左上奥).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点３(左下手前).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点４(左下奥).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点１(右上手前).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点２(右下手前).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点３(右上奥).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点４(右下奥).

		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点１(左下).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点２(左上).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //頂点３(右下).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //頂点４(右上).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点１(左下).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //頂点２(左上).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //頂点３(右下).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z )  //頂点４(右上).
	};
	WORD indexList[]{
		0,  1,  2,     1,  0,  3,
		4,  5,  6,     5,  4,  7,
		8,  9,  10,    9,  8,  11,
		12, 13, 14,    13, 12, 15,
		16, 17, 18,    17, 16, 19,
		20, 21, 22,    21, 20, 23,
	};

	// 最大要素数を算出する.
	UINT uVerMax = sizeof(vertices)/sizeof(vertices[0]);
	UINT uIndexMax = sizeof(indexList)/sizeof(indexList[0]);

	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// 使用方法(デフォルト).
	bd.ByteWidth			= sizeof(VERTEX)*uVerMax;	// 頂点のサイズ.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして扱う.
	bd.CPUAccessFlags		= 0;						// CPUからはアクセスしない.
	bd.MiscFlags			= 0;						// その他のフラグ(未使用).
	bd.StructureByteStride	= 0;						// 構造体のサイズ(未使用).

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

	D3D11_BUFFER_DESC id;
	id.Usage				= D3D11_USAGE_DEFAULT;	   // 使用方法(デフォルト).
	id.ByteWidth			= sizeof(WORD)*uIndexMax;  // 頂点のサイズ.
	id.BindFlags			= D3D11_BIND_INDEX_BUFFER; // 頂点バッファとして扱う.
	id.CPUAccessFlags		= 0;					   // CPUからはアクセスしない.
	id.MiscFlags			= 0;					   // その他のフラグ(未使用).
	id.StructureByteStride	= 0;					   // 構造体のサイズ(未使用).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem			= indexList;
	IndexData.SysMemPitch		= 0;
	IndexData.SysMemSlicePitch	= 0;

	if( FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &m_pIndexBuffer ))){
		_ASSERT_EXPR( false, L"インデックスﾊﾞｯﾌｧ作成失敗" );
		return E_FAIL;
	}
	return S_OK;
}

// テクスチャの作成.
HRESULT CInvisibleWallShader::InitTexture( const char* filename )
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
	// マスク用のテクスチャ作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,	// リソースを使用するデバイスのポインタ.
			"Data\\Mesh\\shieldMask.png",		// ファイル名.
			nullptr,
			nullptr,
			&m_pMaskTexture,	// (out)テクスチャ.
			nullptr ))) {
		std::string err = filename;
		err += " : テクスチャ読み込み失敗";
		ERROR_MESSAGE(err);
		return E_FAIL;
	}
	return S_OK;
}
