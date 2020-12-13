#ifndef BLOOM_H
#define BLOOM_H

#include "..\Common.h"

class CBloom : public CCommon
{
	const int SAMPLE_BLUR_MAX = 6;
	const char* SHADER_NAME = "Data\\Shader/Bloom.hlsl";
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
public:
	CBloom();
	virtual ~CBloom();

	// 初期化.
	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// 解放.
	void Release();

	// 明度をサンプリングする.
	void Sampling( ID3D11ShaderResourceView* tex );

	// ブラーテクスチャの取得.
	std::vector<ID3D11ShaderResourceView*> GetBlurTex(){ return m_pBlurBufferSRV; }

private:
	// 輝度用の作成.
	HRESULT InitDownLuminanceTex();
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
	ID3D11RenderTargetView*		m_pDownLuminanceRTV;	// 輝度を落としたテクスチャのレンダーターゲットビュー.
	ID3D11ShaderResourceView*	m_pDownLuminanceSRV;	// 輝度を落としたテクスチャのシェーダーリソースビュー.
	ID3D11Texture2D*			m_pDownLuminanceTex;	// 輝度を落としたテクスチャのテクスチャ2D.

	std::vector<ID3D11RenderTargetView*>	m_pBlurBufferRTV;	// ブラー用バッファのレンダーターゲットビュー.
	std::vector<ID3D11ShaderResourceView*>	m_pBlurBufferSRV;	// ブラー用バッファのステンシルビュー.
	std::vector<ID3D11Texture2D*>			m_pBlurBufferTex;	// ブラー用バッファのテクスチャー2D.

	ID3D11VertexShader*		m_pVertexShader;		// 頂点シェーダー.
	ID3D11PixelShader*		m_pPixelShader;			// ピクセルシェーダー.
	ID3D11InputLayout*		m_pVertexLayout;		// 頂点レイアウト.
	ID3D11SamplerState*		m_pSampleLinear;		// サンプラ:テクスチャに各種フィルタをかける.
	std::vector<ID3D11Buffer*> m_pConstantBuffer;	// コンスタントバッファ.
	std::vector<ID3D11Buffer*> m_pVertexBuffer;		// 頂点バッファ.

	UINT					m_WndWidth;			// ウィンドウ幅.
	UINT					m_WndHeight;		// ウィンドウ高さ.
};

#endif	// #ifndef BLOOM_H.