/**
* @file FrameRate.h.
* @brief �t���[�����[�g�Ǘ��N���X.
* @author ���c���.
*/
#ifndef FRAME_RATE_H
#define FRAME_RATE_H

#include <Windows.h>

/*******************************
*	�t���[�����[�g�Ǘ��N���X.
**/
class CFrameRate
{
	static double FRAME_TIME;
public:
	CFrameRate( double fps = 60.0 );
	~CFrameRate();

	void Wait();

	static double GetFrameTime(){ return m_OutFPS; }
	static double GetFPS(){ return FRAME_TIME; }

private:
	static double	m_OutFPS;
	double			m_FrameTime;
	LARGE_INTEGER	m_StartTime;
	LARGE_INTEGER	m_NowTime;
	LARGE_INTEGER	m_FreqTime;
};

#endif	// #ifndef FRAME_RATE_H.