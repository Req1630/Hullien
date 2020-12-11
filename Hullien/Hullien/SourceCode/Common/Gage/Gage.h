#ifndef GAGE_H
#define GAGE_H

#include "..\Common.h"

class CGage : public CCommon
{
	const char* SHADER_NAME		= "Data\\Shader\\Gage.hlsl";
	const char* TEXTURE_NAME	= "Data\\Mesh\\select1.png";

	struct CBUFFER_PER_FRAME
	{
		ALIGN16 D3DXMATRIX	mW;				// ワールド行列.
		ALIGN16 D3DXVECTOR4	vColor;
		ALIGN16 D3DXVECTOR2	vViewPort;
		ALIGN16 float		ImageWidth;
		ALIGN16 float		DispPos;
		ALIGN16 float		Value;
		ALIGN16 float		RangeValue;
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
public:
	CGage();
	virtual ~CGage();

	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// 解放.
	void Release();

	// レンダリング.
	void Render();

private:
	// モデル(ポリゴン)の作成.
	HRESULT InitModel();
	// テクスチャの作成.
	HRESULT InitTexture( const char* filename );
	// シェーダーの作成.
	HRESULT InitShader();
	// サンプラの作成.
	HRESULT InitSampler();
	// コンスタントバッファ作成関数.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );

private:
	ID3D11VertexShader*			m_pVertexShader;	// 頂点シェーダー.
	ID3D11PixelShader*			m_pPixelShader;		// ピクセルシェーダー.
	ID3D11InputLayout*			m_pVertexLayout;	// 頂点レイアウト.
	ID3D11Buffer*				m_pConstantBufferFrame;	// コンスタントバッファ.
	ID3D11Buffer*				m_pVertexBuffer;	// 頂点バッファ.

	ID3D11SamplerState*			m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11ShaderResourceView*	m_pTexture;			// テクスチャ.
};

#endif	// #ifndef TRANSITION_H.