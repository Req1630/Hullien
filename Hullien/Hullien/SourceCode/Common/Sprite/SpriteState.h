#ifndef SPRITE_STATE_H
#define SPRITE_STATE_H

#include "..\Common.h"

// 幅高さ構造体(float型).
struct WHDIZE_FLOAT
{
	float w;	// 幅.
	float h;	// 高さ.
};

enum class enLocalPosition : unsigned char
{
	None,

	LeftUp = 0,	// 左上.
	Left,		// 左.
	LeftDown,	// 左下.
	Down,		// 下.
	RightDown,	// 右下.
	Right,		// 右.
	RightUp,	// 右上.
	Up,			// 上.
	Center,		// 中央.

	Max,
} typedef ELocalPosition;

// スプライト構造体.
struct stSpriteState
{
	int				LocalPosNum;// ローカル座標の
	WHDIZE_FLOAT	Disp;		// 表示幅,高さ.
	WHDIZE_FLOAT	Base;		// 元画像の幅,高さ.
	WHDIZE_FLOAT	Stride;		// 1コマ当たりの幅,高さ.
	D3DXVECTOR3		vPos;		// 座標.
	int				AnimNum;	// アニメーション数.

	enum enSPRITE_STATE : unsigned char
	{
		None,

		enLocalPosNum = 0,
		enDisp_w,
		enDisp_h,
		enBase_w,
		enBase_h,
		enStride_w,
		enStride_h,
		envPos_x,
		envPos_y,
		envPos_z,
		enAnimNum,

		MAX = enAnimNum,
	} typedef ESPRITE_STATE;
} typedef SSpriteState;

// スプライト情報.
struct stSpriteData
{
	ID3D11InputLayout*	pVertexLayout;		// 頂点レイアウト.
	ID3D11Buffer*		pVertexBuffer;		// 頂点バッファ.
	ID3D11Buffer*		pVertexBufferUI;	// 頂点バッファ.

	ID3D11ShaderResourceView*	pTexture;	// テクスチャ.

	SSpriteState	SState;				// スプライト情報.
	POINT			PatternNo;			// パターン番号.
	POINT			PatternMax;			// パターン最大数.
	D3DXVECTOR2		UV;					// テクスチャUV座標.
	int				AnimNumber;			// アニメーションナンバー.
	int				FrameCount;			// フレームカウント.
	int				FrameCountSpeed;	// フレームカウント速度.
	int				FrameTime;			// フレームタイム.
	bool			IsAnimation;		// アニメーションするか.
	enLocalPosition enLocalPosition;	// ローカル座標の種類.

	stSpriteData()
		: pVertexLayout		( nullptr )
		, pVertexBuffer		( nullptr )
		, pVertexBufferUI	( nullptr )
		, pTexture			( nullptr )
		, SState			()
		, PatternNo			{ 1, 1 }
		, PatternMax		{ 1, 1 }
		, UV				( 1.0f, 1.0f )
		, AnimNumber		( 0 )
		, FrameCount		( 0 )
		, FrameCountSpeed	( 1 )
		, FrameTime			( 20 )
		, IsAnimation		( true )
		, enLocalPosition	( ELocalPosition::None )
	{}
	~stSpriteData()
	{
		SAFE_RELEASE( pTexture );
		SAFE_RELEASE( pVertexBufferUI );
		SAFE_RELEASE( pVertexBuffer );
		SAFE_RELEASE( pVertexLayout );
	}
	void Release()
	{
		SAFE_RELEASE( pTexture );
		SAFE_RELEASE( pVertexBufferUI );
		SAFE_RELEASE( pVertexBuffer );
		SAFE_RELEASE( pVertexLayout );
	}
} typedef SSpriteData;

#endif	// #ifndef SPRITE_STATE_H.