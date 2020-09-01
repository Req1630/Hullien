#ifndef LIFEGAUGE_H
#define LIFEGAUGE_H

#include "..\ChatacterWidget.h"
#include <vector>

/*************************************
*	���C�t�Q�[�W�N���X.
**/
class CLifeGauge : public CCharacterWidget
{
private:
	const char* SPRITE_GAUGEBACK = "gage";	//�Q�[�W�w�i.
	const char* SPRITE_GAUGE			= "gage";	//�Q�[�W.		
	const int     GAUGE_NUM = 1;						//�Q�[�W�̔z��ԍ�.
	const int     ONE = 1;									//��.

public:
	CLifeGauge();
	virtual ~CLifeGauge();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;
	// �p�����[�^�ݒ�֐�.
//	virtual void SetParameter(CCharacter& pChara) override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//�X�v���C�g�N���X.
	std::vector<SGaugeState> m_GaugeState;			//�Q�[�W�̏��.
};

#endif	//#ifndef LIFEGAUGE_H
