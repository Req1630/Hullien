#include "Mouse.h"
#include "..\..\Global.h"

CMouse::CMouse()
	: m_Point		{ 0, 0 }
	, m_OldPoint	{ 0, 0 }
{
}

CMouse::~CMouse()
{
}

// �}�E�X�̍X�V.
void CMouse::UpdateMouse( HWND hWnd )
{
	m_OldPoint = m_Point;
	// �}�E�X�J�[�\���ʒu�擾.
	GetCursorPos( &m_Point );
	// �X�N���[�����W���N���C�A���g���W�ɕϊ�.
	ScreenToClient( hWnd, &m_Point );
}

// ���WX�̎擾.
float CMouse::GetPosisionX()
{
	return static_cast<float>(m_Point.x);
}

// ���W�x�̎擾.
float CMouse::GetPosisionY()
{
	return static_cast<float>(m_Point.y);
}

// �O��̍��WX�̎擾.
float CMouse::GetOldPosisionX()
{
	return static_cast<float>(m_OldPoint.x);
}

// �O��̍��W�x�̎擾.
float CMouse::GetOldPosisionY()
{
	return static_cast<float>(m_OldPoint.y);
}

// �X�N���[���̒���.
bool CMouse::IsScreenMiddle()
{
	return (( 0 < m_Point.x && m_Point.x < WND_W ) &&
			( 0 < m_Point.y && m_Point.y < WND_H ));
}