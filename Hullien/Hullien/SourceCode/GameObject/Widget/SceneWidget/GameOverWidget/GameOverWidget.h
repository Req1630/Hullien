#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include "..\SceneWidget.h"

class CReturnTitle;	//�^�C�g���ɖ߂�{�^���N���X.

/********************************************
*	�Q�[���I�[�o�[UI�N���X.
**/
class CGameOverWidget : public CSceneWidget
{
public:
	CGameOverWidget();
	virtual ~CGameOverWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:
	std::unique_ptr<CReturnTitle> m_pReturnTitle;	//�^�C�g���ɖ߂�{�^���N���X.
};

#endif	//#ifndef GAMEOVERWIDGET_H.
