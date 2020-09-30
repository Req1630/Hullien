#ifndef EVENT_CAMERA_H
#define EVENT_CAMERA_H

#include "..\Camera.h"

/****************************************
* �C�x���g�p�J����.
**/
class CEventCamera : public CCamera
{
	const float INITIAL_VIEWING_ANGLE = static_cast<float>(D3DX_PI / 4.0);	// ��������p.

public:
	CEventCamera();
	virtual ~CEventCamera();

	// ����p�擾�֐�.
	float GetViewingAngle() const { return m_ViewingAngle; }
	// ����p�ݒ�֐�.
	void SetViewingAngle(const float angle) { m_ViewingAngle = angle; }
	// ����p���Z�b�g.
	void ResetViewingAngle() { m_ViewingAngle = INITIAL_VIEWING_ANGLE; }
private:

};

#endif //#ifndef EVENT_CAMERA_H.