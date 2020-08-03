#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "..\..\SceneBase\SceneBase.h"

class CGameOverWidget;

/**********************************
*	�Q�[���I�[�o�[�N���X.
*/
class CGameOver : public CSceneBase
{
public:
	CGameOver( CSceneManager* pSceneManager );
	virtual ~CGameOver();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	std::unique_ptr<CGameOverWidget> m_pGameOverWidget;
};

#endif	// #ifndef GAME_OVER_H.