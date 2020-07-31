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
	const char* SPRITE_GAGEBACK = "gagesize";	//�Q�[�W�w�i.
	const char* SPRITE_GAGE		= "gagesize";	//�Q�[�W.		

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
	// �X�v���C�g�ǂݍ��݊֐�.
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//�X�v���C�g�N���X.
};

#endif	//#ifndef LIFEGAUGE_H
