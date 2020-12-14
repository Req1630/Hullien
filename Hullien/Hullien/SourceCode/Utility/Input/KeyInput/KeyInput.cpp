#include "KeyInput.h"

#include <Windows.h>
#include <memory>

CKeyInput::CKeyInput()
	: m_InputState() 
{
	for( auto& k : m_InputState ) k = EKeyState_NotPress;
}

CKeyInput::~CKeyInput()
{}

// �L�[�̏�Ԃ̍X�V.
void CKeyInput::Update()
{
	GetInstance()->KeyStateUpdate();
}

// �L�[�̏�Ԃ̎擾.
CKeyInput::KeyState CKeyInput::GetState( const unsigned char& key )
{
	return GetInstance()->m_InputState[key];
}

// ������Ă���Ƃ�.
bool CKeyInput::IsPress( const unsigned char& key )
{
	return GetState( key ) == EKeyState_Moment || GetState( key ) == EKeyState_Hold;
}

// �����ꂽ�u��.
bool CKeyInput::IsMomentPress( const unsigned char& key )
{
	return GetState( key ) == EKeyState_Moment;
}

// ���������Ă���Ƃ�.
bool CKeyInput::IsHold( const unsigned char& key )
{
	return GetState( key ) == EKeyState_Hold;
}

// �������u��.
bool CKeyInput::IsRelease( const unsigned char& key )
{
	return GetState( key ) == EKeyState_Release;
}

// �����Ă��Ȃ�.
bool CKeyInput::NotPress( const unsigned char& key )
{
	return GetState( key ) == EKeyState_NotPress;
}


// �C���X�^���X�̎擾.
CKeyInput* CKeyInput::GetInstance()
{
	static std::unique_ptr<CKeyInput> pInstance = std::make_unique<CKeyInput>();
	return pInstance.get();
}

// �L�[�̏�Ԃ̍X�V.
void CKeyInput::KeyStateUpdate()
{
	BYTE keyState[KEY_MAX];
	GetKeyboardState( keyState );
	for( int i = 0; i < KEY_MAX; i++ ){
		if( keyState[i] & 0x80 ){
			if( m_InputState[i] & EKeyState::EKeyState_NotPress ){
				m_InputState[i] = EKeyState::EKeyState_Moment;
			} else {
				m_InputState[i] = EKeyState::EKeyState_Hold;
			}
		} else {
			if( m_InputState[i] == EKeyState::EKeyState_Release ){
				m_InputState[i] = EKeyState::EKeyState_NotPress;
			} else if( m_InputState[i] != EKeyState::EKeyState_NotPress ){
				m_InputState[i] = EKeyState::EKeyState_Release;
			}
		}
	}
};