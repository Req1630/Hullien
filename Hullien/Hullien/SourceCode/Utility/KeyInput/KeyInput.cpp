#include "KeyInput.h"

#include <Windows.h>
#include <memory>

CKeyInput::CKeyInput()
	: m_InputState() 
{}

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
	//if( GetAsyncKeyState(key) & 0x8000 ){
	//	if( GetInstance()->m_InputState[key] & EKeyState_Press )	GetInstance()->m_InputState[key] = EKeyState_Hold;
	//	if(!(GetInstance()->m_InputState[key] & EKeyState_Hold ))	GetInstance()->m_InputState[key] = EKeyState_Press;
	//} else {
	//	if( GetInstance()->m_InputState[key] & EKeyState_Releae )	GetInstance()->m_InputState[key] = EKeyState_NotPress;
	//	if( GetInstance()->m_InputState[key] != EKeyState_NotPress )GetInstance()->m_InputState[key] = EKeyState_Releae;
	//}
	return GetInstance()->m_InputState[key];
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
	for( int i = 0; i < KEY_MAX; i++ ){
		if( GetKeyState(i) & 0x8000 ){
			if( m_InputState[i] & EKeyState_Press )	m_InputState[i] = EKeyState_Hold;
			if(!(m_InputState[i] & EKeyState_Hold ))m_InputState[i] = EKeyState_Press;
		} else {
			if( m_InputState[i] & EKeyState_Releae )	m_InputState[i] = EKeyState_NotPress;
			if( m_InputState[i] != EKeyState_NotPress )	m_InputState[i] = EKeyState_Releae;
		}
	}
};