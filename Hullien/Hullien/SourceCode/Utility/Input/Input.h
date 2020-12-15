#ifndef INPUT_H
#define INPUT_H

#include "KeyInput/KeyInput.h"
#include "XInput/XInput.h"
#include "InputStruct.h"

#include <unordered_map>
#include <functional>

class CInput
{
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

public:
	CInput();
	~CInput();

	// キーバインドの初期化.
	static void InitKeyBind();

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

private:
	// インスタンスの取得.
	static CInput* GetInstance();

private:
	std::unordered_map<EKeyBind, SKeyBindPair>	m_KeyBindList;	// KeyBindPairのリスト.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CInput( const CInput & )				= delete;
	CInput( CInput && )						= delete;
	CInput& operator = ( const CInput & )	= delete;
	CInput& operator = ( CInput && )		= delete;

};

#endif	// #ifndef INPUT_H.