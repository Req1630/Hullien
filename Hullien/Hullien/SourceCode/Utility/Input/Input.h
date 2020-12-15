#ifndef INPUT_H
#define INPUT_H

#include "KeyInput/KeyInput.h"
#include "XInput/XInput.h"
#include "InputStruct.h"

#include <unordered_map>
#include <functional>

class CInput
{
	// �L�[�o�C���h�̃y�A�\����.
	struct stKeyBindPair
	{
		const unsigned char	Key;	// �L�[�ԍ�.
		const WORD			Button;	// Xinput�̃{�^���ԍ�.

		stKeyBindPair()
			: stKeyBindPair	( 0, 0 )
		{}
		stKeyBindPair( 
			const unsigned char&	k,
			const WORD&				b )
			: Key		( k )
			, Button	( b )
		{}

	} typedef SKeyBindPair;

public:
	CInput();
	~CInput();

	// �L�[�o�C���h�̏�����.
	static void InitKeyBind();

	// ���͏�Ԃ̍X�V.
	static void StateUpdate();

	// ������Ă���Ƃ�.
	static bool IsPress( const EKeyBind& key );
	// �����ꂽ�u��.
	static bool IsMomentPress( const EKeyBind& key );
	// ���������Ă���Ƃ�.
	static bool IsHold( const EKeyBind& key );
	// �������u��.
	static bool IsRelease( const EKeyBind& key );
	// �����Ă��Ȃ�.
	static bool NotPress( const EKeyBind& key );

private:
	// �C���X�^���X�̎擾.
	static CInput* GetInstance();

private:
	std::unordered_map<EKeyBind, SKeyBindPair>	m_KeyBindList;	// KeyBindPair�̃��X�g.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CInput( const CInput & )				= delete;
	CInput( CInput && )						= delete;
	CInput& operator = ( const CInput & )	= delete;
	CInput& operator = ( CInput && )		= delete;

};

#endif	// #ifndef INPUT_H.