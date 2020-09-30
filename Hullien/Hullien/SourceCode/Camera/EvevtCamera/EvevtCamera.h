#ifndef EVENT_CAMERA_H
#define EVENT_CAMERA_H

#include "..\Camera.h"

/****************************************
* イベント用カメラ.
**/
class CEventCamera : public CCamera
{
	const float INITIAL_VIEWING_ANGLE = static_cast<float>(D3DX_PI / 4.0);	// 初期視野角.

public:
	CEventCamera();
	virtual ~CEventCamera();

	// 視野角取得関数.
	float GetViewingAngle() const { return m_ViewingAngle; }
	// 視野角設定関数.
	void SetViewingAngle(const float angle) { m_ViewingAngle = angle; }
	// 視野角リセット.
	void ResetViewingAngle() { m_ViewingAngle = INITIAL_VIEWING_ANGLE; }
private:

};

#endif //#ifndef EVENT_CAMERA_H.