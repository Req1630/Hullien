#include "ClearWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\ReturnTitle\ReturnTitle.h"

/***********************************************
*	�N���AUI�N���X.
**/
CClearWidget::CClearWidget()
	: m_pReturTitle	( nullptr )
{
	m_pReturTitle = std::make_unique<CReturnTitle>();
}

CClearWidget::~CClearWidget()
{
}

// �������֐�.
bool CClearWidget::Init()
{
	//�N���A�����̓ǂݍ���.
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite( "pokemon" );
	if ( m_pSprite == nullptr ) return false;

	//�^�C�g���ɖ߂�{�^���̓ǂݍ���.
	if ( m_pReturTitle->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CClearWidget::Update()
{
	if (m_pReturTitle == nullptr) return;
	m_pReturTitle->Update();
}

// �`��֐�.
void CClearWidget::Render()
{
	//�N���A����.
	if (m_pSprite == nullptr) return;
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );

	//�^�C�g���ɖ߂�{�^��.
	m_pReturTitle->Render();
}
