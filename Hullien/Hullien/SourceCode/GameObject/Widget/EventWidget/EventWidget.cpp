#include "EventWidget.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	�C�x���gUI���N���X.
**/
CEventWidget::CEventWidget()
	: m_pSprite	( nullptr )
	, m_IsSkip  ( false )
	, m_Alpha	( 0.0f )
{
}

CEventWidget::~CEventWidget()
{
}

// �������֐�
bool CEventWidget::Init()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite("Skip");
	if (m_pSprite == nullptr) return false;

	return true;
}

// �X�V�֐�.
void CEventWidget::Update()
{
	if (m_IsSkip == true) return;

	if(m_Alpha <= 0.01f)	  CEventWidget::SetFadeIn(0.01f);
	if(m_Alpha >= m_AlphaMax) CEventWidget::SetFadeOut(0.01f);

	CEventWidget::FadeUpdate( m_Alpha );

}

// �`��֐�.
void CEventWidget::Render()
{
	if( m_IsSkip == true ) return;
	m_pSprite->SetAlpha( m_Alpha );

	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false);
}
