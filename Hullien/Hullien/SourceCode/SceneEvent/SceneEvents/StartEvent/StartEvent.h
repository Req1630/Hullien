#ifndef START_EVENT_H
#define START_EVENT_H

#include "..\..\EventBase\EventBase.h"

/***********************************
*	�X�^�[�g�C�x���g�N���X.
**/
class CStartEvent : public CEventBase
{
public:
	CStartEvent();
	virtual ~CStartEvent();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
};

#endif //#ifndef START_EVENT_H.