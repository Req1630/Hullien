#include "EvevtCamera.h"

CEventCamera::CEventCamera()
{
}

CEventCamera::~CEventCamera()
{
}

// �ړI�n�Ɉړ�.
D3DXVECTOR3 CEventCamera::MoveToTargetPosition(const D3DXVECTOR3& vTargetPos, const float& speed)
{
	float lenght = D3DXVec3Length(&D3DXVECTOR3(vTargetPos - m_vPosition));
	if (lenght < 1.0f) return vTargetPos;


	float x, z;
	x = vTargetPos.x - m_vPosition.x;
	z = vTargetPos.z - m_vPosition.z;

	float Radian = atan2f(x, z);

	m_vPosition.x += cosf(Radian) * speed;
	m_vPosition.z += sinf(Radian) * speed;

	return m_vPosition;
}

// �I�u�W�F�N�g�𒆐S�ɉ�].
void CEventCamera::RotationLookAtObject(const D3DXVECTOR3 & vLookPos, const float& Degree)
{
	// �����ʒu���擾.
	m_vLookPosition = vLookPos;

	// �J�����ʒu���Z�o.
	m_vPosition.x = m_vLookPosition.x + (sinf(Degree) * 3.0f);
	//	m_vPosition.y = m_vLookPosition.y + ( sinf(m_vDegree.y) * m_Length );
	m_vPosition.z = m_vLookPosition.z + (cosf(Degree) * 3.0f);
}

// �J�����̃p�����[�^�ݒ�.
void CEventCamera::SetState(const SCameraState & param)
{
	m_vPosition = param.vPosition;
	m_vLookPosition = param.vLookPosition;
	m_ViewingAngle = param.ViewingAngle;
}
