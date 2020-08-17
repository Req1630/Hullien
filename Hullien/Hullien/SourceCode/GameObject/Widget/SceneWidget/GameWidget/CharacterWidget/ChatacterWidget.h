#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include "..\GameWidget.h"
class CCharacter;

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
	virtual void SetParameter(CCharacter& pChara) = 0;
	// �p�����[�^�ݒ�֐�.
	virtual void SetParameter(SCharacterParam& pChara)
	{
		m_Parameter.Life = pChara.Life;
		m_Parameter.SpecialAbilityGauge = pChara.SpecialAbilityGauge;
	}

protected:
	SCharacterParam m_Parameter;	//�p�����[�^.

private:
};

#endif	//#ifndef CHARACTERWIDGET_H.
