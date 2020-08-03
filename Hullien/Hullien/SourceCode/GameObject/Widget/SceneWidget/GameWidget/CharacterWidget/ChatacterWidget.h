#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include "..\GameWidget.h"

/*****************************************
*	�L�����N�^UI�N���X.
**/
class CCharacterWidget : public CGameWidget
{
public:
	struct stCharacterParam
	{
		float Life;					//���C�t.
		float SpecialAbilityGauge;	//����Q�[�W.
	}typedef SCharacterParam;

public:
	CCharacterWidget();
	virtual ~CCharacterWidget();

	// �p�����[�^�ݒ�֐�.
	void SetParameter(const CCharacterWidget::SCharacterParam& param) { m_Parameter = param; }

protected:
	SCharacterParam m_Parameter;	//�p�����[�^.

private:
};

#endif	//#ifndef CHARACTERWIDGET_H.
