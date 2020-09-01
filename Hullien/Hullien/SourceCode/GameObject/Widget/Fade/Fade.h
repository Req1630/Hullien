#ifndef FADE_H
#define FADE_H

#include "..\Widget.h"

/************************************
*	�t�F�[�h�N���X.
**/
class CFade : public CWidget
{

public:
	CFade();
	virtual ~CFade();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;
	// �t�F�[�h�����擾�֐�.
	bool GetIsFade() const { return GetInstance()->m_IsFade; }
	// �t�F�[�h��Ԏ擾�֐�.
	EFadeState GetFadeState() const { return GetInstance()->m_FadeState; }
	// �t�F�[�h�C���ݒ�֐�.
	void SetFadeIn();
	// �t�F�[�h�A�E�g�ݒ�֐�.
	void SetFadeOut();

	// �V���O���g���̐���.
	static CFade* GetInstance()
	{
		static std::unique_ptr<CFade> pInstance = std::make_unique<CFade>();
		return pInstance.get();
	}

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	bool m_IsFade;		//�t�F�[�h����.

	//�R�s�[�E���[�u�R���X�g���N�^�A������Z�q�̍폜.
	CFade(const CFade &)						= delete;
	CFade(CFade &&)							= delete;
	CFade& operator = (const CFade &)	= delete;
	CFade& operator =(CFade &&)			= delete;

};

#endif	//#ifndef FADE_H.