#ifndef WARNING_H
#define WARNING_H

#include "..\GameWidget.h"

/*************************************
*	�x���N���X.
**/
class CWarning : public CGameWidget
{
public:
	CWarning();
	virtual ~CWarning();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:
};


#endif	//#ifndef WARNING_H.