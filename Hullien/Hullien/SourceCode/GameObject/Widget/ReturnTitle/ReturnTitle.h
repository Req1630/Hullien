#ifndef RETURNTITLE_H
#define RETURNTITLE_H

#include "..\Widget.h"

/*************************************
*	�^�C�g���ɖ߂�{�^���N���X.
**/
class CReturnTitle : public CWidget
{
private:
	const char* SPRITE_NAME = "title";	//�X�v���C�g�̃t�@�C����.
	const float	 ALPHA_SPEED = 0.01f;				//���ߑ��x.

public:
	CReturnTitle();
	virtual ~ CReturnTitle();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;


private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

	
};
#endif	//#ifndef RETURNTITLE_H.