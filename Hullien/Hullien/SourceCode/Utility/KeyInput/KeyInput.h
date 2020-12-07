#ifndef KEY_INPUT_H
#define KEY_INPUT_H

/*******************************
* キー入力を取得するクラス.
**/
class CKeyInput
{
	static const int KEY_MAX = 256;	// キーの最大値.

public:
	// キーの状態.
	enum enKeyState : unsigned char
	{
		EKeyState_NotPress	= 0,		// 何も押していない.
		EKeyState_Press		= 1 << 0,	// 押した瞬間.
		EKeyState_Hold		= 1 << 1,	// 押し続けている.
		EKeyState_Releae	= 1 << 2,	// 離した瞬間.
	} typedef EKeyState;
	typedef char KeyState;

public:
	CKeyInput();
	~CKeyInput();

	// キーの状態の更新.
	static void Update();

	// キーの状態の取得.
	static KeyState GetState( const unsigned char& key );

private:
	// インスタンスの取得.
	static CKeyInput* GetInstance();

	// キーの状態の更新.
	void KeyStateUpdate();

private:
	unsigned char m_InputState[256];	// 現在のキーの状態.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CKeyInput( const CKeyInput & )				= delete;
	CKeyInput( CKeyInput && )					= delete;
	CKeyInput& operator = ( const CKeyInput & )	= delete;
	CKeyInput& operator = ( CKeyInput && )		= delete;
};

#endif	// #ifndef KEY_INPUT_H.