/**
* @file SphereModel.h.
* @brief 球体のモデル描画クラス.
* @author 福田玲也.
*/
#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

#include "..\..\..\Common\Common.h"

class CSimpleShader;

/*********************************************
*	球体のモデル描画クラス.
**/
class CSphereModel : public CCommon
{
private: //------- 定数 -------.
	inline static const D3DXVECTOR4 MODEL_COLOR			= { 0.0f, 0.0f, 1.0f, 1.0f };	// モデルの色.
	inline static const D3DXVECTOR4 MODEL_CHANGE_COLOR	= { 1.0f, 0.0f, 0.0f, 1.0f };	// モデルの色.

public: //------- 関数 -------.
	CSphereModel();
	virtual ~CSphereModel();

	// 初期化.
	HRESULT Init( const float& radius );
	// 描画.
	void Render();
	// 色を変える.
	void ChangeColor( const bool& changed ){ m_IsColorChanged = changed; }

private: //------- 関数 -------.
	// WVP Matrixの作成.
	D3DXMATRIX CreateWVPMatrix( D3DXMATRIX& mWVP );
	// モデルの作成.
	HRESULT InitModel( const float& fRadius );

private: //------- 変数 -------.
	std::unique_ptr<CSimpleShader>	m_pSimpleShader;	// シンプルシェーダ.
	ID3D11Buffer*					m_pVertexBuffer;	// 頂点バッファ.
	ID3D11Buffer*					m_pIndexBuffer;		// インデックスバッファ.

	bool m_IsColorChanged;	// 色が変わったかどうか.
};

#endif	// #ifndef SPHERE_MODEL_H.