#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "..\..\SceneBase\SceneBase.h"

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
};

#endif	// #ifndef GAME_OVER_H.