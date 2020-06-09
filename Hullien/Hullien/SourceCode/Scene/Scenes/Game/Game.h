#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"

class CGroundStage;	// �n�ʃN���X.
class CPlayer;		// �v���C���[�N���X.
class CAlienA;

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
	std::shared_ptr<CPlayer>		m_pPlayer;
	std::shared_ptr<CAlienA>		m_pEnemy;
};

#endif	// #ifndef GAME_H.