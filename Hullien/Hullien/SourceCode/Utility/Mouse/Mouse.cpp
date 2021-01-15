#include "Mouse.h"
#include "..\..\Global.h"

CMouse::CMouse()
	: m_hWnd		( nullptr )
	, m_NowPoint	{ 0, 0 }
	, m_OldPoint	{ 0, 0 }
{
}

CMouse::~CMouse()
{
}

// �}�E�X�̍X�V.
void CMouse::UpdateMouse()
{
	// �O���ݒ�.
	m_OldPoint = m_NowPoint;
	// �}�E�X�J�[�\���ʒu�擾.
	GetCursorPos( &m_NowPoint );
	// �X�N���[�����W���N���C�A���g���W�ɕϊ�.
	ScreenToClient( m_hWnd, &m_NowPoint );
}

// �X�N���[���̒���.
bool CMouse::IsScreenMiddle()
{
	RECT rect;
	if( GetClientRect( m_hWnd, &rect ) == FALSE ) return false;
	return (( rect.left < m_NowPoint.x && m_NowPoint.x < rect.right ) &&
			( rect.top < m_NowPoint.y && m_NowPoint.y < rect.bottom ));
}