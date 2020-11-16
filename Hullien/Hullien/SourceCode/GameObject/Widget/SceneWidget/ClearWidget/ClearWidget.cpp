#include "ClearWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\ReturnTitle\ReturnTitle.h"

/***********************************************
*	�N���AUI�N���X.
**/
CClearWidget::CClearWidget()
	: m_pReturTitle				( nullptr )
	, m_IsSTGEnd				( false )
	, m_SpriteDisappearCount	( SPRITE_DIDSPPEAR_TIME )
	, m_SpriteAlpha				( 1.0f )
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
	m_pSprite = CSpriteResource::GetSprite( "gameClear" );
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
	if (m_pSprite == nullptr) return;
	if( m_SpriteDisappearCount <= 10.0f ){
		m_SpriteAlpha -= 0.01f;
		if( m_SpriteAlpha <= 0.0f ){
			m_SpriteAlpha = 0.0f;
		}
	}
	//�N���A����.
	if( m_SpriteDisappearCount >= 0.0f ){
		m_SpriteDisappearCount -= 0.1f;
		m_pSprite->SetAlpha( m_SpriteAlpha );
		m_pSprite->SetBlend( true );
		m_pSprite->SetDeprh( false );
		m_pSprite->RenderUI();
		m_pSprite->SetDeprh( true );
		m_pSprite->SetBlend( false );
	}

	//�^�C�g���ɖ߂�{�^�� STG�I�����ɕ`��.
	if( m_IsSTGEnd == true ) m_pReturTitle->Render();
}

bool CClearWidget::SpriteSetting()
{
	return false;
}
