#ifndef GIRL_PARAM_H
#define GIRL_PARAM_H

#include "..\Character.h"

// 女の子パラメーター.
struct stGirlParam : public CCharacter::SParameter
{
	float		RotatlonalSpeed;	// 回転速度.
	D3DXVECTOR3 InitPosition;		// 初期座標.
	float		InitPosLenght;		// 初期座標の許容の長さ.
	float		SearchCollRadius;	// 索敵用のスフィアの半径.
	D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
	float		SphereAdjRadius;	// スフィアの調整半径.

	stGirlParam()
		: RotatlonalSpeed	( 0.0f )
		, InitPosition		( 0.0f, 0.0f, 0.0f )
		, InitPosLenght		( 1.0f )
		, SearchCollRadius	( 10.0f )
		, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius	( 0.0f )
	{}
} typedef SGirlParam;

#endif	// #ifndef GIRL_PARAM_H.