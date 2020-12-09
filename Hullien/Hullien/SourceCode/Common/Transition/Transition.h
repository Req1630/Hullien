#ifndef TRANSITION_H
#define TRANSITION_H

#include "..\Common.h"

class CTransition : public CCommon
{
	const char* SHADER_NAME		= "Data\\Shader\\Transition.hlsl";
	const char* TEXTURE_NAME	= "Data\\Mesh\\171Mask.png";
	const char* NAME			= "Data\\Mesh\\171.png";
	struct CBUFFER_PER_INIT
	{
		ALIGN16 D3DXMATRIX	mW;				// ワールド行列.
		ALIGN16 D3DXVECTOR2 vViewPort;
	};
	struct CBUFFER_PER_FRAME
	{
		ALIGN16 float Value;
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
public:
	CTransition();
	virtual ~CTransition();

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

	// コンスタントバッファの設定.
	void SetConstantBufferInit();

private:
	ID3D11VertexShader*			m_pVertexShader;	// 頂点シェーダー.
	ID3D11PixelShader*			m_pPixelShader;		// ピクセルシェーダー.
	ID3D11InputLayout*			m_pVertexLayout;	// 頂点レイアウト.
	ID3D11Buffer*				m_pConstantBufferInit;	// コンスタントバッファ.
	ID3D11Buffer*				m_pConstantBufferFrame;	// コンスタントバッファ.
	ID3D11Buffer*				m_pVertexBuffer;	// 頂点バッファ.

	ID3D11SamplerState*			m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11ShaderResourceView*	m_pMaskTexture;		// マスクテクスチャ.
	ID3D11ShaderResourceView*	m_pTexture;			// テクスチャ.
};

#endif	// #ifndef TRANSITION_H.