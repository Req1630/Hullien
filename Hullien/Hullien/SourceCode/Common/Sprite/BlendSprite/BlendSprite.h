#ifndef BLEND_SPRITE_UI_H
#define BLEND_SPRITE_UI_H

#include "..\..\Common.h"
#include "..\..\Shader\SpriteShader\SpriteShader.h"
#include "..\SpriteState.h"

enum enBLEND_MODE
{
	EBLEND_MODE_None = -1,

	EBLEND_MODE_Normal,
	EBLEND_MODE_HardLight,

	EBLEND_MODE_Max,
} typedef EBLEND_MODE;

class CBlendSprite : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader\\BlendSprite.hlsl";

	// コンスタントバッファのアプリ側の定義.
	//※シェーダー内のコンスタントバッファと一致している必要あり.
	struct C_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				// ワールド行列.
		ALIGN16 D3DXMATRIX	mWVP;
		ALIGN16 D3DXVECTOR4 vColor;			// カラー(RGBAの型に合わせる).
		ALIGN16 D3DXVECTOR2 vUV;			// UV座標.
		ALIGN16 D3DXVECTOR2 vViewPort;
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
		EBLEND_MODE enBlendMode;
		const char* entryName;

	} typedef SPSShaderName;
public:
	CBlendSprite();
	CBlendSprite(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	virtual ~CBlendSprite();

	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);
	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// 解放.
	void Release();

	// レンダリング.
	// 3D空間に配置.
	void Render(const bool& isBillboard = false);
	// UIとして配置.
	void RenderUI();
	// アニメーション番号の設定.
	void SetAnimNumber(const int& animNumber);
	// フレームタイムの設定(何フレームでアニメーションするか).
	void SetFrameTime(const int& frameTime) { m_pSpriteData->FrameTime = frameTime; }
	// アニメーションを再生する.
	void PlayAnimation() { m_pSpriteData->IsAnimation = true; }
	// 表示画像サイズの取得.
	D3DXVECTOR2 GetSpriteSize() { return { m_pSpriteData->SState.Disp.w, m_pSpriteData->SState.Disp.h }; }
	// 描画座標の取得.
	D3DXVECTOR3 GetRenderPos() { return m_pSpriteData->SState.vPos; }
	// スプライト情報の設定.
	void SetSpriteData( const std::shared_ptr<SSpriteData>& pSpriteData )
	{
		m_pSpriteData = pSpriteData;
		m_vPos = m_pSpriteData->SState.vPos;
	}
	// スプライト情報の取得.
	std::shared_ptr<SSpriteData> GetSpriteData(){ return m_pSpriteData; }

	// 規定色のテクスチャ情報のセット.
	void SetDestTexture( ID3D11ShaderResourceView* pDestTexture ){ m_pDestTexture = pDestTexture; }
private:
	// シェーダーの作成.
	HRESULT InitShader();
	// モデル作成.
	HRESULT InitModel();
	// テクスチャ作成.
	HRESULT CreateTexture( const char* fileName, ID3D11ShaderResourceView** pTexture );
	// サンプラの作成.
	HRESULT InitSample();

	D3DXMATRIX CreateWorldMatrix();
	// UV座標に変換.
	D3DXVECTOR2 ConvertIntoUV();
	// アニメーションの更新.
	void AnimUpdate();

	// 頂点情報の作成.
	void CreateVERTEX(
		const float& w, const float& h,
		const float& u, const float& v);

	// テクスチャの比率を取得.
	int myGcd(int t, int t2)
	{
		if (t2 == 0) return t;
		return myGcd(t2, t % t2);
	}

private:
	std::shared_ptr<SSpriteData>	m_pSpriteData;	// スプライト情報.

	ID3D11VertexShader*				m_pVertexShader;	// 頂点シェーダー 3D描画用に使用.
	ID3D11VertexShader*				m_pVertexShaderUI;	// 頂点シェーダー UD描画用に使用.
	std::vector<ID3D11PixelShader*>	m_pPixelShaderList;	// ピクセルシェーダー.
	ID3D11SamplerState*				m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	ID3D11Buffer*					m_pConstantBuffer;	// コンスタントバッファ.
	ID3D11ShaderResourceView*		m_pDestTexture;		// 規定色のSRV.
	CSpriteShader::VERTEX m_VerticesUI[4];	// 頂点作成用.
};

#endif	// #ifndef BLEND_SPRITE_UI_H.