#include "BlendSprite.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\SceneTexRenderer\SceneTexRenderer.h"
#include <crtdbg.h>
#include <algorithm>

static const CBlendSprite::SPSShaderName PS_SHADER_LIST[] = 
{
	{ EBLEND_MODE_Normal,		"PS_Main" },
	{ EBLEND_MODE_HardLight,	"PS_HardLight" },
};

// コンストラクタ.
CBlendSprite::CBlendSprite()
	: m_pSpriteData		( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pVertexShader	( nullptr )
	, m_pVertexShaderUI	( nullptr )
	, m_pPixelShaderList()
	, m_pSampleLinear	( nullptr )
	, m_pDestTexture	( nullptr )
	, m_VerticesUI		()
{
	// PSシェーダーのサイズを確保.
	const int psShaderListSize = sizeof(PS_SHADER_LIST)/sizeof(PS_SHADER_LIST[0]);
	m_pPixelShaderList.resize( psShaderListSize );
}

CBlendSprite::CBlendSprite(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SSpriteState& pSs)
	: CBlendSprite()
{
	Init(pDevice11, pContext11, fileName, pSs);
}

// デストラクタ.
CBlendSprite::~CBlendSprite()
{
	Release();
}

// 初期化.
HRESULT CBlendSprite::Init(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SSpriteState& pSs)
{
	m_pSpriteData->SState = pSs;
	m_vPos = m_pSpriteData->SState.vPos;
	m_pSpriteData->enLocalPosition = static_cast<enLocalPosition>(m_pSpriteData->SState.LocalPosNum);
	m_pSpriteData = std::make_shared<SSpriteData>();
	if( FAILED( InitPram( pDevice11, pContext11 ) )) return E_FAIL;
	if( FAILED( InitShader() )) return E_FAIL;
	if( FAILED( InitModel() )) return E_FAIL;
	if( FAILED( CreateTexture( fileName, &m_pSpriteData->pTexture ) )) return E_FAIL;
	if( FAILED( InitSample() )) return E_FAIL;

	return S_OK;
}

// 初期化.
HRESULT CBlendSprite::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitPram( pDevice11, pContext11 ) )) return E_FAIL;
	if( FAILED( InitShader() )) return E_FAIL;
	if( FAILED( InitSample() )) return E_FAIL;

	return S_OK;
}

// 解放.
void CBlendSprite::Release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pVertexShaderUI );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pVertexLayout );
	m_pDestTexture = nullptr;
}

