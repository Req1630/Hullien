/**
* @file AnimationParamInfo.h.
* @brief アニメーションの情報を持たせる構造体.
* @author 福田玲也.
*/
#ifndef ANIMATION_PARAM_H
#define ANIMATION_PARAM_H

#include <vector>

namespace anim
{

// アニメーションフレーム構造体.
struct stAnimFrame
{
	double NowFrame;	// 経過フレーム.
	double EndFrame;	// 終了フレーム.

	stAnimFrame() : NowFrame ( 0.0 ), EndFrame ( 0.0 )
	{}
	stAnimFrame( const double& nowFrame, const double& endFrame ) 
		: NowFrame	( nowFrame )
		, EndFrame	( endFrame )
	{}

	// フレームの更新.
	void UpdateFrame( const double& animSpeed ){ NowFrame += animSpeed; }
	// 経過フレームが終了フレームを超えているか.
	bool IsNowFrameOver() const { return NowFrame >= EndFrame; }

} typedef SAnimFrame;

// アニメーションのフレーム情報のリスト.
typedef std::vector<SAnimFrame> AAnimFrameList;
/*
AAnimFrameList
A = arrary
*/

};

#endif	// #ifndef ANIMATION_PARAM_H.