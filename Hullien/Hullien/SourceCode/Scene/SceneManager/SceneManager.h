#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"
#include "..\..\GameObject\Widget\Fade\Fade.h"

// �V�[���̎��.
enum class enSceneNo
{
	None,

	Title,		// �^�C�g��.
	GameMain,	// �Q�[�����C��.
	GameClear,	// �Q�[���N���A.
	GameOver,	// �Q�[���I�[�o�[.

	Edit,		// �G�f�B�b�g.

	Max,

	ResultCheck,		// �Q�[���N���A���Q�[���I�[�o�[�̊m�F.
	Start = Title,		// ���߂̃V�[��.

} typedef EScene;

/**********************************
*	�V�[���Ǘ��N���X.
*/
class CSceneManager
{	
public:
	CSceneManager();
	~CSceneManager();

	// �X�V�֐�.
	void Update();
	// ���̃V�[���Ɉړ�.
	void NextSceneMove();
	// �Q�[���I��.
	void EndGameClose();
	// �Q�[���I�[�o�[�̐ݒ�.
	void OnGameOver(){ m_IsGameOver = true; }
	// �Q�[�����g���C.
	void RetryGame();
	// HWND�ݒ�.
	void SethWnd(HWND hWnd) { m_hWnd = hWnd; }
	HWND GethWnd(){ return m_hWnd; }

	// ���g���C������.
	bool GetRetry() const { return m_IsRetry; }

private:
	// �G�f�B�b�g�V�[���ɕύX.
	void ChangeEditScene();
	
private:
	HWND m_hWnd;
	std::shared_ptr<CSceneBase> m_pScene;
	EScene	m_NowScene;		// ���݂̃V�[��.
	EScene	m_NextScene;	// ���̃V�[��.
	bool	m_IsLoadEnd;	// ���[�h���I��������.
	bool	m_IsGameOver;	// �Q�[���I�[�o�[��.
	bool	m_IsGameEnd;	// �Q�[�����I��������.
	bool	m_IsRetry;		// ���g���C������.
};

#endif // #ifndef SCENE_MANAGER_H.