// シェーダーの作成.
HRESULT CBlendSprite::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG.


	// 頂点シェーダーの作成.

	// 3D描画用.
	if( FAILED( D3DX10CompileFromFile(
		SHADER_NAME,		// シェーダーファイルパス.
		nullptr,			// マクロ定義のポインター : オプション.
		nullptr,			// インクルードファイルを処理するためのポインタ : オプション.
		"VS_Main",			// シェーダーのエントリーポイント関数名.
		"vs_5_0",			// シェーダーモデルの名前.
		uCompileFlag,		// シェーダーコンパイルフラグ.
		0,					// エフェクトコンパイルフラグ (0推奨).
		nullptr,			// スレッドポンプインターフェイスへのポインター.
		&pCompiledShader,	// コンパイルされたシェーダーのデータ (out).
		&pErrors,			// コンパイル中のエラー出力 (out).
		nullptr ) )){		// 戻り値のポインタ.
		return E_FAIL;
	}
	if( FAILED( m_pDevice11->CreateVertexShader( 
		pCompiledShader->GetBufferPointer(), 
		pCompiledShader->GetBufferSize(), 
		nullptr, 
		&m_pVertexShader ) )){
		return E_FAIL;
	}
	// 2D描画用.
	if( FAILED( D3DX10CompileFromFile(
		SHADER_NAME,		// シェーダーファイルパス.
		nullptr,			// マクロ定義のポインター : オプション.
		nullptr,			// インクルードファイルを処理するためのポインタ : オプション.
		"VS_MainUI",		// シェーダーのエントリーポイント関数名.
		"vs_5_0",			// シェーダーモデルの名前.
		uCompileFlag,		// シェーダーコンパイルフラグ.
		0,					// エフェクトコンパイルフラグ (0推奨).
		nullptr,			// スレッドポンプインターフェイスへのポインター.
		&pCompiledShader,	// コンパイルされたシェーダーのデータ (out).
		&pErrors,			// コンパイル中のエラー出力 (out).
		nullptr ) )){		// 戻り値のポインタ.
		return E_FAIL;
	}
	if( FAILED( m_pDevice11->CreateVertexShader( 
		pCompiledShader->GetBufferPointer(), 
		pCompiledShader->GetBufferSize(), 
		nullptr, 
		&m_pVertexShaderUI ) )){
		return E_FAIL;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// 頂点インプットレイアウトを作成.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ))){
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	
	for( int i = 0; i < static_cast<int>(m_pPixelShaderList.size()); i++ ){
		// 3D描画用.
		if( FAILED( D3DX10CompileFromFile(
			SHADER_NAME,					// シェーダーファイルパス.
			nullptr,						// マクロ定義のポインター : オプション.
			nullptr,						// インクルードファイルを処理するためのポインタ : オプション.
			PS_SHADER_LIST[i].entryName,	// シェーダーのエントリーポイント関数名.
			"ps_5_0",						// シェーダーモデルの名前.
			uCompileFlag,					// シェーダーコンパイルフラグ.
			0,								// エフェクトコンパイルフラグ (0推奨).
			nullptr,						// スレッドポンプインターフェイスへのポインター.
			&pCompiledShader,				// コンパイルされたシェーダーのデータ (out).
			&pErrors,						// コンパイル中のエラー出力 (out).
			nullptr ) )){					// 戻り値のポインタ.
			ERROR_MESSAGE( (char*)pErrors->GetBufferPointer()  );
			return E_FAIL;
		}
		if( FAILED( m_pDevice11->CreatePixelShader( 
			pCompiledShader->GetBufferPointer(), 
			pCompiledShader->GetBufferSize(), 
			nullptr,
			&m_pPixelShaderList[PS_SHADER_LIST[i].enBlendMode] ) )){
			return E_FAIL;
		}
		SAFE_RELEASE(pCompiledShader);
	}
	SAFE_RELEASE(pCompiledShader);

	D3D11_BUFFER_DESC cb;

	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= sizeof(C_BUFFER);
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;
	// コンスタントバッファの作成.
	if( FAILED( m_pDevice11->CreateBuffer( &cb, nullptr, &m_pConstantBuffer ) )){
		ERROR_MESSAGE("Instant buffer creation failed");
		return E_FAIL;
	}

	return S_OK;
}

