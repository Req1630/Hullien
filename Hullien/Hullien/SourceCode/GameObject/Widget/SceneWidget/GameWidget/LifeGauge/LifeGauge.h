#ifndef LIFEGAUGE_H
#define LIFEGAUGE_H

#include "..\GameWidget.h"

/*************************************
*	���C�t�Q�[�W�N���X.
**/
class CLifeGauge : public CGameWidget
{
public:
	CLifeGauge();
	virtual ~CLifeGauge();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:
};

#endif	//#ifndef LIFEGAUGE_H
