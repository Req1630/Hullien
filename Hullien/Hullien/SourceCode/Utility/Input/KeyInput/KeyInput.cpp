#include "KeyInput.h"

#include <Windows.h>
#include <memory>

CKeyInput::CKeyInput()
	: m_InputState() 
{
	for( auto& k : m_InputState ) k = EInputState_NotPress;
}

CKeyInput::~CKeyInput()
{}

// キーの状態の更新.
void CKeyInput::Update()
{
	GetInstance()->KeyStateUpdate();
}

// キーの状態の取得.
unsigned char CKeyInput::GetState( const unsigned char& key )
{
	return GetInstance()->m_InputState[key];
}

// 押されているとき.
bool CKeyInput::IsPress( const unsigned char& key )
{
	return GetState( key ) == EInputState_MomentPress || GetState( key ) == EInputState_Hold;
}

// 押された瞬間.
bool CKeyInput::IsMomentPress( const unsigned char& key )
{
	return GetState( key ) == EInputState_MomentPress;
}

// 長押ししているとき.
bool CKeyInput::IsHold( const unsigned char& key )
{
	return GetState( key ) == EInputState_Hold;
}

// 離した瞬間.
bool CKeyInput::IsRelease( const unsigned char& key )
{
	return GetState( key ) == EInputState_Release;
}

// 押していない.
bool CKeyInput::NotPress( const unsigned char& key )
{
	return GetState( key ) == EInputState_NotPress;
}


// インスタンスの取得.
CKeyInput* CKeyInput::GetInstance()
{
	static std::unique_ptr<CKeyInput> pInstance = std::make_unique<CKeyInput>();
	return pInstance.get();
}

// キーの状態の更新.
void CKeyInput::KeyStateUpdate()
{
	BYTE keyState[KEY_MAX];
	GetKeyboardState( keyState );
	for( int i = 0; i < KEY_MAX; i++ ){
		if( keyState[i] & 0x80 ){
			if( m_InputState[i] & EInputState_NotPress ){
				m_InputState[i] = EInputState_MomentPress;
			} else {
				m_InputState[i] = EInputState_Hold;
			}
		} else {
			if( m_InputState[i] == EInputState_Release ){
				m_InputState[i] = EInputState_NotPress;
			} else if( m_InputState[i] != EInputState_NotPress ){
				m_InputState[i] = EInputState_Release;
			}
		}
	}
};