// モデル作成.
HRESULT CBlendSprite::InitModel()
{
	float w = m_pSpriteData->SState.Disp.w;	// 表示スプライト幅,
	float h = m_pSpriteData->SState.Disp.h;	// 表示スプライト高さ,
	float u = m_pSpriteData->SState.Stride.w / m_pSpriteData->SState.Base.w;	// 1コマ当たりの幅,
	float v = m_pSpriteData->SState.Stride.h / m_pSpriteData->SState.Base.h;	// 1コマ当たりの高さ.

	m_pSpriteData->PatternMax.x = static_cast<LONG>( m_pSpriteData->SState.Base.w / m_pSpriteData->SState.Stride.w );// xの最大マス数.
	m_pSpriteData->PatternMax.y = static_cast<LONG>( m_pSpriteData->SState.Base.h / m_pSpriteData->SState.Stride.h );// yの最大マス数.
	//-------------------------------.
	// UI用.
	//-------------------------------.
	// バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// 使用方法(デフォルト).
	bd.ByteWidth = sizeof(CSpriteShader::VERTEX) * 4;	// 頂点のサイズ.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 頂点バッファとして扱う.
	bd.CPUAccessFlags = 0;								// CPUからはアクセスしない.
	bd.MiscFlags = 0;									// その他のフラグ(未使用).
	bd.StructureByteStride = 0;							// 構造体のサイズ(未使用).

	// サブリソース構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	CreateVERTEX(w, h, u, v);
	InitData.pSysMem = m_VerticesUI;	// 板ポリの頂点をセット.

	// 頂点バッファの作成.
	if( FAILED( 
		m_pDevice11->CreateBuffer( 
			&bd, &InitData, &m_pSpriteData->pVertexBufferUI ) )) {
		_ASSERT_EXPR(false, L"頂点ﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}

	// 頂点バッファをセット.
	UINT stride = sizeof(CSpriteShader::VERTEX);	// データ間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pSpriteData->pVertexBufferUI, &stride, &offset );

	//-------------------------------.
	// 3D用.
	//-------------------------------.
	// 画像の比率を求める.
	int as = myGcd((int)m_pSpriteData->SState.Disp.w, (int)m_pSpriteData->SState.Disp.h);
	w = m_pSpriteData->SState.Disp.w / as;
	h = m_pSpriteData->SState.Disp.h / as;
	w *= 0.1f;
	h *= 0.1f;
	// 板ポリ(四角形)の頂点を作成.
	CSpriteShader::VERTEX vertices[] =
	{
		// ポリゴンの中心を頂点とする.
		// 頂点座標(x,y,z)					UV座標(u,v)
		D3DXVECTOR3(-w / 2, -h / 2, 0.0f), D3DXVECTOR2(0.0f,    v),	//頂点１(左下).
		D3DXVECTOR3(-w / 2,  h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f),	//頂点２(左上).
		D3DXVECTOR3( w / 2, -h / 2, 0.0f), D3DXVECTOR2(   u,    v),	//頂点３(右下).
		D3DXVECTOR3( w / 2,  h / 2, 0.0f), D3DXVECTOR2(   u, 0.0f)	//頂点４(右上).
	};

	// サブリソース構造体.
	InitData.pSysMem = vertices;	// 板ポリの頂点をセット.

	// 頂点バッファの作成.
	if (FAILED(m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pSpriteData->pVertexBuffer))) {
		_ASSERT_EXPR(false, L"頂点ﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}

	// 頂点バッファをセット.
	m_pContext11->IASetVertexBuffers(0, 1, &m_pSpriteData->pVertexBuffer, &stride, &offset);

	return S_OK;
}

HRESULT CBlendSprite::InitSample()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// サンプラ作成.
	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))
	{
		ERROR_MESSAGE("SamplerState creation failed");
		return E_FAIL;
	}

	return S_OK;
}

// テクスチャ作成.
HRESULT CBlendSprite::CreateTexture(const char* fileName,
	ID3D11ShaderResourceView** pTexture)
{
	//ﾃｸｽﾁｬ作成.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ﾘｿｰｽを使用するﾃﾞﾊﾞｲｽのﾎﾟｲﾝﾀ.
		fileName,		//ﾌｧｲﾙ名.
		nullptr,
		nullptr,
		pTexture,		//(out)ﾃｸｽﾁｬ.
		nullptr ))){
		std::string err = fileName;
		err += " : テクスチャ読み込み失敗";
		ERROR_MESSAGE( err );
		return E_FAIL;
	}

	return S_OK;
}

