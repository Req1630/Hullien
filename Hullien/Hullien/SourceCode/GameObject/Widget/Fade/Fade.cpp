#include "Fade.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	�t�F�[�h�N���X.
**/
CFade::CFade()
	: m_IsFade(true)
{
	m_FadeState = EFadeState::Out;
}

CFade::~CFade()
{
}

// �������֐�.
bool CFade::Init()
{
	if (SpriteSetting() == false) return false;
	return true;
}

//	�X�V�֐�.
void CFade::Update()
{
	if (m_pSprite == nullptr) return;
	FadeUpdate(m_Alpha);

	//�t�F�[�h�C����Ԃ̎�.
	if (m_FadeState == EFadeState::In)
	{
		//���ߒl���ő�l��菬������΃t�F�[�h��.
		if (m_Alpha < ALPHA_MAX)
		{
			m_IsFade = true;
		}
		else
		{
			m_IsFade = false;
		}
	}
	//�t�F�[�h�A�E�g��Ԃ̎�.
	else if (m_FadeState == EFadeState::Out)
	{
		//���ߒl���ŏ��l���傫����΃t�F�[�h��.
		if (m_Alpha > 0.0f)
		{
			m_IsFade = true;
		}
		else
		{
			m_IsFade = false;
		}
	}
}

// �`��֐�.
void CFade::Render()
{
	if (m_pSprite == nullptr) return;
	m_pSprite->SetAlpha(m_Alpha);

	m_pSprite->SetBlend(true);
	m_pSprite->SetDeprh(false);
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh(true);
	m_pSprite->SetBlend(false);
}

// �X�v���C�g�ݒ�֐�.
bool CFade::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite("CBackGround");
	if (m_pSprite == false) return false;

	return true;
}
