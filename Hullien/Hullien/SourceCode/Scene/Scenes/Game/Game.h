#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGameActorManager;	// �Q�[���I�u�W�F�N�g�Ǘ��N���X.

/**********************************
*	�Q�[���V�[���N���X.
*/
class CGame : public CSceneBase
{
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
	std::unique_ptr<CGameActorManager> m_GameObjManager;
};

#endif	// #ifndef GAME_H.