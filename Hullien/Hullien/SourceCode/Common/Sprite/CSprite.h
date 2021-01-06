#ifndef CSPRITE_UI_H
#define CSPRITE_UI_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"
#include "SpriteState.h"

class CSprite : public CCommon
{
public:
	CSprite();
	CSprite(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	virtual ~CSprite();

	// 初期化.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	// 解放.
	void Release();

	// レンダリング.
	// 3D空間に配置.
	void Render( const bool& isBillboard = false );
	// UIとして配置.
	void RenderUI();
	// アニメーション番号の設定.
	void SetAnimNumber( const int& animNumber );
	// フレームタイムの設定(何フレームでアニメーションするか).
	void SetFrameTime( const int& frameTime ) { m_pSpriteData->FrameTime = frameTime; }
	// アニメーションを再生する.
	void PlayAnimation() {m_pSpriteData->IsAnimation = true; }
	// 表示画像サイズの取得.
	D3DXVECTOR2 GetSpriteSize() { return { m_pSpriteData->SState.Disp.w, m_pSpriteData->SState.Disp.h }; }
	// 描画座標の取得.
	D3DXVECTOR3 GetRenderPos() { return m_pSpriteData->SState.vPos; }
	// スプライト情報の設定.
	void SetSpriteData( const std::shared_ptr<SSpriteData>& pSpriteData ){ m_pSpriteData = pSpriteData; }
	// スプライト情報の取得.
	std::shared_ptr<SSpriteData> GetSpriteData(){ return m_pSpriteData; }
private:
	// モデル作成.
	HRESULT InitModel();
	// テクスチャ作成.
	HRESULT CreateTexture( const char* fileName, ID3D11ShaderResourceView** pTexture );
	// サンプラの作成.
	HRESULT InitSample();

	// world行列の作成.
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
	int myGcd( int t, int t2 )
	{
		if (t2 == 0) return t;
		return myGcd(t2, t % t2);
	}
private:
	std::unique_ptr<CSpriteShader>	m_pSpriteShader;	// スプライトシェーダー.
	std::shared_ptr<SSpriteData>	m_pSpriteData;		// スプライト情報.
	ID3D11SamplerState*				m_pSampleLinear;	// サンプラ:テクスチャに各種フィルタをかける.
	CSpriteShader::VERTEX m_VerticesUI[4];
};

#endif	// #ifndef CSPRITE_UI_H.