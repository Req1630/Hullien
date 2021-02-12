/**
* @file AlienList.h.
* @brief 宇宙人リストヘッダー.
* @author 福田玲也.
*/
#ifndef ALIEN_LIST_H
#define ALIEN_LIST_H

enum class enAlienList : unsigned char
{
	None,

	A = 0,
	Ada,
	B,
	Bda,
	C,
	D,
	
	Max,

	First	= A,	// 開始位置.
	Last	= D,	// 終了位置.

} typedef EAlienList;

#endif	// #ifndef ALIEN_LIST_H.