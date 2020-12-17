#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>

class CMouse
{
public:
	CMouse();
	~CMouse();
	
	// �E�B���h�E�n���h���̐ݒ�.
	inline void SetHWND( HWND hWnd ){ m_hWnd = hWnd; }

	// �}�E�X�̍X�V.
	void UpdateMouse();

	// ���W�̎擾.
	inline POINT GetPosition()		const { return m_NowPoint; }
	// �O��̍��W�̎擾.
	inline POINT GetOldPosition()	const { return m_OldPoint; }
	// ���WX�̎擾.
	inline float GetPosisionX()		const { return static_cast<float>(m_NowPoint.x); }
	// ���WY�̎擾.
	inline float GetPosisionY()		const { return static_cast<float>(m_NowPoint.y); }
	// �O��̍��WX�̎擾.
	inline float GetOldPosisionX()	const { return static_cast<float>(m_OldPoint.x); }
	// �O��̍��WY�̎擾.
	inline float GetOldPosisionY()	const { return static_cast<float>(m_OldPoint.y); }
	// �X�N���[���̒���.
	bool IsScreenMiddle();

private:
	HWND	m_hWnd;		// �E�B���h�E�n���h��.
	POINT	m_NowPoint;	// ���݂̍��W.
	POINT	m_OldPoint;	// �O��̍��W.
};

#endif	// #ifndef MOUSE_H.