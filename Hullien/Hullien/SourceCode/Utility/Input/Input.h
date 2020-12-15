#ifndef INPUT_H
#define INPUT_H

#include "KeyInput/KeyInput.h"
#include "XInput/XInput.h"
#include "InputStruct.h"

#include <unordered_map>
#include <functional>

class CInput
{
public:
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

	// ���o�C���h.
	struct stAxisBind
	{
		const unsigned char	PlusKey;	// �L�[�ԍ�.
		const unsigned char	MinusKey;	// �L�[�ԍ�.

		std::function<SHORT()> GetValue;// �l�̎擾�֐�.

		const float MinValue;
		const float MaxValue;

		stAxisBind()
			: stAxisBind	( 0, 0, [](){ return 0; }, 0.0f, 0.0f )
		{}
		stAxisBind(
			const unsigned char& pk,
			const unsigned char& mk,
			const std::function<SHORT()>& func,
			const float& minValue,
			const float& maxValue )
			: PlusKey	( pk )
			, MinusKey	( mk )
			, GetValue	( func )
			, MinValue	( minValue )
			, MaxValue	( maxValue )
		{}
	} typedef SAxisBind;

	template<typename T>
	struct Vector2
	{
		T x;
		T y;
		T length;

		Vector2()
			: x	(0), y (0), length (0)
		{}
	};

public:
	CInput();
	~CInput();

	// �L�[�o�C���h�̏�����.
	static void InitKeyBind();
	// �L�[�o�C���h�̏����� : �O������ݒ�p.
	// �ȉ��g�p��.
	/*
		CInput::InitKeyBind( []( std::unordered_map<EKeyBind, CInput::SKeyBindPair>& list )
		{
			list =
			{
				{ EKeyBind::Decision,	CInput::SKeyBindPair( VK_RETURN,	XINPUT_GAMEPAD_B )	},
				{ EKeyBind::Cancel,		CInput::SKeyBindPair( VK_BACK,		XINPUT_GAMEPAD_A )	},
			};
		};
	*/
	static void InitKeyBind( std::function<void(std::unordered_map<EKeyBind, SKeyBindPair>&)> func );

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

	template<typename T>
	static Vector2<T> GetAxisValue( const EAxisBind& key_x, const EAxisBind& key_y );

	// ���l�̎擾.
	static float GetAxisValue( const EAxisBind& key );

private:
	// �C���X�^���X�̎擾.
	static CInput* GetInstance();

private:
	std::unordered_map<EKeyBind, SKeyBindPair>	m_KeyBindList;	// KeyBindPair�̃��X�g.
	std::unordered_map<EAxisBind, SAxisBind>	m_AxisBindList;	// KeyBindPair�̃��X�g.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CInput( const CInput & )				= delete;
	CInput( CInput && )						= delete;
	CInput& operator = ( const CInput & )	= delete;
	CInput& operator = ( CInput && )		= delete;

};

template<typename T>
CInput::Vector2<T> CInput::GetAxisValue( const EAxisBind& key_x, const EAxisBind& key_y )
{
	Vector2<T> t;
	t.x = GetAxisValue(key_x);
	t.y = GetAxisValue(key_y);

	if( fabsf(t.x) < 0.4f && fabsf(t.y) < 0.4f ){
		t.x = 0.0f; t.y = 0.0f;
	}

	return t;
}

#endif	// #ifndef INPUT_H.