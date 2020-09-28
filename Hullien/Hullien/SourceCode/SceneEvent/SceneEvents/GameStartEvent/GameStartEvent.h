#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"

class CGroundStage;		// �n�ʃN���X.
class CSpawnUFO;			// �C�x���g�pUFO�N���X.
class CEventPlayer;			// �C�x���g�p�v���C���[�N���X.
class CEventGirl;				// �C�x���g�p���̎q�N���X.
class CEventAlienA;			// �C�x���g�p�F���lA�N���X.
class CBarrier;				// �o���A�N���X.
class CMotherShipUFO;	// �}�U�[�V�b�v�N���X.
class CEventCamera;		// �C�x���g�p�J�����N���X.
class CEventManager;		// �C�x���g�Ǘ��N���X.

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
	// �L�����N�^�̍X�V�֐�.
	void CharacterUpdate();
	// �J�����̍X�V�֐�.
	void CameraUpdate();


	// �ȉ��f�o�b�O�p.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>		m_pGroundStage;
	std::shared_ptr<CSpawnUFO>		m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>		m_pPlayer;
	std::shared_ptr<CEventGirl>			m_pGirl;
	std::shared_ptr<CEventAlienA>		m_pAlienA;
	std::shared_ptr<CBarrier>				m_pBarrier;
	std::shared_ptr<CMotherShipUFO>	m_pMotherShipUFO;
	std::shared_ptr<CEventCamera>		m_pEventCamera;
	std::shared_ptr<CEventManager>	m_pEventManager;
	D3DXVECTOR3							m_vPosition;
	D3DXVECTOR3							m_vRotation;
	D3DXVECTOR3							m_vLookPosition;
	D3DXVECTOR3							m_vUFOPosition;
	int												m_NowStep;
	float											m_Speed;
	bool											m_IsDisp;
};

#endif //#ifndef START_EVENT_H.