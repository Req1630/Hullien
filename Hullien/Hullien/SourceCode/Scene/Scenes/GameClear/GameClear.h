#ifndef GAME_CLEAR_H
#define GAME_CLEAR_H

#include "..\..\SceneBase\SceneBase.h"

class CClearWidget;	// �N���AUI�N���X.

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

private:
	// �V�[���؂�ւ��֐�.
	void ChangeScene();

private:
	std::unique_ptr<CClearWidget> m_pClearWidget;	// �N���AUI�N���X.
};

#endif	// #ifndef GAME_CLEAR_H.