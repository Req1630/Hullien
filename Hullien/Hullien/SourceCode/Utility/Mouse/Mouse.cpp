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

// マウスの更新.
void CMouse::UpdateMouse()
{
	// 前回を設定.
	m_OldPoint = m_NowPoint;
	// マウスカーソル位置取得.
	GetCursorPos( &m_NowPoint );
	// スクリーン座標→クライアント座標に変換.
	ScreenToClient( m_hWnd, &m_NowPoint );
}

// スクリーンの中か.
bool CMouse::IsScreenMiddle()
{
	RECT rect;
	if( GetClientRect( m_hWnd, &rect ) == FALSE ) return false;
	return (( rect.left < m_NowPoint.x && m_NowPoint.x < rect.right ) &&
			( rect.top < m_NowPoint.y && m_NowPoint.y < rect.bottom ));
}