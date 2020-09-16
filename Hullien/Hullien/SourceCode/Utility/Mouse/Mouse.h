#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>

class CMouse
{
public:
	CMouse();
	~CMouse();

	// �}�E�X�̍X�V.
	void UpdateMouse( HWND hWnd );

	// ���WX�̎擾.
	float GetPosisionX();
	// ���W�x�̎擾.
	float GetPosisionY();
	// �O��̍��WX�̎擾.
	float GetOldPosisionX();
	// �O��̍��W�x�̎擾.
	float GetOldPosisionY();
	// �X�N���[���̒���.
	bool IsScreenMiddle();

private:
	POINT	m_Point;
	POINT	m_OldPoint;
};

#endif	// #ifndef MOUSE_H.