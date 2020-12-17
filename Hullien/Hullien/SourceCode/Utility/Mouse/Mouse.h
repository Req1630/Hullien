#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>

class CMouse
{
public:
	CMouse();
	~CMouse();
	
	// ウィンドウハンドルの設定.
	inline void SetHWND( HWND hWnd ){ m_hWnd = hWnd; }

	// マウスの更新.
	void UpdateMouse();

	// 座標の取得.
	inline POINT GetPosition()		const { return m_NowPoint; }
	// 前回の座標の取得.
	inline POINT GetOldPosition()	const { return m_OldPoint; }
	// 座標Xの取得.
	inline float GetPosisionX()		const { return static_cast<float>(m_NowPoint.x); }
	// 座標Yの取得.
	inline float GetPosisionY()		const { return static_cast<float>(m_NowPoint.y); }
	// 前回の座標Xの取得.
	inline float GetOldPosisionX()	const { return static_cast<float>(m_OldPoint.x); }
	// 前回の座標Yの取得.
	inline float GetOldPosisionY()	const { return static_cast<float>(m_OldPoint.y); }
	// スクリーンの中か.
	bool IsScreenMiddle();

private:
	HWND	m_hWnd;		// ウィンドウハンドル.
	POINT	m_NowPoint;	// 現在の座標.
	POINT	m_OldPoint;	// 前回の座標.
};

#endif	// #ifndef MOUSE_H.