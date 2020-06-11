#ifndef ACTOR_H
#define ACTOR_H

#include "..\GameObject.h"
#include <functional>

class CCollisionManager;	// �����蔻��N���X.

class CActor : public CGameObject
{
public:
	CActor();
	virtual ~CActor();

	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) = 0;

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&)>& ){}
	// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ){}
	// �U���́A���͎��Ԑݒ�֐�.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ){}
	// �ړ����x�A���͎��Ԑݒ�֐�.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ){}
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& actor ){}

	// �����蔻��擾�֐�.
	CCollisionManager* GetCollManager(){ return m_pCollManager.get(); }

protected:
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// �����蔻��N���X.
};

#endif	// #ifndef ACTOR_H.