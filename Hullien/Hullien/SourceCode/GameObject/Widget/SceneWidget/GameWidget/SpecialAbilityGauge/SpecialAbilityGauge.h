#ifndef SPECIALABILITYGAUGE_H
#define SPECIALABILITYGAUGE_H

#include "..\GameWidget.h"

/*******************************************
*	����\�̓Q�[�W�N���X.
**/
class CSpecialAbilityGauge : public CGameWidget
{
public:
	CSpecialAbilityGauge();
	virtual ~CSpecialAbilityGauge();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.