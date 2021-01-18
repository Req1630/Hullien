#include "CSprite.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include <crtdbg.h>
#include <algorithm>

// コンストラクタ.
CSprite::CSprite()
	: m_pSpriteShader	( nullptr )
	, m_pSpriteData		( nullptr )
	, m_pSampleLinear	( nullptr )
	, m_VerticesUI		()
{
	m_pSpriteShader = std::make_unique<CSpriteShader>();
	m_pSpriteData = std::make_shared<SSpriteData>();
}

CSprite::CSprite(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SSpriteState& pSs)
	: CSprite()
{
	Init(pDevice11, pContext11, fileName, pSs);
}

// デストラクタ.
CSprite::~CSprite()
{
	Release();
}

// 初期化.
HRESULT CSprite::Init(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SSpriteState& pSs)
{
	m_pSpriteData->SState = pSs;
	m_vPos = m_pSpriteData->SState.vPos;
	m_pSpriteData->enLocalPosition = static_cast<enLocalPosition>(m_pSpriteData->SState.LocalPosNum);
	if( FAILED( InitPram( pDevice11, pContext11 ) )) return E_FAIL;
	if( FAILED( m_pSpriteShader->Init( m_pDevice11, m_pContext11 ) )) return E_FAIL;
	if( FAILED( InitModel() )) return E_FAIL;
	if( FAILED( CreateTexture( fileName, &m_pSpriteData->pTexture ) )) return E_FAIL;

	return S_OK;
}

// 解放.
void CSprite::Release()
{
	SAFE_RELEASE( m_pSampleLinear );
	m_pSpriteData->Release();
}

// モデル作成.
HRESULT CSprite::InitModel()
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

	InitSample();

	return S_OK;
}

HRESULT CSprite::InitSample()
{
	// テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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
HRESULT CSprite::CreateTexture( const char* fileName,
	ID3D11ShaderResourceView** pTexture )
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
void CSprite::Render( const bool& isBillboard )
{
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
	UVScrollUpdate();
	// コンスタントバッファの設定.
	m_pSpriteShader->SetConstantBufferData( mWVP, m_vColor, { m_pSpriteData->UV.x, m_pSpriteData->UV.y } );

	// 3D用のシェーダーを設定.
	m_pSpriteShader->ShaderSet(m_pSpriteData->pVertexBuffer);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	//	m_pContext11->IASetInputLayout( m_pVertexLayout );

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(0, 1, &m_pSpriteData->pTexture);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(4, 0);//板ﾎﾟﾘ(頂点4つ分).
}

void CSprite::RenderUI()
{
	// 座標のz値を初期化.
	m_vPos.z = 0.0f;
	AnimUpdate();
	UVScrollUpdate();
	// コンスタントバッファの設定.

	m_pSpriteShader->SetConstantBufferData( 
		CreateWorldMatrix(), m_vColor, { m_pSpriteData->UV.x, m_pSpriteData->UV.y } );


	// UI用のシェーダーを設定.
	m_pSpriteShader->ShaderUISet(m_pSpriteData->pVertexBufferUI);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	//	m_pContext11->IASetInputLayout( m_pVertexLayout );

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(0, 1, &m_pSpriteData->pTexture);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(4, 0);//板ﾎﾟﾘ(頂点4つ分).
}

D3DXMATRIX CSprite::CreateWorldMatrix()
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
void CSprite::SetAnimNumber( const int& animNumber )
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
D3DXVECTOR2 CSprite::ConvertIntoUV()
{
	return {
		// x座標.
		static_cast<float>(m_pSpriteData->PatternNo.x) / static_cast<float>(m_pSpriteData->PatternMax.x),
		// y座標.
		static_cast<float>(m_pSpriteData->PatternNo.y) / static_cast<float>(m_pSpriteData->PatternMax.y)
	};
}

// アニメーションの更新.
void CSprite::AnimUpdate()
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

// UVスクロールの更新.
void CSprite::UVScrollUpdate()
{
	if (m_pSpriteData->IsUVScroll == false) return;
	m_pSpriteData->UV.x -= 0.001f;
	if( m_pSpriteData->UV.x >= 1.0f ) m_pSpriteData->UV.x = 0.0f;
	if( m_pSpriteData->UV.x <= 0.0f ) m_pSpriteData->UV.x = 1.0f;
}

void CSprite::CreateVERTEX( const float& w, const float& h, const float& u, const float& v )
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