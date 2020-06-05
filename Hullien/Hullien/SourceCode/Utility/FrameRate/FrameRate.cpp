#include "FrameRate.h"
#include <thread>
#include <chrono>

double CFrameRate::FRAME_TIME = 0.0;
double CFrameRate::m_OutFPS = 0.0;

CFrameRate::CFrameRate( double fps )
	: m_FrameTime	( 0.0 )
	, m_StartTime	()
	, m_NowTime		()
	, m_FreqTime	()
{
	FRAME_TIME = 1.0 / fps;
	QueryPerformanceFrequency( &m_FreqTime );
	QueryPerformanceCounter( &m_StartTime );
}

CFrameRate::~CFrameRate()
{
}

void CFrameRate::Wait()
{
	// ���݂̎��Ԃ��擾.
	QueryPerformanceCounter( &m_NowTime );
	// (���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��).
	m_FrameTime =
		(static_cast<double>(m_NowTime.QuadPart) - static_cast<double>(m_StartTime.QuadPart)) / 
		static_cast<double>(m_FreqTime.QuadPart);

	m_OutFPS = 0.0;
	// �������Ԃɗ]�T������ꍇ�͂��̕��ҋ@.
	if( m_FrameTime < FRAME_TIME ){
		// �ҋ@���Ԃ��v�Z.
		DWORD sleepTime =
			static_cast<DWORD>((FRAME_TIME - m_FrameTime) * 1000.0);

		timeBeginPeriod(1);
		Sleep( sleepTime );	// �ҋ@.
		timeEndPeriod(1); 

		return;
	}
	m_OutFPS = 1.0/m_FrameTime;

	m_StartTime = m_NowTime;
}