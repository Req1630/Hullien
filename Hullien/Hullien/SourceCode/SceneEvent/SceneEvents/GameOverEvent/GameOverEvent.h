#ifndef GAMEOVER_EVENT_H
#define GAMEOVER_EVENT_H

#include "..\..\EventBase\EventBase.h"

/************************************
*	�Q�[���I�[�o�[�C�x���g�N���X.
**/
class CGameOverEvent : public CEventBase
{
public:
	CGameOverEvent();
	virtual ~CGameOverEvent();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �X�v���C�g�`��֐�.
	virtual void SpriteRender() override;

private:
	// ���̃V�[���ɐi�߂�.
	virtual void NextStep() override;
	// �X�L�b�v.
	virtual void Skip() override;

};

#endif	//#ifndef GAMEOVER_EVENT_H.