#ifndef CIRCLE_GAGE_H
#define CIRCLE_GAGE_H

#include "..\Common.h"

class CCircleGage : public CCommon
{
	const char* SHADER_NAME			= "Data\\Shader\\CircleGage.hlsl";
	const char* MASK_TEXTURE_NAME	= "Data\\Mesh\\gage.png";
	const char* TEXTURE_NAME		= "Data\\Mesh\\Fog.png";

	struct CBUFFER_PER_FRAME
	{
		ALIGN16 D3DXMATRIX	mW;				// ワールド行列.
		ALIGN16 D3DXVECTOR2	vViewPort;		// ビューポート.
		ALIGN16 D3DXVECTOR2	vCenterPos;		// 円の中心座標 : 0~1.
		ALIGN16 D3DXVECTOR2	vStartVecotr;	// 円の開始ベクトル.
		ALIGN16 float		Value;			// 値.
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
public:
	CCircleGage();
	virtual ~CCircleGage();

	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// 解放.
	void Release();

	// レンダリング.
	void Render();
	// 値の設定.
	inline void SetValue( const float& value ){ m_Value = value; }

private:
	// モデル(ポリゴン)の作成.
	HRESULT InitModel();
	// テクスチャの作成.
	HRESULT InitTexture();
	// シェーダーの作成.
	HRESULT InitShader();
	// サンプラの作成.
	HRESULT InitSampler();
	// コンスタントバッファ作成関数.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );

private:
	ID3D11VertexShader*			m_pVertexShader;		// 頂点シェーダー.
	ID3D11PixelShader*			m_pPixelShader;			// ピクセルシェーダー.
	ID3D11InputLayout*			m_pVertexLayout;		// 頂点レイアウト.
	ID3D11Buffer*				m_pConstantBufferFrame;	// コンスタントバッファ.
	ID3D11Buffer*				m_pVertexBuffer;		// 頂点バッファ.

	ID3D11SamplerState*			m_pSampleLinear;		// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11ShaderResourceView*	m_pMaskTexture;			// マスクテクスチャ.
	ID3D11ShaderResourceView*	m_pTexture;				// テクスチャ.

	float						m_Value;
};

#endif	// #ifndef CIRCLE_GAGE_H.