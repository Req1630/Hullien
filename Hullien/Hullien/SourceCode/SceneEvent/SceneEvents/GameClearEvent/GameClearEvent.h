#ifndef GAMECLEAR_EVENT_H
#define GAMECLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"

/************************************
*	�N���A�C�x���g�N���X.
**/
class CGameClearEvent : public CEventBase
{
public:
	CGameClearEvent();
	virtual ~CGameClearEvent();

	// �ǂݍ��݊֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �X�v���C�g�`��֐�.
	virtual void SpriteRender() override;

private:
};


#endif	//#ifndef GAMECLEAR_EVENT_H