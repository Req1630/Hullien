#ifndef FADE_H
#define FADE_H

#include "..\Widget.h"

/************************************
*	�t�F�[�h�N���X.
**/
class CFade : public CWidget
{

public:
	CFade();
	virtual ~CFade();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;
	// �t�F�[�h�����擾�֐�.
	bool GetIsFade() const { return m_IsFade; }
	// �t�F�[�h��Ԏ擾�֐�.
	EFadeState GetFadeState() const { return m_FadeState; }


private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	bool m_IsFade;		//�t�F�[�h����.

};

#endif	//#ifndef FADE_H.