#ifndef BLUR_H
#define BLUR_H

#include "..\Common.h"

class CBlur : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader\\Blur.hlsl";
	const D3DXVECTOR4 CLEAR_BACK_COLOR = { 0.0f, 0.0f, 0.0f, 0.0f };
	// コンスタントバッファ.
	struct C_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				// ワールド行列.
		ALIGN16 D3DXVECTOR2	vViewPort;		// UV座標.
		ALIGN16 D3DXVECTOR2 vPixelSize;		// ピクセルのサイズ.
		ALIGN16 D3DXVECTOR2	vRatio;			// 画面の比率.
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2 Tex;	// テクスチャ座標.
	};

	enum enBlurSmpleDir
	{
		EBlurSmpleDir_Width,	// 横方向.
		EBlurSmpleDir_Height,	// 高さ方向.

		EBlurSmpleDir_Final = EBlurSmpleDir_Height,

		EBlurSmpleDir_Max,
	} typedef EBlurSmpleDir;

public:
	CBlur();
	virtual ~CBlur();

	// 初期化.
	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// 解放.
	void Release();

	// 明度をサンプリングする.
	void Sampling( ID3D11ShaderResourceView* tex );

	// バッファのクリア.
	void ClearBuffer();

	// ブラーテクスチャの取得.
	ID3D11ShaderResourceView* GetBlurTex(){ return m_pBlurBufferSRV[EBlurSmpleDir_Final]; }

private:
	// ブラー用の作成.
	HRESULT InitBlurTex();
	// シェーダ作成.
	HRESULT CreateShader();
	// サンプラの作成.
	HRESULT InitSample();
	// コンスタントバッファの作成.
	HRESULT CreateConstantBuffer();
	// モデル作成.
	HRESULT CreateModel();

	// バッファの作成.
	HRESULT CreateBufferTex(
		const D3D11_TEXTURE2D_DESC	texDesc,
		ID3D11RenderTargetView**	ppRTV,
		ID3D11ShaderResourceView**	ppSRV,
		ID3D11Texture2D**			ppTex );

private:
	std::vector<ID3D11RenderTargetView*>	m_pBlurBufferRTV;	// ブラー用バッファのレンダーターゲットビュー.
	std::vector<ID3D11ShaderResourceView*>	m_pBlurBufferSRV;	// ブラー用バッファのステンシルビュー.
	std::vector<ID3D11Texture2D*>			m_pBlurBufferTex;	// ブラー用バッファのテクスチャー2D.

	ID3D11VertexShader*				m_pVertexShader;	// 頂点シェーダー.
	std::vector<ID3D11PixelShader*>	m_pPixelShaderList;	// ピクセルシェーダー.
	ID3D11InputLayout*	m_pVertexLayout;	// 頂点レイアウト.
	ID3D11SamplerState*	m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11Buffer*		m_pConstantBuffer;	// コンスタントバッファ.
	ID3D11Buffer*		m_pVertexBuffer;	// 頂点バッファ.

	UINT				m_WndWidth;			// ウィンドウ幅.
	UINT				m_WndHeight;		// ウィンドウ高さ.
};

#endif	// #ifndef BLOOM_H.