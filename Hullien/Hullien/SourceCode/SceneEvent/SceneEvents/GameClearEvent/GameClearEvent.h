#ifndef GAMECLEAR_EVENT_H
#define GAMECLEAR_EVENT_H

#include "..\..\EventBase\EventBase.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventCharacter.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"

class CGroundStage;				// �n�ʃN���X.
class CSpawnUFO;				// �C�x���g�pUFO�N���X.
class CEventPlayer;				// �C�x���g�p�v���C���[�N���X.
class CEventGirl;				// �C�x���g�p���̎q�N���X.
class CEventAlienA;				// �C�x���g�p�F���lA�N���X.
class CEventCamera;				// �C�x���g�p�J�����N���X.
class CEventManager;			// �C�x���g�Ǘ��N���X.

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
	// �A�N�^�̍X�V�֐�.
	void ActorUpdate();
	// �J�����̍X�V�֐�.
	void CameraUpdate();
	// �V�[���̐ݒ�.
	void SceneSetting();

	// ���̃V�[���ɐi�߂�.
	virtual void NextStep() override;
	// �X�L�b�v.
	virtual void Skip() override;

	// �ȉ��f�o�b�O�p.
	void DebugRender();
	void DebugOperation();

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventPlayer>			m_pPlayer;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventAlienA>			m_pAlienA;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventManager>			m_pEventManager;
	CEventCharacter::SOptionalState			m_stPlayer;			//�v���C���[�̏��.
	CEventCharacter::SOptionalState			m_stGirl;			//���̎q�̏��.
	CEventCharacter::SOptionalState			m_stAlien;			//�F���l�̏��.
	CEventCamera::SCameraState				m_stCamera;			//�J�����̏��.
	
	D3DXVECTOR3								m_vUFOPosition;		// UFO�̈ʒu.
	D3DXVECTOR3								m_vUFOScale;		// UFO�̑傫��.
	D3DXVECTOR3								m_vAlienOldPosition;// �F���l�̑O���W.
	int										m_WaitCount;		// �ҋ@�J�E���g.
	int										m_SwingCameraCount;	// �J������h�炷�J�E���g.
	float									m_Gravity;			// �d��.

	float									m_Speed;
	int										m_NowScene;			// ���݂̃V�[��/
	int										m_UFOScale;			// ���݂̃V�[��/

};


#endif	//#ifndef GAMECLEAR_EVENT_H