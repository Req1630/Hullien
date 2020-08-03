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
	const char* SPRITE_GAGEBACK = "gagesize";	//�Q�[�W�w�i.
	const char* SPRITE_GAGE		= "gagesize";	//�Q�[�W.		

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
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	// �X�v���C�g�N���X.

};

#endif	//#ifndef SPECIALABILITYGAUGE_H.