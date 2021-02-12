/**
* @file AnimationParamInfo.h.
* @brief �A�j���[�V�����̏�����������\����.
* @author ���c���.
*/
#ifndef ANIMATION_PARAM_H
#define ANIMATION_PARAM_H

#include <vector>

namespace anim
{

// �A�j���[�V�����t���[���\����.
struct stAnimFrame
{
	double NowFrame;	// �o�߃t���[��.
	double EndFrame;	// �I���t���[��.

	stAnimFrame() : NowFrame ( 0.0 ), EndFrame ( 0.0 )
	{}
	stAnimFrame( const double& nowFrame, const double& endFrame ) 
		: NowFrame	( nowFrame )
		, EndFrame	( endFrame )
	{}

	// �t���[���̍X�V.
	void UpdateFrame( const double& animSpeed ){ NowFrame += animSpeed; }
	// �o�߃t���[�����I���t���[���𒴂��Ă��邩.
	bool IsNowFrameOver() const { return NowFrame >= EndFrame; }

} typedef SAnimFrame;

// �A�j���[�V�����̃t���[�����̃��X�g.
typedef std::vector<SAnimFrame> AAnimFrameList;
/*
AAnimFrameList
A = arrary
*/

};

#endif	// #ifndef ANIMATION_PARAM_H.