#include "ReturnTitle.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	�^�C�g���ɖ߂�{�^���N���X.
**/
CReturnTitle::CReturnTitle()
{
	//�t�F�[�h�C���ݒ�.
	SetFadeIn();
}

CReturnTitle::~CReturnTitle()
{
}

// �������֐�.
bool CReturnTitle::Init()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;
	return true;
}

// �X�V�֐�.
void CReturnTitle::Update()
{
	switch (m_FadeState)
	{
	case CWidget::enFadeState::In:
		 FadeIn(m_Alpha, ALPHA_SPEED);

		if ( m_Alpha >= m_AlphaMax )
		{
			SetFadeOut();
		}
		break;

	case CWidget::enFadeState::Out:
		FadeOut( m_Alpha, ALPHA_SPEED);

		if (m_Alpha <= 0.0f)
		{
			SetFadeIn();
		}
		break;
	default:
		break;
	}
}

// �`��֐�.
void CReturnTitle::Render()
{
	if ( m_pSprite == nullptr ) return;

	m_pSprite->SetCoverage( true );
	m_pSprite->SetAlpha( m_Alpha );
	m_pSprite->RenderUI();
	m_pSprite->SetCoverage(false);

}
