#include "Tutorial.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Common\Transition\Transition.h"

// �萔.
namespace
{
	const char* SPRITE_TUTORIAL_ONE_NAME	= "Tutorial1";	// �`���[�g���A���摜.
	const char* SPRITE_TUTORIAL_TWO_NAME	= "Tutorial2";	// �`���[�g���A���摜.
	const char* SPRITE_TUTORIAL_THREE_NAME	= "Tutorial3";	// �`���[�g���A���摜.
	const char* SPRITE_BACKGROUND_NAME		= "Fade";		// �w�i�摜.

	const float BACK_GROUND_ALPHA_SPEED		= 0.01f;	// �w�i�̃A���t�@���x.
	const float BACK_GROUND_ALPHA_MAX		= 0.4f;		// �w�i�̃A���t�@�ő�l.
	const float BACK_GROUND_ALPHA_MIN		= 0.0f;		// �w�i�̃A���t�@�ŏ��l.
	const float TRANSITION_SPEED			= 0.02f;	// �g�����W�V�����̑��x.
	const float TRANSITION_MAX				= 1.0f;		// �g�����W�V�����̍ő�l.
	const float TRANSITION_MIN				= 0.0f;		// �g�����W�V�����̍ŏ��l.
};

CTutorial::CTutorial()
	: m_pSprites		()
	, m_pTransition		( nullptr )
	, m_NowSpriteNo		( ETutorialState_One )
	, m_BackGroundAlpha	( 0.0f )
	, m_TransitionValue	( 0.0f )
	, m_IsSetup			( false )
	, m_IsTurorialEnd	( true )
{
	m_pTransition = std::make_unique<CTransition>();
}

CTutorial::~CTutorial()
{
}

// �������֐�.
bool CTutorial::Init()
{
	if( SpriteSetting() == false ) return false;
	if( FAILED( m_pTransition->Init(
		CDirectX11::GetDevice(),
		CDirectX11::GetContext(),
		"Data\\Texture\\UI\\Fade\\171TransitoinRule.png",
		m_pSprites[m_NowSpriteNo]->GetSpriteSize().x,
		m_pSprites[m_NowSpriteNo]->GetSpriteSize().y ) )) return false;

	m_pTransition->SetTexture( m_pSprites[m_NowSpriteNo]->GetSpriteData()->pTexture );
	m_pTransition->SetPosition( m_pSprites[m_NowSpriteNo]->GetRenderPos() );
	return true;
}

// �X�V�֐�.
void CTutorial::Update()
{
	if( m_IsSetup == false ) return;
	if( m_IsTurorialEnd == true ) return;
	if( m_BackGroundAlpha <= BACK_GROUND_ALPHA_MAX ) return;

	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		m_NowSpriteNo++;
		if( m_NowSpriteNo >= ETutorialState_End ){
			m_IsTurorialEnd = true;
			return;
		}
		m_pTransition->SetTexture( m_pSprites[m_NowSpriteNo]->GetSpriteData()->pTexture );
	}

	if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
		m_NowSpriteNo--;
		if( m_NowSpriteNo < ETutorialState_Start ) m_NowSpriteNo = ETutorialState_Start;
		m_pTransition->SetTexture( m_pSprites[m_NowSpriteNo]->GetSpriteData()->pTexture );
	}
}

// �`��֐�.
void CTutorial::Render()
{
	if( m_IsSetup == false ) return;

	if( m_IsTurorialEnd == false ){
		// �w�i�̃A���t�@�l���ő�l��菬�������.
		if( m_BackGroundAlpha <= BACK_GROUND_ALPHA_MAX ){
			m_BackGroundAlpha += BACK_GROUND_ALPHA_SPEED;	// �A���t�@�l�����Z.
		} else {
			// �A���t�@�l���ő�l�ɒB�����.
			m_TransitionValue += TRANSITION_SPEED;	// �g�����W�V�����̒l�����Z.
			if( m_TransitionValue > TRANSITION_MAX ) m_TransitionValue = TRANSITION_MAX;
			if( m_TransitionValue < TRANSITION_MIN ) m_TransitionValue = TRANSITION_MIN;
		}
	} else {
		// �g�����W�V�����̒l���ŏ��l���傫�����.
		if( m_TransitionValue >= TRANSITION_MIN ){
			m_TransitionValue -= TRANSITION_SPEED;	// �g�����W�V�����̒l�����Z.
		} else {
			m_BackGroundAlpha -= BACK_GROUND_ALPHA_SPEED;	// �A���t�@�l�����Z.
			if( m_BackGroundAlpha <= BACK_GROUND_ALPHA_MIN ) return;
		}
	}

	// �w�i�̕`��.
	m_pSprite->SetAlpha( m_BackGroundAlpha );
	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	// �`���[�g���A���摜�̕`��.
	m_pTransition->SetValue( m_TransitionValue );
	m_pTransition->IsScreenSize( false );
	m_pTransition->SetTransitionCutMode( ETRANSITION_OUT_MODE_Alpha );
	m_pTransition->SetDeprh( false );
	m_pTransition->SetBlend( true );
	m_pTransition->Render();
	m_pTransition->SetBlend( false );
	m_pTransition->SetDeprh( true );
}

// �X�v���C�g�ݒ�֐�.
bool CTutorial::SpriteSetting()
{
	if( m_pSprite == nullptr ){
		m_pSprite = CSpriteResource::GetSprite( SPRITE_BACKGROUND_NAME );
		if( m_pSprite == nullptr ) return false;
	}

	const char* spriteName[] =
	{
		SPRITE_TUTORIAL_ONE_NAME,
		SPRITE_TUTORIAL_TWO_NAME,
		SPRITE_TUTORIAL_THREE_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	return true;
}