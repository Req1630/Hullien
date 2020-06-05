#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGroundStage;	// �n�ʃN���X.

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
	std::shared_ptr<CGroundStage>	m_pGroundStage;
};

#endif	// #ifndef GAME_H.