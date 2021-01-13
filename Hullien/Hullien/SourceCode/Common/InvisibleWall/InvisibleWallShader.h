#ifndef INVISIBLE_WALL_SHADER_H
#define INVISIBLE_WALL_SHADER_H

#include "..\Common.h"

class CInvisibleWallShader : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader\\InvisibleWall.hlsl";
	const char* TEXTURE_NAME = "Data\\Mesh\\shield.png";
	const D3DXVECTOR4 CLEAR_BACK_COLOR = { 0.0f, 0.0f, 0.0f, 0.0f };
	// コンスタントバッファ.
	struct C_BUFFER
	{
		D3DXMATRIX	mW;			// ワールド行列.
		D3DXMATRIX	mWVP;		// ワールド,ビュー,プロジェクションの合成行列.
		D3DXMATRIX	mVPT;		// ワールド,ビュー,プロジェクションの合成行列.
		D3DXVECTOR4	PlayerPos;	// プレイヤー位置.
		D3DXVECTOR4	Color;	// 色.
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
	};

	enum enBlurSmpleDir
	{
		EBlurSmpleDir_Width,	// 横方向.
		EBlurSmpleDir_Height,	// 高さ方向.

		EBlurSmpleDir_Final = EBlurSmpleDir_Height,

		EBlurSmpleDir_Max,
	} typedef EBlurSmpleDir;

public:
	CInvisibleWallShader();
	virtual ~CInvisibleWallShader();

	// 初期化.
	HRESULT Init( 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11,
		const D3DXVECTOR3& vMaxPoint, 
		const D3DXVECTOR3& vMinPoint );
	// 解放.
	void Release();

	// 描画.
	void Render();

	// プレイヤー座標の設定.
	inline void SetPlayerPos( const D3DXVECTOR3& pos ){ m_PlayerPosition = pos; }

private:
	// シェーダ作成.
	HRESULT CreateShader();
	// サンプラの作成.
	HRESULT InitSample();
	// モデル作成.
	HRESULT CreateModel( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint );
	// テクスチャの作成.
	HRESULT InitTexture( const char* filename );

private:
	ID3D11VertexShader*	m_pVertexShader;	// 頂点シェーダー.
	ID3D11PixelShader*	m_pPixelShader;		// ピクセルシェーダー.
	ID3D11InputLayout*	m_pVertexLayout;	// 頂点レイアウト.
	ID3D11ShaderResourceView*	m_pTexture;	// テクスチャ.
	ID3D11SamplerState*	m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11Buffer*		m_pConstantBuffer;	// コンスタントバッファ.
	ID3D11Buffer*		m_pVertexBuffer;	// 頂点バッファ.
	ID3D11Buffer*		m_pIndexBuffer;		// インデックスバッファ.
	D3DXVECTOR3			m_PlayerPosition;	// プレイヤー座標.
};

#endif	// #ifndef INVISIBLE_WALL_SHADER_H.