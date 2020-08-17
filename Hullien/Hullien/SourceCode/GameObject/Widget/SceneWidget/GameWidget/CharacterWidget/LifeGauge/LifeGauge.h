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
	const char* SPRITE_GAGEBACK = "gage";	//�Q�[�W�w�i.
	const char* SPRITE_GAGE		= "gage";	//�Q�[�W.		

	// �Q�[�W�̏��.
	struct stGaugeState
	{
		D3DXVECTOR3 vPosition;	//�ʒu.
		D3DXVECTOR3	vScale;		//�傫��.
		int						AnimNum;	//�A�j���[�V�����ԍ�.

		stGaugeState()
			: vPosition	(D3DXVECTOR3(0.0f,0.0f,0.0f))
			, vScale		(D3DXVECTOR3(1.0f,1.0f,1.0f))
			, AnimNum (0)
		{}
	}typedef SGaugeState;
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
	virtual void SetParameter(CCharacter& pChara) override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//�X�v���C�g�N���X.
	std::vector<SGaugeState> m_GaugeState;			//�Q�[�W�̏��.
};

#endif	//#ifndef LIFEGAUGE_H
