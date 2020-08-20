#ifndef CONTINUEWIDGET_H
#define CONTINUEWIDGET_H

#include "..\SceneWidget.h"
#include <vector>

/********************************************
*	�R���e�j���[UI�N���X.
**/
class CContinueWidget : public CSceneWidget
{
public:
	const char* SPRITE_BACK				= "CBackGround";	//�w�i
	const char* SPRITE_CONTINUE		= "continuesize";		//�R���e�j���[.
	const char* SPRITE_YES					= "yessize";			//�C�G�X
	const char* SPRITE_NO					= "nosize";				//�m�[.
	const int BACKGROUND					= 0;						//�w�i�z��ԍ�.
	const int YES									= 1;						//�C�G�X�z��ԍ�.
	const int NO									= 2;						//�m�[�z��ԍ�.
	const float BACKGROUND_ALPHA		= 0.5f;					//�w�i���ߒl.

	//�I�����.
	enum class enSelectState
	{
		Yes,		//�C�G�X.
		No,		//�m�[..
	} typedef ESelectState;

public:
	CContinueWidget();
	virtual ~CContinueWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �I����Ԃ̎擾�֐�.
	ESelectState GetSelectState() const { return m_SelectState; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �J�[�\���ݒ�֐�.
	void CursorSetting();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprite;			//�X�v���C�g�N���X.
	std::shared_ptr<CCursor>					m_pCursor;			//�J�[�\���N���X.
	ESelectState									m_SelectState;	//�I�����.
};

#endif	//#ifndef CONTINUEWIDGET_H
