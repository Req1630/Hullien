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

	// �o�߃t���[�����I���t���[���𒴂��Ă��邩.
	bool IsNowFrameOver() const { return NowFrame >= EndFrame; }

} typedef SAnimFrame;

// �A�j���[�V�����̃t���[�����̃��X�g.
typedef std::vector<SAnimFrame> AAnimFrameList;	

};

#endif	// #ifndef ANIMATION_PARAM_H.