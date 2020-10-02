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
	// �J�����̃p�����[�^.
	struct stCameraState
	{
		D3DXVECTOR3 vPosition;		// �ʒu.
		D3DXVECTOR3 vRotation;		// ��]�l.
		D3DXVECTOR3 vLookPosition;	// �����ʒu.
		D3DXVECTOR3 vLenght;		// �^�[�Q�b�g�ւ̒���.
		float		ViewingAngle;	// ����p.
		float		MoveSpeed;		// �ړ����x.

		stCameraState()
			: vPosition			({0.0f,0.0f,0.0f})
			, vRotation			({0.0f,0.0f,0.0f})
			, vLookPosition		({0.0f,0.0f,0.0f})
			, vLenght			({10.0f,0.0f,7.0f})
			, ViewingAngle		(static_cast<float>(D3DX_PI / 4.0))
			, MoveSpeed			(0.0f)
		{}

	}typedef SCameraState;

public:
	CEventCamera();
	virtual ~CEventCamera();

	// ����p�擾�֐�.
	float GetViewingAngle() const { return m_ViewingAngle; }
	// ����p���Z�b�g.
	float ResetViewingAngle() { return INITIAL_VIEWING_ANGLE; }
	// �ړI�n�Ɉړ�.
	D3DXVECTOR3 MoveToTargetPosition(const D3DXVECTOR3& vTargetPos, const float& speed);
	// �I�u�W�F�N�g�𒆐S�ɉ�].
	void RotationLookAtObject(const D3DXVECTOR3& vLookPos, const float& Degree);
	// �J�����̏��ݒ�.
	void SetState(const SCameraState& param);

private:
	SCameraState m_Parameter;

};

#endif //#ifndef EVENT_CAMERA_H.