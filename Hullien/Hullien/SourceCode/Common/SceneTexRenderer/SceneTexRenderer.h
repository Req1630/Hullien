/**
* @file SceneTexRenderer.h.
* @brief ポストエフェクトなどを描画する.
* @author 福田玲也.
*/
#ifndef PERA_RENDERER_H
#define PERA_RENDERER_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

#include <vector>

class CBloom;	// ブルームクラス.
class CBlur;	// ブラークラス.

/*************************************
*	ポストエフェクトなどを描画する.
**/
class CSceneTexRenderer
{
public:
	static const int MAX_CASCADE = 4;	// カスケードの最大数.
	// G-Bufferの種類.
	enum enGBufferNo
	{
		enGBufferNo_COLOR,		// 色情報.
		enGBufferNo_NORMAL,		// 法線情報.
		enGBufferNo_Z_DEPTH,	// 深度値情報.

		enGBufferNo_MAX,	// 最大値.

	} typedef EGBufferNo;

	enum enScreenRenderNo
	{
		EScreenRenderNo_None,	// 通常.
		EScreenRenderNo_Tmp,	// 保存用.

		EScreenRenderNo_Max,
	} typedef EScreenRenderNo;
	// レンダーパス.
	enum class enRenderPass
	{
		Shadow,		// 一回目.
		Trans,		// 二回目.
		GBuffer,	// 三回目.

		Max,	// 最大.
	} typedef ERenderPass;

public:
	//======================================
	//	構造体.
	//======================================
	// コンスタントバッファ.
	struct C_BUFFER_PER_INIT
	{
		ALIGN16 D3DXMATRIX	mW;				// ワールド行列.
		ALIGN16 D3DXVECTOR2	vViewPort;		// UV座標.
	};
	struct C_BUFFER_PER_FRAME
	{
		D3DXVECTOR4 SoftKneePram;
	};
	// 頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// 頂点座標.
		D3DXVECTOR2 Tex;	// テクスチャ座標.
	};

public:
	CSceneTexRenderer();
	~CSceneTexRenderer();

	// インスタンスの取得.
	static CSceneTexRenderer* GetInstance();

	// 初期化.
	static HRESULT Init();

	// 解放.
	static void Release();

	// 描画関数.
	static void Render( const bool& isBloomSmpling = true );
	// 最終描画.
	static void FinalRender();

	// バッファのクリア.
	static void ClearBuffer();

	// レンダリングパスを設定.
	static void SetRenderPass( const ERenderPass& pass ){ GetInstance()->m_NowRenderPass = pass; }
	// 現在のレンダリングパスを取得.
	static ERenderPass GetRenderPass() { return GetInstance()->m_NowRenderPass; }

	// Shadowバッファの設定.
	static void SetShadowBuffer( int i );
	// G-Bufferの設定.
	static void SetGBuffer();
	// TransBufferの設定.
	static void SetTransBuffer();

	// 深度バッファテクスチャを取得.
	static std::vector<ID3D11ShaderResourceView*> GetShadowBuffer(){ return GetInstance()->m_pShadowBufferSRV; }
	// G-Bufferテクスチャを取得.
	static std::vector<ID3D11ShaderResourceView*> GetGBuffer(){ return GetInstance()->m_pGBufferSRV; }
	// TransBufferテクスチャを取得.
	static ID3D11ShaderResourceView* GetTransBaffer(){ return GetInstance()->m_pTransBufferSRV; }
	// tmpスクリーンテクスチャ.
	static ID3D11ShaderResourceView* GetTmpScreenTexture(){ return GetInstance()->m_pScreenSRV[EScreenRenderNo_Tmp]; }
	// ブラーテクスチャの取得.
	static ID3D11ShaderResourceView* GetBlurTexture();

	// 現在のスクリーンを保存する.
	static void SetSaveScreen( const bool& isSave ){ GetInstance()->m_IsSaveScreen = isSave; }

	static void SetIsStartLoad( const bool& isLoad ){ GetInstance()->m_IsStartGameLoad = isLoad; }
	static void SetIsEndLoad( const bool& isLoad ){ GetInstance()->m_IsEndGameLoad = isLoad; }

	// ウィンドウサイズが変更された時に呼ぶ.
	static void Resize();

