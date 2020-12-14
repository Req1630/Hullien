#ifndef KEY_INPUT_H
#define KEY_INPUT_H

#include <Windows.h>

/*******************************
* �L�[���͂��擾����N���X.
**/
class CKeyInput
{
	static const int KEY_MAX = 256;	// �L�[�̍ő�l.

public:
	// �L�[�̏��.
	enum enKeyState : unsigned char
	{
		EKeyState_NotPress	= 1 << 0,	// ���������Ă��Ȃ�.
		EKeyState_Moment	= 1 << 1,	// �������u��.
		EKeyState_Hold		= 1 << 2,	// ���������Ă���.
		EKeyState_Release	= 1 << 3,	// �������u��.
	} typedef EKeyState;
	typedef char KeyState;

public:
	CKeyInput();
	~CKeyInput();

	// �L�[�̏�Ԃ̍X�V.
	static void Update();

	// �L�[�̏�Ԃ̎擾.
	static KeyState GetState( const unsigned char& key );

	// ������Ă���Ƃ�.
	static bool IsPress( const unsigned char& key );
	// �����ꂽ�u��.
	static bool IsMomentPress( const unsigned char& key );
	// ���������Ă���Ƃ�.
	static bool IsHold( const unsigned char& key );
	// �������u��.
	static bool IsRelease( const unsigned char& key );
	// �����Ă��Ȃ�.
	static bool NotPress( const unsigned char& key );

private:
	// �C���X�^���X�̎擾.
	static CKeyInput* GetInstance();

	// �L�[�̏�Ԃ̍X�V.
	void KeyStateUpdate();

private:
	unsigned char m_InputState[256];	// ���݂̃L�[�̏��.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CKeyInput( const CKeyInput & )				= delete;
	CKeyInput( CKeyInput && )					= delete;
	CKeyInput& operator = ( const CKeyInput & )	= delete;
	CKeyInput& operator = ( CKeyInput && )		= delete;
};

#endif	// #ifndef KEY_INPUT_H.