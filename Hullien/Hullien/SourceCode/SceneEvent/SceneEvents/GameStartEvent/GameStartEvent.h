#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"

class CGroundStage;		// �n�ʃN���X.
class CEventUFO;

/***********************************
*	�X�^�[�g�C�x���g�N���X.
**/
class CGameStartEvent : public CEventBase
{
public:
	CGameStartEvent();
	virtual ~CGameStartEvent();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	void DebugRender();

private:
	std::shared_ptr<CGroundStage>		m_pGroundStage;
	std::shared_ptr<CEventUFO>			m_pUFO;
};

#endif //#ifndef START_EVENT_H.