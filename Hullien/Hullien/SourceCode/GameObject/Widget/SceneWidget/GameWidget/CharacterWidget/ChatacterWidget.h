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
	// �L�����N�^�̃p�����[�^.
	struct stCharacterParam
	{
		float Life;					//���C�t.
		float SpecialAbilityGauge;	//����Q�[�W.
	}typedef SCharacterParam;


	// �Q�[�W�̏��.
	struct stGaugeState
	{
		D3DXVECTOR3 vPosition;	//�ʒu.
		D3DXVECTOR3	vScale;		//�傫��.
		int						AnimNum;	//�A�j���[�V�����ԍ�.

		stGaugeState()
			: vPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			, vScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
			, AnimNum(0)
		{}
	}typedef SGaugeState;

public:
	CCharacterWidget();
	virtual ~CCharacterWidget();

	// �p�����[�^�ݒ�֐�.
//	virtual void SetParameter(CCharacter& pChara) = 0;
	// �p�����[�^�ݒ�֐�.
	virtual void SetParameter(SCharacterParam& pChara)
	{
		m_Parameter.Life = pChara.Life;
		m_Parameter.SpecialAbilityGauge = pChara.SpecialAbilityGauge;
	}

protected:
	SCharacterParam m_Parameter;							//�p�����[�^.

private:
};

#endif	//#ifndef CHARACTERWIDGET_H.
