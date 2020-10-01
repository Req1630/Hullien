#ifndef GAMESTART_EVENT_H
#define GAMESTART_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"

class CGroundStage;		// �n�ʃN���X.
class CSpawnUFO;		// �C�x���g�pUFO�N���X.
class CEventPlayer;		// �C�x���g�p�v���C���[�N���X.
class CEventGirl;		// �C�x���g�p���̎q�N���X.
class CEventAlienA;		// �C�x���g�p�F���lA�N���X.
class CBarrier;			// �o���A�N���X.
class CMotherShipUFO;	// �}�U�[�V�b�v�N���X.
class CEventCamera;		// �C�x���g�p�J�����N���X.
class CEventManager;	// �C�x���g�Ǘ��N���X.

/***********************************
*	�X�^�[�g�C�x���g�N���X.
**/
class CGameStartEvent : public CEventBase
{
	// �C�x���g�̊e�X�e�b�v.
	enum class enEventStep
	{
		Escape_PlayerAndGirl = 0,	// ������v���C���[�Ə��̎q.
		Viewpoint_UFO,				// UFO�̎��_.
		Move_UFO,					// UFO��ʒu�܂ňړ�.
		Stop_UFO,					// UFO��~.
		Appearance_Alien,			// �F���l�o��.
		Move_Alien,					// �F���l�O�i.
		GetCaught_Girl,				// ���̎q���߂܂�.
		InvocatingOrder_Barrier,	// �o���A��������.
		Invocating_Barrier,			// �o���A����.
		Return_Girl,				// ���̎q�A��.
		GameStart,					// �Q�[���J�n.

		Max,
		EventStart = Escape_PlayerAndGirl,
	} typedef EEventStep;

private:


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
	// �J�����������֐�.
	bool CameraInit();
	// UFO�������֐�.
	bool SpawnUFOInit();
	// �v���C���[�������֐�.
	bool PlayerInit();
	// ���̎q�������֐�.
	bool GirlInit();
	// �F���l�������֐�.
	bool AlienInit();
	// �}�U�[�V�b�v�������֐�.
	bool MotherShipUFOInit();
	// �A�N�^�̍X�V�֐�.
	void ActorUpdate();
	// �J�����̍X�V�֐�.
	void CameraUpdate();
	// �V�[���̐ݒ�.
	void SceneSetting();
	// ���̃V�[���ɐi�߂�.
	void NextStep();

	// �ȉ��C�x���g�̃X�e�b�v.
	void EscapePlayerAndGirl();
	void ViewpointUFO();
	void MoveUFO();
	void StopUFO();
	void AppearanceAlien();
	void MoveAlien();
	void GetCaughtGirl();
	void InvocatingOrderBarrier();
	void InvocatingBarrier();
	void ReturnGirl();
	void GameStart();

	// �ȉ��f�o�b�O�p.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>		m_pGroundStage;
	std::shared_ptr<CSpawnUFO>			m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>		m_pPlayer;
	std::shared_ptr<CEventGirl>			m_pGirl;
	std::shared_ptr<CEventAlienA>		m_pAlienA;
	std::shared_ptr<CBarrier>			m_pBarrier;
	std::shared_ptr<CMotherShipUFO>		m_pMotherShipUFO;
	std::shared_ptr<CEventCamera>		m_pEventCamera;
	std::shared_ptr<CEventManager>		m_pEventManager;
	D3DXVECTOR3							m_vCameraPosition;
	D3DXVECTOR3							m_vCameraRotation;
	D3DXVECTOR3							m_vLookPosition;
	D3DXVECTOR3							m_vUFOPosition;
	EEventStep							m_EventStep;
	int									m_NowStep;
	float								m_Speed;
	float								m_DecelerationZ;	// z���W�����x.
	float								m_Count;			// �J�E���g.
	bool								m_IsDisp;

	CEventCharacter::SOptionalState		m_stPlayer;
	CEventCharacter::SOptionalState		m_stGirl;
	CEventCharacter::SOptionalState		m_stAlien;
};

#endif //#ifndef START_EVENT_H.