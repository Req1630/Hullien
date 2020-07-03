#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "..\Widget.h"

class CCursor;
class CReturnTitle;

/*******************************************
*	�V�[��UI���N���X.
**/
class CSceneWidget : public CWidget
{
protected:
	//	�`����.
	enum class enDrawingState
	{
		NowDrawing,	// �`�撆.
		Finish,			// �`��I��.
	} typedef EDrawingState;

public:
	CSceneWidget();
	virtual ~CSceneWidget();

protected:
	EDrawingState m_DrawingState;		// �`����.

private:
};

#endif	//#ifndef SCENEWIDGET_H.