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
		SetVibration( 0, 0, i );	// バイブレーションの初期化.
	}
}

void CXInput::StatsUpdate()
{
	GetInstance()->m_ConnectedCount = 0;	// 接続数の初期化.
	DWORD dwResult;	// 接続状態.
	for( size_t i = 0; i < GetInstance()->m_State.size(); i++ ){
		dwResult = XInputGetState( i, &GetInstance()->m_State[i] );
		if( dwResult == ERROR_SUCCESS ){
			GetInstance()->m_ConnectedCount++;
		}
	}
	// ボタン情報の更新.
	GetInstance()->ButtonStateUpdate();
}

// 押されているとき.
bool CXInput::IsPress( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_Hold;
}

// 押された瞬間.
bool CXInput::IsMomentPress( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_MomentPress;
}

// 長押ししているとき.
bool CXInput::IsHold( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_Hold;
}

// 離した瞬間.
bool CXInput::IsRelease( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_Release;
}

// 押していない.
bool CXInput::NotPress( const WORD& button_mask, const int& connectNum )
{
	return GetInstance()->m_ButtonStateList[connectNum][button_mask] == EInputState_NotPress;
}

//--------------------------------.
// Lスティック.
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
// Rスティック.
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
// トリガー.
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
//振動設定.
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
//			"コントローラーの数が足りません", "", MB_OK );
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
			// ボタンが押されているか.
			if( IsButtonInput( connectNum, s.first ) == true ){
				// 何も押されていない場合"押した瞬間"に.
				// 何か押されていれば"長押し"に.
				if( s.second & EInputState_NotPress ){
					s.second = EInputState_MomentPress;
				} else {
					s.second = EInputState_Hold;
				}
			} else {
				// 既に離した瞬間なら"何も押していない状態"に.
				// 前回何か押されていれば"離した瞬間"に.
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