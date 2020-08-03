#ifndef CLEARWIDGET_H
#define CLEARWIDGET_H

#include "..\SceneWidget.h"

class CReturnTitle;

/***********************************************
*	�N���AUI�N���X.
**/
class CClearWidget : public CSceneWidget
{
public:
	CClearWidget();
	virtual ~CClearWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:
	std::unique_ptr<CReturnTitle> m_pReturTitle;	//�^�C�g���ɖ߂�{�^���N���X.
};

#endif	//#ifndef CLEARWIDGET_H.
