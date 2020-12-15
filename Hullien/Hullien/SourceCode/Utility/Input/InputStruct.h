#ifndef INPUT_STRUCT_H
#define INPUT_STRUCT_H

// キーバインド.
// ゲーム毎に変更して使用する.
enum class enKeyBind : unsigned char
{
	None,

	Decision,		// 決定.
	Cancel,			// キャンセル.
	Skip,			// スキップ.

	Attack,			// 攻撃.
	SpecialAbility,	// 特殊能力.
	Avoidance,		// 回避.
	STGAttack,		// シューティング時の攻撃.

	Up,			// 上.
	Down,		// 下.
	Right,		// 右.
	Left,		// 左.

	UpAxis,		// 上軸.
	RightAxis,	// 右軸.

	Start,		// Xbox : Start,	DS3 : Start.
	Back,		// Xbox : Back,		DS3 : Select.

} typedef EKeyBind;

// 入力状態.
enum enInputState : unsigned char
{
	EInputState_NotPress	= 1 << 0,	// 押していない.
	EInputState_MomentPress	= 1 << 1,	// 押した瞬間.
	EInputState_Hold		= 1 << 2,	// 長押し.
	EInputState_Release		= 1 << 3,	// 離した瞬間.
} typedef EInputState;


#endif	// #ifndef INPUT_STRUCT_H.