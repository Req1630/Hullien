#ifndef COMMAND_H
#define COMMAND_H

#include <queue>
#include <vector>

class CCommand
{
	// �L�[(button)�̃��X�g.
	enum enKey : unsigned char
	{
		EKey_Up,	// ��.
		EKey_Down,	// ��.
		EKey_Right,	// �E.
		EKey_Left,	// ��.
		EKey_A,		// A.
		EKey_B,		// B.
		EKey_X,		// X.
		EKey_Y,		// Y.
		EKey_RB,	// RB.
		EKey_LB,	// LB.
	} typedef EKey;
	
 	const std::vector<EKey> COMMAND_LIST = 
	{
		EKey_Up,
		EKey_Up,
		EKey_Down,
		EKey_Down,
		EKey_Left,
		EKey_Right,
	};

public:
	CCommand();
	~CCommand();

	void Update();

	// ����������.
	inline bool IsSuccess() { return m_isSuccessCommand; }

private:
	// �L�[(button�̓���).
	void InputKey();
	// �L�[�̒ǉ�.
	void PushKey( const EKey& key );
	// �L���[�̊m�F.
	void Check();

private:
	std::queue<EKey>	m_KeyQueue;			// ���͂��ꂽ�L�[�̃L���[.
	int					m_TimeCount;		// ���͂���Ă���̃^�C���J�E���g.
	bool				m_isSuccessCommand;	// �R�}���h������������.
};

#endif	// #ifndef COMMAND_H.