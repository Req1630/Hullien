#ifndef LIMITTIME_H
#define LIMITTIME_H

#include "..\\GameWidget.h"

/*****************************************
*	�������ԃN���X.
**/
class CLimitTime : public CGameWidget
{
public:
	CLimitTime();
	virtual ~CLimitTime();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:


};

#endif	//#ifndef LIMITTIME_H.