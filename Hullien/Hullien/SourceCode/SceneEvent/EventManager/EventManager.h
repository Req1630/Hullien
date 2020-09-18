#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "..\EventBase\EventBase.h"

// �C�x���g�̎��.
enum class enEventNo
{
	None,

	GameStart,				//�Q�[���X�^�[�g.
	GameClear,				//�Q�[���N���A.
	GameOver,				//�Q�[���I�[�o�[.

	Max,

	Start = GameStart,	//���߂̃V�[��.
}typedef EEvent;

/************************************
*	�C�x���g�V�[���Ǘ��N���X.
**/
class CEventManager
{
public:
	CEventManager();
	~CEventManager();

	// �X�V�֐�.
	void Update();
	// ���̃C�x���g�Ɉړ�.
	void NextEventMove();
	// �C�x���g�̏I�����擾.
	bool GetIsEventEnd() const { return m_IsEventEnd; };
	// �X�L�b�v�̐ݒ�.
	void OnSkip() { m_IsSkip = true; }
	// �Q�[���I�[�o�[�̐ݒ�.
	void OnGameOver() { m_IsGameOver = true; }


private:
	std::shared_ptr<CEventBase> m_pEventBase;	// �C�x���g���N���X.
	EEvent								 m_NowEvent;		// ���݂̃C�x���g.	
	EEvent								 m_NextEvent;		// ���̃C�x���g.
	bool									 m_IsLoadEnd;		// ���[�h���I��������.
	bool									 m_IsSkip;			// �X�L�b�v���邩.
	bool									 m_IsGameOver;	// �Q�[���I�[�o�[��.
	bool									 m_IsEventEnd;	// �C�x���g���I��������.
};

#endif	//#ifndef SCENE_EVENT_MANAGER_H