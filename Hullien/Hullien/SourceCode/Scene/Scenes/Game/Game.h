#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGameActorManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
class CGameWidgetManager;	// UI�Ǘ��N���X.
class CContinueWidget;		// �R���e�j���[UI�N���X.
class CSkyDome;					// �w�i�N���X.
class CEventManager;			//�C�x���g�Ǘ��N���X.


/**********************************
*	�Q�[���V�[���N���X.
*/
class CGame : public CSceneBase
{
private:
	// �V�[���؂�ւ����.
	enum class enChangeSceneState
	{
		None, 

		Game,			//�Q�[��.
		Clear,				//�N���A.
		GameOver,		//�Q�[���I�[�o�[.
	}typedef EChangeSceneState;

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
	// �t�F�[�h�������֐�.
	bool InitFade();	
	// �R���e�j���[�����֐�.
	void UpdateContinue();
	// �V�[���؂�ւ��֐�.
	void ChangeScene();
	// �V�[���̑I��.
	void SelectScene();
	// �V�[���؂�ւ��ݒ�֐�.
	void SetChangeScene( const EChangeSceneState& changeState );


private:
	std::unique_ptr<CGameActorManager>		m_GameObjManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.
	std::unique_ptr<CGameWidgetManager>	m_WidgetManager;		// �Q�[��UI�Ǘ��N���X.
	std::unique_ptr<CContinueWidget>			m_ContinueWidget;		// �R���e�j���[UI�N���X.
	std::unique_ptr<CSkyDome>					m_pSkyDome;				// �w�i.
	std::unique_ptr<CEventManager>			m_pEventManager;		//	�C�x���g�Ǘ��N���X.
	EChangeSceneState							m_ChangeSceneState;	// �V�[���؂�ւ����.
	bool													m_IsChangeScene;		// �V�[���؂�ւ����\��.

};

#endif	// #ifndef GAME_H.