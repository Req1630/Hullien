#ifndef EVENT_ACTOR_MANAGER_H
#define EVENT_ACTOR_MANAGER_H

#include "..\..\GameObject.h"

class CCollisionManager;	// �����蔻��N���X.

/****************************************
*	�C�x���g�p�A�N�^�N���X.
**/
class CEventActor : public CGameObject
{
public:
	CEventActor();
	virtual ~CEventActor();

	// �����蔻��֐�.
	virtual void Collision(CEventActor* pActor) = 0;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos(CEventActor& actor) {}

	// �����蔻��擾�֐�.
	CCollisionManager* GetCollManager() { return m_pCollManager.get(); }

protected:
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// �����蔻��N���X.
};

#endif//	#ifndef EVENT_ACTOR_MANAGER_H.