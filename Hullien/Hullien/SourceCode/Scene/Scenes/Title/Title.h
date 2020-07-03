#ifndef TITLE_H
#define TITLE_H

#include "..\..\SceneBase\SceneBase.h"

#include "..\..\..\Common\Sprite\CSprite.h"

#include "..\..\..\Collider\Colliders\Capsule\CapsuleModel.h"
#include "..\..\..\Collider\Colliders\Capsule\CapsuleCollision.h"

class CRotLookAtCenter;
class CTitleWidget;

/**********************************
*	�^�C�g���V�[���N���X.
*/
class CTitle : public CSceneBase
{
public:
	CTitle(CSceneManager* pSceneManager);
	virtual ~CTitle();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	std::unique_ptr<CTitleWidget>	m_pWidget;		//UI�N���X.

};

#endif	// #ifndef TITLE_H.