#ifndef CLEARWIDGET_H
#define CLEARWIDGET_H

#include "..\SceneWidget.h"

class CReturnTitle;

/***********************************************
*	�N���AUI�N���X.
**/
class CClearWidget : public CSceneWidget
{
	const float SPRITE_DIDSPPEAR_TIME = 30.0f;
public:
	CClearWidget();
	virtual ~CClearWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

	inline bool IsSpriteRenderEnd(){ return m_SpriteAlpha <= 0.0f; }
	inline void SetIsSTGEnd(){ m_IsSTGEnd = true; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::unique_ptr<CReturnTitle> m_pReturTitle;	//�^�C�g���ɖ߂�{�^���N���X.
	bool	m_IsSTGEnd;
	float	m_SpriteDisappearCount;
	float	m_SpriteAlpha;
};

#endif	//#ifndef CLEARWIDGET_H.
