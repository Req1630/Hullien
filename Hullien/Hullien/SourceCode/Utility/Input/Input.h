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
	// キーバインドのペア構造体.
	struct stKeyBindPair
	{
		const unsigned char	Key;	// キー番号.
		const WORD			Button;	// Xinputのボタン番号.

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

	// 軸バインド.
	struct stAxisBind
	{
		const unsigned char	PlusKey;	// キー番号.
		const unsigned char	MinusKey;	// キー番号.

		std::function<SHORT()> GetValue;// 値の取得関数.

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

	// キーバインドの初期化.
	static void InitKeyBind();
	// キーバインドの初期化 : 外部から設定用.
	// 以下使用例.
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

	// 入力状態の更新.
	static void StateUpdate();

	// 押されているとき.
	static bool IsPress( const EKeyBind& key );
	// 押された瞬間.
	static bool IsMomentPress( const EKeyBind& key );
	// 長押ししているとき.
	static bool IsHold( const EKeyBind& key );
	// 離した瞬間.
	static bool IsRelease( const EKeyBind& key );
	// 押していない.
	static bool NotPress( const EKeyBind& key );

	template<typename T>
	static Vector2<T> GetAxisValue( const EAxisBind& key_x, const EAxisBind& key_y );

	// 軸値の取得.
	static float GetAxisValue( const EAxisBind& key );

private:
	// インスタンスの取得.
	static CInput* GetInstance();

private:
	std::unordered_map<EKeyBind, SKeyBindPair>	m_KeyBindList;	// KeyBindPairのリスト.
	std::unordered_map<EAxisBind, SAxisBind>	m_AxisBindList;	// KeyBindPairのリスト.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
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