#ifndef GAME_CLEAR_H
#define GAME_CLEAR_H

#include "..\..\SceneBase\SceneBase.h"

/**********************************
*	�Q�[���N���A�N���X.
*/
class CGameClear : public CSceneBase
{
public:
	CGameClear( CSceneManager* pSceneManager );
	virtual ~CGameClear();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
};

#endif	// #ifndef GAME_CLEAR_H.