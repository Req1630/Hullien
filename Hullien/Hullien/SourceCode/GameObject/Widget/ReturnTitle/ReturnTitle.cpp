#include "ReturnTitle.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	�^�C�g���ɖ߂�{�^���N���X.
**/
CReturnTitle::CReturnTitle()
{
	//�t�F�[�h�C���ݒ�.
	SetFadeIn(ALPHA_SPEED);
}

CReturnTitle::~CReturnTitle()
{
}

// �������֐�.
bool CReturnTitle::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CReturnTitle::Update()
{
	switch (m_FadeState)
	{
	case CWidget::enFadeState::In:
		if ( m_Alpha >= m_AlphaMax )
		{
			SetFadeOut(ALPHA_SPEED);
		}
		break;

	case CWidget::enFadeState::Out:
		if (m_Alpha <= 0.0f)
		{
			SetFadeIn(ALPHA_SPEED);
		}
		break;
	default:
		break;
	}

	FadeUpdate(m_Alpha);
}

// �`��֐�.
void CReturnTitle::Render()
{
	if ( m_pSprite == nullptr ) return;

	m_pSprite->SetAlpha(m_Alpha);

	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false );
}

bool CReturnTitle::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;

	return true;
}
