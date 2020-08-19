#include "Widget.h"

#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Common\Sprite\CSprite.h"

/****************************************
*	UI���N���X.
**/
CWidget::CWidget()
	: m_pSprite(nullptr)
	, m_Alpha(0.0f)
	, m_AlphaMax(ALPHA_MAX)
	, m_FadeState(CWidget::enFadeState::Finish)
{
}

CWidget::~CWidget()
{
}

// �t�F�[�h�C���ݒ�֐�.
void CWidget::SetFadeIn()
{
	m_FadeState = CWidget::enFadeState::In;
}

// �t�F�[�h�A�E�g�ݒ�֐�.
void CWidget::SetFadeOut()
{
	m_FadeState = CWidget::enFadeState::Out;
}

// �t�F�[�h�C���֐�.
void CWidget::FadeIn(float& alpha, const float& alphaSpeed)
{
	if (m_FadeState != m_OldFadeState)
	{
		alpha = 0.0f;
		m_OldFadeState = m_FadeState;
	}

	SetAlpha(alpha, alphaSpeed);
}

// �t�F�[�h�A�E�g�֐�.
void CWidget::FadeOut(float& alpha, const float& alphaSpeed)
{
	if (m_FadeState != m_OldFadeState)
	{
		alpha = m_AlphaMax;
		m_OldFadeState = m_FadeState;
	}

	SetAlpha(alpha, -alphaSpeed);
}

// �t�F�[�h�X�V�֐�.
void CWidget::FadeUpdate(float & alpha, const float & alphaSpeed)
{
	if (m_FadeState == EFadeState::In) { FadeIn(alpha, alphaSpeed); }
	if (m_FadeState == EFadeState::Out) { FadeOut(alpha, alphaSpeed); }
}

	

//���ߒl�ݒ�֐�.
void CWidget::SetAlpha(float & alpha, const float & alphaSpeed)
{
	alpha += alphaSpeed;
}
