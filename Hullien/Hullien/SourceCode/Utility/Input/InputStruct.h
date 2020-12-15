#ifndef INPUT_STRUCT_H
#define INPUT_STRUCT_H

// �L�[�o�C���h.
// �Q�[�����ɕύX���Ďg�p����.
enum class enKeyBind : unsigned char
{
	None,

	Decision,		// ����.
	Cancel,			// �L�����Z��.
	Skip,			// �X�L�b�v.

	Attack,			// �U��.
	SpecialAbility,	// ����\��.
	Avoidance,		// ���.
	STGAttack,		// �V���[�e�B���O���̍U��.

	Up,			// ��.
	Down,		// ��.
	Right,		// �E.
	Left,		// ��.

	UpAxis,		// �㎲.
	RightAxis,	// �E��.

	Start,		// Xbox : Start,	DS3 : Start.
	Back,		// Xbox : Back,		DS3 : Select.

} typedef EKeyBind;

enum class enAxisBind : unsigned char
{
	None,

	L_Forward,
	L_Right,
} typedef EAxisBind;

// ���͏��.
enum enInputState : unsigned char
{
	EInputState_NotPress	= 1 << 0,	// �����Ă��Ȃ�.
	EInputState_MomentPress	= 1 << 1,	// �������u��.
	EInputState_Hold		= 1 << 2,	// ������.
	EInputState_Release		= 1 << 3,	// �������u��.
} typedef EInputState;


#endif	// #ifndef INPUT_STRUCT_H.