//ﾚﾝﾀﾞﾘﾝｸﾞ用.
void CBlendSprite::Render(const bool& isBillboard)
{
	if( m_pSpriteData == nullptr ) return;

	//　ワールド行列.
	D3DXMATRIX mWorld;

	// ワールド行列作成.
	mWorld = CreateWorldMatrix();
	if (isBillboard == true) {
		// ビルボード用.
		D3DXMATRIX CancelRotation = CCameraManager::GetViewMatrix();
		CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f; // xyzを0にする.
		// CancelRotationの逆行列を求める.
		D3DXMatrixInverse(&CancelRotation, nullptr, &CancelRotation);
		mWorld = CancelRotation * mWorld;
	}
	// WVPの作成.
	D3DXMATRIX mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	AnimUpdate();
	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.

	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData ))) {
		//ﾜｰﾙﾄﾞ行列を渡す.
		cb.mWVP = mWVP;
		cb.mW = mWVP;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );//行列を転置する.
		D3DXMatrixTranspose( &cb.mW, &cb.mW );//行列を転置する.
		// ビューポートの幅,高さを渡す.
		cb.vViewPort.x	= static_cast<float>(WND_W);
		cb.vViewPort.y	= static_cast<float>(WND_H);

		// アルファ値を渡す.
		cb.vColor = m_vColor;

		// テクスチャ座標.
		cb.vUV.x = m_pSpriteData->UV.x;
		cb.vUV.y = m_pSpriteData->UV.y;

		memcpy_s(
			pData.pData,
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}
	// 3D用のシェーダーを設定.
	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );		// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShaderList[0], nullptr, 0 );	// ピクセルシェーダ.

	// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.

	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pSpriteData->pVertexBuffer, &stride, &offset );

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pSpriteData->pTexture );

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw( 4, 0 );//板ﾎﾟﾘ(頂点4つ分).
}

void CBlendSprite::RenderUI()
{
	if( m_pSpriteData == nullptr ) return;
	// 座標のz値を初期化.
	m_vPos.z = 0.0f;
	AnimUpdate();
	// コンスタントバッファの設定.

	// シェーダーのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// コンスタントバッファ.
	// バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData ))) {
		//ﾜｰﾙﾄﾞ行列を渡す.
		cb.mW = cb.mWVP = CreateWorldMatrix();;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );//行列を転置する.
		D3DXMatrixTranspose( &cb.mW, &cb.mW );//行列を転置する.
		// ビューポートの幅,高さを渡す.
		cb.vViewPort.x	= static_cast<float>(WND_W);
		cb.vViewPort.y	= static_cast<float>(WND_H);

		// アルファ値を渡す.
		cb.vColor = m_vColor;

		// テクスチャ座標.
		cb.vUV.x = m_pSpriteData->UV.x;
		cb.vUV.y = m_pSpriteData->UV.y;

		memcpy_s(
			pData.pData,
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	// 使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShaderUI, nullptr, 0 );	// 頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShaderList[1], nullptr, 0 );	// ピクセルシェーダ.

	// このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// 頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ピクセルシェーダー.

	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX); // データの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pSpriteData->pVertexBufferUI, &stride, &offset );

	// 頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pSpriteData->pVertexLayout );

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pDestTexture );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pSpriteData->pTexture );

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(4, 0);//板ﾎﾟﾘ(頂点4つ分).
}

D3DXMATRIX CBlendSprite::CreateWorldMatrix()
{
	//　ワールド行列, スケール行列, 回転行列, 平行移動行列.
	D3DXMATRIX mScale, mRot, mTran;

	// 拡大縮小行列作成.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// 回転行列を作成.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.y, m_vRot.x, m_vRot.z );
	// 平行移動行列.
	D3DXMatrixTranslation(&mTran, m_vPos.x, m_vPos.y, m_vPos.z);

	// ワールド行列作成.
	return mScale * mRot * mTran;
}

// アニメーション番号の設定.
void CBlendSprite::SetAnimNumber(const int& animNumber)
{
	if (m_pSpriteData->AnimNumber >= m_pSpriteData->SState.AnimNum) return;
	m_pSpriteData->IsAnimation = false;	// アニメーションをしない.
	// アニメーション番号を算出.
	m_pSpriteData->PatternNo.x = animNumber % m_pSpriteData->PatternMax.x;
	m_pSpriteData->PatternNo.y = animNumber / m_pSpriteData->PatternMax.x;
	// UV座標に変換.
	m_pSpriteData->UV = ConvertIntoUV();
}

