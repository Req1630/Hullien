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

};

#endif	//#ifndef FADE_H.