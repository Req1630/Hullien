#ifndef CLEAR_EVENT_H
#define CLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"

/************************************
*	�N���A�C�x���g�N���X.
**/
class CClearEvent : public CEventBase
{
public:
	CClearEvent();
	virtual ~CClearEvent();

	// �ǂݍ��݊֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
};


#endif	//#ifndef CLEAR_EVENT_H