// UV座標に変換.
D3DXVECTOR2 CBlendSprite::ConvertIntoUV()
{
	return {
		// x座標.
		static_cast<float>(m_pSpriteData->PatternNo.x) / static_cast<float>(m_pSpriteData->PatternMax.x),
		// y座標.
		static_cast<float>(m_pSpriteData->PatternNo.y) / static_cast<float>(m_pSpriteData->PatternMax.y)
	};
}

// アニメーションの更新.
void CBlendSprite::AnimUpdate()
{
	if (m_pSpriteData->IsAnimation == false) return;

	if (m_pSpriteData->FrameCount % m_pSpriteData->FrameTime == 0) {
		// アニメーション番号を算出.
		m_pSpriteData->PatternNo.x = m_pSpriteData->AnimNumber % m_pSpriteData->PatternMax.x;
		m_pSpriteData->PatternNo.y = m_pSpriteData->AnimNumber / m_pSpriteData->PatternMax.x;
		m_pSpriteData->AnimNumber++;	// アニメーション番号を加算.
		if (m_pSpriteData->AnimNumber >= m_pSpriteData->SState.AnimNum) {
			// アニメーション番号が最大アニメーション数より多ければ.
			// 初期化する.
			m_pSpriteData->FrameCount = 0;
			m_pSpriteData->AnimNumber = 0;
		}
	}
	m_pSpriteData->FrameCount += m_pSpriteData->FrameCountSpeed;
	// UV座標に変換.
	m_pSpriteData->UV = ConvertIntoUV();
}

void CBlendSprite::CreateVERTEX(const float& w, const float& h, const float& u, const float& v)
{
	switch (m_pSpriteData->enLocalPosition)
	{
	case ELocalPosition::LeftUp:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(0.0f,    h, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w,    h, 0.0f), D3DXVECTOR2(u,    v) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w, 0.0f, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点４(右上).
		break;
	case ELocalPosition::Left:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(0.0f, -h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(0.0f,  h / 2, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w, -h / 2, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w,  h / 2, 0.0f), D3DXVECTOR2(u,    v) };	//頂点４(右上).
		break;
	case ELocalPosition::LeftDown:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(0.0f,    -h, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(0.0f,  0.0f, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w,    -h, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w,  0.0f, 0.0f), D3DXVECTOR2(u,    v) };	//頂点４(右上).
		break;
	case ELocalPosition::Down:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w / 2,    -h, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w / 2,  0.0f, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w / 2,    -h, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w / 2,  0.0f, 0.0f), D3DXVECTOR2(u,    v) };	//頂点４(右上).
		break;
	case ELocalPosition::RightDown:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w,   -h, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w, 0.0f, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(0.0f,   -h, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(u,    v) };	//頂点４(右上).
		break;
	case ELocalPosition::Right:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w, -h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w,  h / 2, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(0.0f, -h / 2, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(0.0f,  h / 2, 0.0f), D3DXVECTOR2(u,    v) };	//頂点４(右上).
		break;
	case ELocalPosition::RightUp:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w,    h, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(0.0f,    h, 0.0f), D3DXVECTOR2(u,    v) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点４(右上).
		break;
	case ELocalPosition::Up:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w / 2,    h, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w / 2, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w / 2,    h, 0.0f), D3DXVECTOR2(u,    v) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w / 2, 0.0f, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点４(右上).
		break;
	case ELocalPosition::Center:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w / 2, -h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//頂点１(左下).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w / 2,  h / 2, 0.0f), D3DXVECTOR2(0.0f,    v) };	//頂点２(左上).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w / 2, -h / 2, 0.0f), D3DXVECTOR2(u, 0.0f) };	//頂点３(右下).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w / 2,  h / 2, 0.0f), D3DXVECTOR2(u,    v) };	//頂点４(右上).
		break;
	default:
		break;
	}
}