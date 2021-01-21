#include "Tutorial.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Common\Transition\Transition.h"

// 定数.
namespace
{
	const char* SPRITE_TUTORIAL_ONE_NAME	= "Tutorial1";	// チュートリアル画像.
	const char* SPRITE_TUTORIAL_TWO_NAME	= "Tutorial2";	// チュートリアル画像.
	const char* SPRITE_TUTORIAL_THREE_NAME	= "Tutorial3";	// チュートリアル画像.
	const char* SPRITE_BACKGROUND_NAME		= "Fade";		// 背景画像.

	const float BACK_GROUND_ALPHA_SPEED		= 0.01f;	// 背景のアルファ速度.
	const float BACK_GROUND_ALPHA_MAX		= 0.4f;		// 背景のアルファ最大値.
	const float BACK_GROUND_ALPHA_MIN		= 0.0f;		// 背景のアルファ最小値.
	const float TRANSITION_SPEED			= 0.02f;	// トランジションの速度.
	const float TRANSITION_MAX				= 1.0f;		// トランジションの最大値.
	const float TRANSITION_MIN				= 0.0f;		// トランジションの最小値.
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

// 初期化関数.
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

// 更新関数.
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

// 描画関数.
void CTutorial::Render()
{
	if( m_IsSetup == false ) return;

	if( m_IsTurorialEnd == false ){
		// 背景のアルファ値が最大値より小さければ.
		if( m_BackGroundAlpha <= BACK_GROUND_ALPHA_MAX ){
			m_BackGroundAlpha += BACK_GROUND_ALPHA_SPEED;	// アルファ値を加算.
		} else {
			// アルファ値が最大値に達すれば.
			m_TransitionValue += TRANSITION_SPEED;	// トランジションの値を加算.
			if( m_TransitionValue > TRANSITION_MAX ) m_TransitionValue = TRANSITION_MAX;
			if( m_TransitionValue < TRANSITION_MIN ) m_TransitionValue = TRANSITION_MIN;
		}
	} else {
		// トランジションの値が最小値より大きければ.
		if( m_TransitionValue >= TRANSITION_MIN ){
			m_TransitionValue -= TRANSITION_SPEED;	// トランジションの値を減算.
		} else {
			m_BackGroundAlpha -= BACK_GROUND_ALPHA_SPEED;	// アルファ値を減算.
			if( m_BackGroundAlpha <= BACK_GROUND_ALPHA_MIN ) return;
		}
	}

	// 背景の描画.
	m_pSprite->SetAlpha( m_BackGroundAlpha );
	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	// チュートリアル画像の描画.
	m_pTransition->SetValue( m_TransitionValue );
	m_pTransition->IsScreenSize( false );
	m_pTransition->SetTransitionCutMode( ETRANSITION_OUT_MODE_Alpha );
	m_pTransition->SetDeprh( false );
	m_pTransition->SetBlend( true );
	m_pTransition->Render();
	m_pTransition->SetBlend( false );
	m_pTransition->SetDeprh( true );
}

// スプライト設定関数.
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

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	return true;
}