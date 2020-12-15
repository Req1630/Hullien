#include "XInput.h"
#include "..\..\FileManager\FileManager.h"
#include <algorithm>

CXInput::CXInput()
	: m_State			( FOUR_LIMITED_CONTROLLER )
	, m_Vibration		( FOUR_LIMITED_CONTROLLER )
	, m_ConnectedCount	( 0 )
	, m_IsVibration		( false )
{
	std::unordered_map<WORD, EInputState> inputState;
	inputState[XINPUT_GAMEPAD_DPAD_UP]			= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_DPAD_DOWN]		= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_DPAD_LEFT]		= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_DPAD_RIGHT]		= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_START]			= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_BACK]				= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_LEFT_THUMB]		= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_RIGHT_THUMB]		= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_LEFT_SHOULDER]	= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_RIGHT_SHOULDER]	= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_A]				= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_B]				= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_X]				= EInputState_NotPress;
	inputState[XINPUT_GAMEPAD_Y]				= EInputState_NotPress;
	for( int i = 0; i < FOUR_LIMITED_CONTROLLER; i++ ){
		m_ButtonStateList.emplace_back(inputState);
	}
	CFileManager::BinaryReading( VIBRATION_CONFIG_FILE_PATH, m_IsVibration );
}

CXInput::~CXInput()
{
	for( int i = 0; i < GetInstance()->m_ConnectedCount; i++ ){
		SetVibration( 0, 0, i );	// �o�C�u���[�V�����̏�����.
	}
}

void CXInput::StatsUpdate()
{
	GetInstance()->m_ConnectedCount = 0;	// �ڑ����̏�����.
	DWORD dwResult;	// �ڑ����.
	for( size_t i = 0; i < GetInstance()->m_State.size(); i++ ){
		dwResult = XInputGetState( i, &GetInstance()->m_State[i] );
		if( dwResult == ERROR_SUCCESS ){
			GetInstance()->m_ConnectedCount++;
		}
	}
	// �{�^�����̍X�V.
	GetInstance()->ButtonStateUpdate();
}

// ������Ă���Ƃ�.
bool CXInput::IsPress( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_Hold;
}

// �����ꂽ�u��.
bool CXInput::IsMomentPress( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_MomentPress;
}

// ���������Ă���Ƃ�.
bool CXInput::IsHold( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_Hold;
}

// �������u��.
bool CXInput::IsRelease( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_Release;
}

// �����Ă��Ȃ�.
bool CXInput::NotPress( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_NotPress;
}

//--------------------------------.
// L�X�e�B�b�N.
//--------------------------------.
SHORT CXInput::LThumbX_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbLX;
}
SHORT CXInput::LThumbY_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbLY;
}
//--------------------------------.
// R�X�e�B�b�N.
//--------------------------------.
SHORT CXInput::RThumbX_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbRX;
}
SHORT CXInput::RThumbY_Axis( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.sThumbRY;
}

//--------------------------------.
// �g���K�[.
//--------------------------------.
INT CXInput::LTrigger( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.bLeftTrigger;
}
INT CXInput::RTrigger( const int& connectNum )
{
	GetInstance()->ConnectCheck( connectNum );
	return GetInstance()->m_State[connectNum].Gamepad.bRightTrigger;
}

//--------------------------------.
//�U���ݒ�.
//--------------------------------.
bool CXInput::SetVibration( WORD LMotorSpd, WORD RMotorSpd, const int& connectNum  )
{
	if( GetInstance()->m_IsVibration == false ) return false;
	GetInstance()->m_Vibration[connectNum].wLeftMotorSpeed = std::clamp<WORD>( LMotorSpd, INPUT_VIBRATION_MIN, INPUT_VIBRATION_MAX );
	GetInstance()->m_Vibration[connectNum].wRightMotorSpeed = std::clamp<WORD>( RMotorSpd, INPUT_VIBRATION_MIN, INPUT_VIBRATION_MAX );

	if( ERROR_SUCCESS == XInputSetState(
		connectNum, &GetInstance()->m_Vibration[connectNum] ) ){
		return true;
	}
	return false;
}

void CXInput::ConnectCheck( const int& connectNum )
{
	if( connectNum >= m_ConnectedCount ){
//		MessageBox( nullptr,
//			"�R���g���[���[�̐�������܂���", "", MB_OK );
	}
}

bool CXInput::IsButtonInput( const int& connectNum, const WORD& button_mask )
{
	return ( GetInstance()->m_State[connectNum].Gamepad.wButtons & button_mask );
}

void CXInput::ButtonStateUpdate()
{
	int connectNum = 0;
	for( auto& c : m_ButtonStateList ){
		for( auto& s : c ){
			// �{�^����������Ă��邩.
			if( IsButtonInput( connectNum, s.first ) == true ){
				// ����������Ă��Ȃ��ꍇ"�������u��"��.
				// ����������Ă����"������"��.
				if( s.second & EInputState_NotPress ){
					s.second = EInputState_MomentPress;
				} else {
					s.second = EInputState_Hold;
				}
			} else {
				// ���ɗ������u�ԂȂ�"���������Ă��Ȃ����"��.
				// �O�񉽂�������Ă����"�������u��"��.
				if( s.second == EInputState_Release ){
					s.second = EInputState_NotPress;
				} else if ( s.second != EInputState_NotPress ){
					s.second = EInputState_Release;
				}
			}
		}
		connectNum++;
	}

}