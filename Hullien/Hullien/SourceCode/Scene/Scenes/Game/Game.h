#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGameActorManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
class CGameWidgetManager;	// UI�Ǘ��N���X.
class CContinueWidget;		// �R���e�j���[UI�N���X.
class CSkyDome;					// �w�i�N���X.
class CEventManager;			//�C�x���g�Ǘ��N���X.
class CPeraRenderer;	// G-Buufer�`��p.

/**********************************
*	�Q�[���V�[���N���X.
*/
class CGame : public CSceneBase
{
private:
	// �V�[���̏��.
	enum class enSceneState
	{
		None,

		GameStart,			// �Q�[���X�^�[�g.
		Game,				// �Q�[����.
		GameOver_Player,	// �v���C���[�����񂾏ꍇ.
		GameOver_Girl,		// ���̎q���A�ꋎ��ꂽ�ꍇ.
		Continue,			// �R���e�j���[.
		Clear,				// �N���A.

		Max,
	}typedef ESceneState;

	// �V�[���؂�ւ����.
	enum class enNextSceneState
	{
		None,

		Game,			//�Q�[��.
		Clear,			//�N���A.
		GameOver,		//�Q�[���I�[�o�[.
	}typedef ENextSceneState;

public:
	CGame(CSceneManager* pSceneManager);
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
	// �t�F�[�h�������֐�.
	bool InitFade();
	// �Q�[�������֐�.
	void UpdateGame();
	// �R���e�j���[�����֐�.
	void UpdateContinue();
	// �V�[���؂�ւ��֐�.
	void ChangeScene();
	// ���̃V�[���Ɉڍs.
	void NextSceneMove();

private:
	std::unique_ptr<CGameActorManager>		m_GameObjManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
	std::unique_ptr<CGameWidgetManager>		m_WidgetManager;	// �Q�[��UI�Ǘ��N���X.
	std::unique_ptr<CContinueWidget>		m_ContinueWidget;	// �R���e�j���[UI�N���X.
	std::unique_ptr<CSkyDome>				m_pSkyDome;			// �w�i.
	std::unique_ptr<CEventManager>			m_pEventManager;	// �C�x���g�Ǘ��N���X.
	std::unique_ptr<CPeraRenderer>			m_pPeraRenderer;
	ESceneState								m_NowScene;			// ���݂̃V�[��.
	ENextSceneState							m_NextSceneState;	// ���̃V�[�����.
	bool									m_IsChangeScene;	// �V�[���؂�ւ����\��.

};

#endif	// #ifndef GAME_H.