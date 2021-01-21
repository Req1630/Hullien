#ifndef TRANSITION_H
#define TRANSITION_H

#include "..\Common.h"

enum enTRANSITION_OUT_MODE
{
	ETRANSITION_OUT_MODE_None = -1,

	ETRANSITION_OUT_MODE_Alpha,
	ETRANSITION_OUT_MODE_Cut,
	ETRANSITION_OUT_MODE_HardLight,

	ETRANSITION_OUT_MODE_Max,
} typedef ETRANSITION_OUT_MODE;

class CTransition : public CCommon
{
	const char* SHADER_NAME		= "Data\\Shader\\Transition.hlsl";
	struct CBUFFER_PER_INIT
	{
		ALIGN16 D3DXMATRIX	mW;		// ワールド行列.
		ALIGN16 D3DXVECTOR2 vViewPort;
	};
	struct CBUFFER_PER_FRAME
	{
		ALIGN16 D3DXMATRIX	mW;		// ワールド行列.
		ALIGN16 float Value;
		ALIGN16 float IsScreenSize;	// screenサイズに合わせるかどうか.
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2	Tex;	// テクスチャ座標.
	};
public:
	struct stPSShaderName
	{
		ETRANSITION_OUT_MODE	enBlendMode;
		const char*				entryName;

	} typedef SPSShaderName;
public:
	CTransition();
	virtual ~CTransition();

	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* ruleSpritePath,
		const float& width,
		const float& height );

	// 解放.
	void Release();

	// レンダリング.
	void Render();

	// テクスチャの設定.
	void SetTexture( ID3D11ShaderResourceView* pTexture );
	void SetDestTexture( ID3D11ShaderResourceView* pTexture );
	// 値の設定.
	inline void SetValue( const float& value ){ m_Value = value; }
	// screenサイズに合わせるかどうか.
	inline void IsScreenSize( const bool& isScreenSize ){ m_IsScreenSize = isScreenSize; };

	// トランジションの切り抜き方法の設定.
	void SetTransitionCutMode( const ETRANSITION_OUT_MODE& mode ){ m_TransitionOutMode = mode; }

private:
	// モデル(ポリゴン)の作成.
	HRESULT InitModel( const float& width, const float& height );
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
	ID3D11VertexShader*				m_pVertexShader;		// 頂点シェーダー.
	std::vector<ID3D11PixelShader*>	m_pPixelShaderList;		// ピクセルシェーダー.
	ID3D11InputLayout*				m_pVertexLayout;		// 頂点レイアウト.
	ID3D11Buffer*					m_pConstantBufferInit;	// コンスタントバッファ.
	ID3D11Buffer*					m_pConstantBufferFrame;	// コンスタントバッファ.
	ID3D11Buffer*					m_pVertexBuffer;		// 頂点バッファ.

	ID3D11SamplerState*			m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11ShaderResourceView*	m_pMaskTexture;		// マスクテクスチャ.
	ID3D11ShaderResourceView*	m_pTexture;			// テクスチャ.
	ID3D11ShaderResourceView*	m_pDestTexture;		// テクスチャ.

	ETRANSITION_OUT_MODE		m_TransitionOutMode;// トランジションの切り抜き方法.

	float m_Value;
	bool m_IsScreenSize;
};

#endif	// #ifndef TRANSITION_H.