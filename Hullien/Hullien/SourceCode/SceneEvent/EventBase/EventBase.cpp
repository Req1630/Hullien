#include "EventBase.h"

/***********************************
*	�C�x���g�V�[�����N���X.
**/
CEventBase::CEventBase()
	: m_IsEventEnd	( false )
	, m_IsSkip		( false )
{
}

CEventBase::~CEventBase()
{
}

// �ړI�n�ւ̂̈ړ�.
bool CEventBase::MoveDestination(D3DXVECTOR3& vMyPosition, const D3DXVECTOR3 & vDestination, const float & speed)
{
	// �ړ������̎Z�o.
	float Distance = sqrtf((vMyPosition.x - vDestination.x) * (vMyPosition.x - vDestination.x)
		+ (vMyPosition.y - vDestination.y) * (vMyPosition.y - vDestination.y)
		+ (vMyPosition.z - vDestination.z) * (vMyPosition.z - vDestination.z));

	// �ړ��񐔂̎Z�o.
	float MoveCount = Distance / speed;
	if (MoveCount < 1) return true;

	// �ړ��ʂ����W�ɑ���.
	vMyPosition.x += (vDestination.x - vMyPosition.x) / MoveCount;
	vMyPosition.y += (vDestination.y - vMyPosition.y) / MoveCount;
	vMyPosition.z += (vDestination.z - vMyPosition.z) / MoveCount;

	if (MoveCount >= 1) return false;
	return true;
}
