#ifndef INPUT_H
#define INPUT_H

// �x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)

#include "KeyInput/KeyInput.h"
#include "XInput/XInput.h"
#include "InputStruct.h"

#include <unordered_map>
#include <functional>

//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3DX10.h>
#pragma comment( lib, "d3dx10.lib" )

// ���͎��̖����͈�.
inline static const float	INPUT_AXIS_DEAD_ZONE = 0.4f;
// ���͎���n��l.
// �R���g���[���[����󂯎�����l��n�悷���.
// �l�����炩�ɂȂ�.
// https://hexadrive.jp/lab/tips/976/.
inline static const int		INPUT_AXIS_POW_VALUE = 3;

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
		const unsigned char	MinusKey;	// �}�C�i�X�����Ŏ󂯎��L�[�ԍ�.
		const unsigned char	PlusKey;	// �v���X�����Ŏ󂯎��L�[�ԍ�.

		const std::function<SHORT()> GetValue;// �R���g���[���̎��l�̎擾�֐�.

		const float MinValue;	// �L�[�Ŏ󂯎��ۂ̍ŏ��l.
		const float MaxValue;	// �L�[�Ŏ󂯎��ۂ̍ő�l.

		stAxisBind() : stAxisBind	( 0, 0, [](){ return 0; }, 0.0f, 0.0f ){}
		stAxisBind(
			const unsigned char& mk,
			const unsigned char& pk,
			const std::function<SHORT()>& func,
			const float& minValue,
			const float& maxValue )
			: MinusKey	( mk )
			, PlusKey	( pk )
			, GetValue	( func )
			, MinValue	( minValue )
			, MaxValue	( maxValue )
		{}
	} typedef SAxisBind;

	struct Vector2
	{
		float x;
		float y;
		float length;

		Vector2() : x (0), y (0), length (0){}
	};

public:
	CInput();
	~CInput();

	// �o�C���h�̏�����.
	static void InitBind();
	// �o�C���h�̏����� : �O������ݒ�p.
	static void InitBind(
		std::function<void(std::unordered_map<EKeyBind, SKeyBindPair>&)>	keyFunc,
		std::function<void(std::unordered_map<EAxisBind, SAxisBind>&)>		axisFunc );

	// �L�[�o�C���h�̏�����.
	static void InitKeyBind();
	// ���o�C���h�̏�����.
	static void InitAxisBind();
	// �L�[�o�C���h�̏����� : �O������ݒ�p.
	// �ȉ��g�p��.
	/*
		CInput::InitKeyBind( []( std::unordered_map<EKeyBind, CInput::SKeyBindPair>& list )
		{
			list =
			{
				�L�[�o�C���h�񋓑�,		�ݒ肷��L�[�A�Q�[���p�b�h�̃{�^���̐ݒ�.
				{ EKeyBind::Decision,	CInput::SKeyBindPair( VK_RETURN,	XINPUT_GAMEPAD_B )	},
				{ EKeyBind::Cancel,		CInput::SKeyBindPair( VK_BACK,		XINPUT_GAMEPAD_A )	},
			};
		};
	*/
	static void InitKeyBind( std::function<void(std::unordered_map<EKeyBind, SKeyBindPair>&)> func );
	// ���o�C���h�̏����� : �O������ݒ�p.
	// �ȉ��g�p��.
	/*
		CInput::InitAxisBind( []( std::unordered_map<EAxisBind, SAxisBind>& list )
		{
			list =
			{
				���o�C���h�̗񋓑�		�ݒ肷��L�[�A�󂯎�鎲�̊֐��A�󂯎��ŏ��l�A�ő�l.
				{ EAxisBind::L_Forward,	SAxisBind( 'S', 'W', [](){ return CXInput::LThumbY_Axis(); }, -1.0f, 1.0f ) },
				{ EAxisBind::L_Right,	SAxisBind( 'A', 'D', [](){ return CXInput::LThumbX_Axis(); }, -1.0f, 1.0f ) },
			};
		};
	*/
	static void InitAxisBind( std::function<void(std::unordered_map<EAxisBind, SAxisBind>&)> func );

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

	// ���l�̎擾(vector2).
	static Vector2 GetAxisVector( const EAxisBind& key_x, const EAxisBind& key_y );
#ifdef	__D3DX9MATH_H__	//�uD3DX�`�v�n���Ă΂ꂢ��Έȉ��̊֐����g�p�ł���.
	// ���l�̎擾(D3DXVECTOR2).
	static D3DXVECTOR2 GetAxisDxVector( const EAxisBind& key_x, const EAxisBind& key_y );
#endif	// #ifdef __D3DX9MATH_H__

	// ���l�̎擾.
	static float GetAxisValue( const EAxisBind& key );

private:
	// �C���X�^���X�̎擾.
	static CInput* GetInstance();

private:
	std::unordered_map<EKeyBind, SKeyBindPair>	m_KeyBindList;	// KeyBindPair�̃��X�g.
	std::unordered_map<EAxisBind, SAxisBind>	m_AxisBindList;	// AxisBind�̃��X�g.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CInput( const CInput & )				= delete;
	CInput( CInput && )						= delete;
	CInput& operator = ( const CInput & )	= delete;
	CInput& operator = ( CInput && )		= delete;

};

#endif	// #ifndef INPUT_H.