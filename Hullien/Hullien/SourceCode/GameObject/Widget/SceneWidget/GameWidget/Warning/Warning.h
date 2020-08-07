#ifndef WARNING_H
#define WARNING_H

#include "..\GameWidget.h"
class CGirl;

/*************************************
*	�x���N���X.
**/
class CWarning : public CGameWidget
{
private:
	const char* SPRITE_NAME = "dangersignsize";

public:
	CWarning();
	virtual ~CWarning();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;
	// ���̎q�̏�Ԑݒ�֐�.
	void SetGirlState(CGirl* pGirl);

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	bool m_IsDisp;
};


#endif	//#ifndef WARNING_H.