#ifndef ACTOR_H
#define ACTOR_H

#include "..\GameObject.h"
#include <functional>

class CActor : public CGameObject
{
public:
	CActor();
	virtual ~CActor();

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&)>& ){}
	// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ){}
	// �U���́A���͎��Ԑݒ�֐�.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ){}
	// �ړ����x�A���͎��Ԑݒ�֐�.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ){}
	// ������W�̐ݒ�.
	virtual void SetOpponentPos( CActor& actor ){}

};

#endif	// #ifndef ACTOR_H.