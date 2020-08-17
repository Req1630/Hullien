#ifndef SPECIALABILITYGAUGE_H
#define SPECIALABILITYGAUGE_H

#include "..\ChatacterWidget.h"
#include <vector>

/*******************************************
*	����\�̓Q�[�W�N���X.
**/
class CSpecialAbilityGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAUGEBACK = "gage";	//�Q�[�W�w�i.
	const char* SPRITE_GAUGE = "gage";			//�Q�[�W.		
	const int     GAUGE_NUM = 1;						//�Q�[�W�̔z��ԍ�.
	const int     ONE = 1;									//��.

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
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	// �X�v���C�g�N���X.
	std::vector<SGaugeState> m_GaugeState;			//�Q�[�W�̏��.

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.