#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGameActorManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
class CGameWidgetManager;	// UI�Ǘ��N���X.
class CContinueWidget;		// �R���e�j���[UI�N���X.
class CConfigWidget;		// �ݒ�UI�N���X.
class CSkyDome;				// �w�i�N���X.
class CEventManager;		// �C�x���g�Ǘ��N���X.
class CTutorial;			// �`���[�g���A��UI�N���X.

/**********************************
*	�Q�[���V�[���N���X.
*/
class CGame : public CSceneBase
{
private:
	// �V�[���̏��.
	enum class enEventSceneState
	{
		None,

		GameStart,			// �Q�[���X�^�[�g.
		Game,				// �Q�[����.
		GameOver_Player,	// �v���C���[�����񂾏ꍇ.
		GameOver_Girl,		// ���̎q���A�ꋎ��ꂽ�ꍇ.
		Continue,			// �R���e�j���[.
		Clear,				// �N���A.

		Max,
	}typedef EEventSceneState;

	// �V�[���؂�ւ����.
	enum class enNextSceneState
	{
		None,

		Game,			//�Q�[��.
		Clear,			//�N���A.
		GameOver,		//�Q�[���I�[�o�[.

		Title,
	}typedef ENextSceneState;

public:
	CGame( CSceneManager* pSceneManager );
	virtual ~CGame();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// ���f���̕`��.
	void ModelRender();
	// �Q�[�������֐�.
	void GameUpdate();
	// �R���e�j���[�����֐�.
	void ContinueUpdate();
	// �ݒ��ʂ̍X�V.
	void ConfigUpdate();
	// �V�[���؂�ւ��֐�.
	void ChangeEventScene();
	// ���̃V�[���Ɉڍs.
	void NextSceneMove();
	// ���̃V�[����ݒ�.
	void SetNextScene(EEventSceneState state, bool GameOver = false);
	// BGM�̒�~.
	void StopBGM(const char* name);
	// �S�Ă�BGM��~.
	void StopAllBGM();

private:
	std::unique_ptr<CGameActorManager>	m_pGameObjManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
	std::unique_ptr<CGameWidgetManager>	m_pWidgetManager; 	// �Q�[��UI�Ǘ��N���X.
	std::unique_ptr<CContinueWidget>	m_pContinueWidget;	// �R���e�j���[UI�N���X.
	std::unique_ptr<CEventManager>		m_pEventManager; 	// �C�x���g�Ǘ��N���X.
	std::unique_ptr<CTutorial>			m_pTutorial;		// �`���[�g���A���N���X.
	EEventSceneState					m_NowEventScene;	// ���݂̃C�x���g�V�[��.
	ENextSceneState						m_NextSceneState;	// ���̃V�[�����.
	float								m_WaitCount;		// �ҋ@�J�E���g.
	bool								m_IsContinueSelect;	// �R���e�j���[��I�����Ă��邩.
	bool								m_IsConfig;			// �ݒ蒆��.
	bool								m_IsPlayGameBGM;	// �Q�[����BGM���Đ����Ă��邩.
	bool								m_IsPlayDangerBGM;	// �댯BGM���Đ����Ă��邩.
};

#endif	// #ifndef GAME_H.