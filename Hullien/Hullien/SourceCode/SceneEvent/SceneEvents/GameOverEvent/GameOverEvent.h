#ifndef GAMEOVER_EVENT_H
#define GAMEOVER_EVENT_H

#include "..\..\EventBase\EventBase.h"

class CGroundStage;				// �n�ʃN���X.
class CSpawnUFO;				// �C�x���g�pUFO�N���X.
class CEventGirl;				// �C�x���g�p���̎q�N���X.
class CEventCamera;				// �C�x���g�p�J�����N���X.
class CEventManager;			// �C�x���g�Ǘ��N���X.
class CEventWidget;				// �C�x���g��UI�N���X.

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

private:
	std::shared_ptr<CGroundStage>			m_pGroundStage;
	std::shared_ptr<CSpawnUFO>				m_pSpawnUFO;
	std::shared_ptr<CEventGirl>				m_pGirl;
	std::shared_ptr<CEventCamera>			m_pEventCamera;
	std::shared_ptr<CEventManager>			m_pEventManager;
	std::shared_ptr<CEventWidget>			m_pEventWidget;
	CEventCharacter::SOptionalState			m_stGirl;			//���̎q�̏��.
	CEventCamera::SCameraState				m_stCamera;			//�J�����̏��.
	D3DXVECTOR3								m_vUFOPosition;		// UFO�̈ʒu.
	D3DXVECTOR3								m_vUFOScale;		// UFO�̑傫��.

};

#endif	//#ifndef GAMEOVER_EVENT_H.