private:
	// ShadowBufferの作成.
	HRESULT InitShadowBufferTex();
	// G-Bufferの作成.
	HRESULT InitGBufferTex();
	// TransBufferの作成.
	HRESULT InitTransBufferTex();
	// Antialiasingの作成.
	HRESULT InitAntialiasingTex();
	// 輝度用の作成.
	HRESULT InitDownLuminanceTex();
	// スクショ用の作成.
	HRESULT InitScreenShotTex();
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
	ID3D11Device*			m_pDevice11;	// デバイスオブジェクト.
	ID3D11DeviceContext*	m_pContext11;	// デバイスコンテキスト.

	std::unique_ptr<CBloom>	m_pBloom;	// ブルーム処理クラス.
	std::unique_ptr<CBlur>	m_pBlur;	// ブラー処理クラス.

	std::vector<ID3D11RenderTargetView*>	m_pShadowBufferRTV;	// シャドウマップ用バッファのレンダーターゲットビュー.
	std::vector<ID3D11ShaderResourceView*>	m_pShadowBufferSRV;	// シャドウマップ用バッファのステンシルビュー.
	std::vector<ID3D11Texture2D*>			m_pShadowBufferTex;	// シャドウマップ用バッファのテクスチャー2D.

	std::vector<ID3D11RenderTargetView*>	m_pGBufferRTV;	// G-Bufferのレンダーターゲットビュー.
	std::vector<ID3D11ShaderResourceView*>	m_pGBufferSRV;	// G-Bufferのシェーダーリソースビュー.
	std::vector<ID3D11Texture2D*>			m_pGBufferTex;	// G-Bufferのテクスチャ2D.

	std::vector<ID3D11RenderTargetView*>	m_pScreenRTV;	// 全描画用レンダーターゲットビュー.
	std::vector<ID3D11ShaderResourceView*>	m_pScreenSRV;	// 全描画用シェーダーリソースビュー.
	std::vector<ID3D11Texture2D*>			m_pScreenTex;	// 全描画用テクスチャ2D.

	ID3D11RenderTargetView*		m_pTransBufferRTV;		// 半透明描画用のレンダーターゲットビュー.
	ID3D11ShaderResourceView*	m_pTransBufferSRV;		// 半透明描画用のシェーダーリソースビュー.
	ID3D11Texture2D*			m_pTransBufferTex;		// 半透明描画用のテクスチャ2D.

	ID3D11RenderTargetView*		m_pAntialiasingRTV;		// アンチエイリアスのレンダーターゲットビュー.
	ID3D11ShaderResourceView*	m_pAntialiasingSRV;		// アンチエイリアスのシェーダーリソースビュー.
	ID3D11Texture2D*			m_pAntialiasingTex;		// アンチエイリアスのテクスチャ2D.

	ID3D11RenderTargetView*		m_pDownLuminanceRTV;	// 輝度を落としたテクスチャのレンダーターゲットビュー.
	ID3D11ShaderResourceView*	m_pDownLuminanceSRV;	// 輝度を落としたテクスチャのシェーダーリソースビュー.
	ID3D11Texture2D*			m_pDownLuminanceTex;	// 輝度を落としたテクスチャのテクスチャ2D.


	ID3D11VertexShader*		m_pVertexShader;		// 頂点シェーダー.
	ID3D11PixelShader*		m_pPixelShader;			// ピクセルシェーダー.
	ID3D11PixelShader*		m_pEffectPixelShader;	// ピクセルシェーダー.
	ID3D11PixelShader*		m_pFinalPixelShader;	// ピクセルシェーダー.
	ID3D11InputLayout*		m_pVertexLayout;		// 頂点レイアウト.
	ID3D11Buffer*			m_pConstantBufferInit;	// コンスタントバッファ.
	ID3D11Buffer*			m_pConstantBufferFrame;	// コンスタントバッファ.
	ID3D11Buffer*			m_pVertexBuffer;		// 頂点バッファ.
	ID3D11SamplerState*		m_pSampleLinear;		// サンプラ:テクスチャに各種フィルタをかける.

	UINT					m_WndWidth;			// ウィンドウ幅.
	UINT					m_WndHeight;		// ウィンドウ高さ.

	ERenderPass				m_NowRenderPass;	// 現在のレンダリングパス.

	bool					m_IsSaveScreen;
	bool					m_IsGameLoad;
	bool					m_IsStartGameLoad;
	bool					m_IsEndGameLoad;

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CSceneTexRenderer( const CSceneTexRenderer & )				= delete;
	CSceneTexRenderer& operator = ( const CSceneTexRenderer & )	= delete;
	CSceneTexRenderer( CSceneTexRenderer && )					= delete;
	CSceneTexRenderer& operator = ( CSceneTexRenderer && )		= delete;
};

#endif	// #ifndef PERA_RENDERER_H.