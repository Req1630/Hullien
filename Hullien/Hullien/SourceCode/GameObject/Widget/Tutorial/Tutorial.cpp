#include "Tutorial.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Common\Transition\Transition.h"
#include "..\ButtonExplanation\ButtonExplanation.h"
#include "..\..\..\XAudio2\SoundManager.h"

CTutorial::CTutorial()
	: m_pSprites		()
	, m_pArrowSprites	()
	, m_ArrowParams		( EArrowSpriteNo_Max )
	, m_pTransition		( nullptr )
	, m_pButtonExp		( nullptr )
	, m_NowSpriteNo		( ETutorialState_One )
	, m_SelectMoveNo	( EArrowSpriteNo_None )
	, m_InputWaitTime	( 0.0f )
	, m_BackGroundAlpha	( 0.0f )
	, m_TransitionValue	( 0.0f )
	, m_IsSetup			( false )
	, m_IsTutorialEnd	( true )
{
	m_pTransition = std::make_unique<CTransition>();
	m_pButtonExp = std::make_unique<CButtonExp>( BUTTON_EXP_RENDER_POS.x, BUTTON_EXP_RENDER_POS.y );
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
	if( m_pButtonExp->Init() == false ) return false;

	m_pTransition->SetTexture( m_pSprites[m_NowSpriteNo]->GetSpriteData()->pTexture );
	m_pTransition->SetPosition( m_pSprites[m_NowSpriteNo]->GetRenderPos() );
	return true;
}

// 更新関数.
void CTutorial::Update()
{
	if( m_IsSetup == false ) return;
	if( m_IsTutorialEnd == true ) return;
	if( m_BackGroundAlpha <= BACK_GROUND_ALPHA_MAX ) return;

	if( m_InputWaitTime <= 0.0f ){
		if( CInput::IsMomentPress( EKeyBind::Right ) == true || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX ||
			CInput::IsMomentPress( EKeyBind::Decision ) == true ){
			CSoundManager::PlaySE("Determination");
			m_NowSpriteNo++;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			m_SelectMoveNo = EArrowSpriteNo_SelectArrowRight;
			if( m_NowSpriteNo >= ETutorialState_End ){
				m_IsTutorialEnd = true;
				return;
			}
		}

		if( CInput::IsMomentPress( EKeyBind::Left ) == true || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN ||
			CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
			m_NowSpriteNo--;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			m_SelectMoveNo = EArrowSpriteNo_SelectArrowLeft;
			if( m_NowSpriteNo < ETutorialState_Start ){
				m_NowSpriteNo = ETutorialState_Start;
				m_SelectMoveNo = EArrowSpriteNo_None;
				m_InputWaitTime = 0.0f;
			} else {
				CSoundManager::PlaySE("Determination");
			}
		}
	} else {
		m_InputWaitTime--;
	}

	if( m_SelectMoveNo == EArrowSpriteNo_None ) return;
	m_ArrowParams[m_SelectMoveNo].Scale = 
		ARROW_SCALE + fabsf(sinf( static_cast<float>(D3DX_PI)/ (m_InputWaitTime+ARROW_ADJ_SCALE) )) * ARROW_SCALE_VALUE;
	if( m_InputWaitTime <= 0.0f ){
		m_SelectMoveNo = EArrowSpriteNo_None;
		m_pTransition->SetTexture( m_pSprites[m_NowSpriteNo]->GetSpriteData()->pTexture );
	}
}

// 描画関数.
void CTutorial::Render()
{
	if( m_IsSetup == false ) return;

	if( m_IsTutorialEnd == false ){
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

	ArrowRender();	// 矢印の描画.
}

// 矢印の描画.
void CTutorial::ArrowRender()
{
	if( m_TransitionValue < TRANSITION_MAX ) return;

//	m_pButtonExp->Render();

	if( m_SelectMoveNo == EArrowSpriteNo_None ){
		for( int i = EArrowSpriteNo_ArrowRight; i < EArrowSpriteNo_SelectArrowRight; i++ ){
			if( m_NowSpriteNo == ETutorialState_Start && i == EArrowSpriteNo_ArrowLeft ) continue;
			if( m_NowSpriteNo == ETutorialState_Three && i == EArrowSpriteNo_ArrowRight ) continue;
			m_pArrowSprites[i]->SetPosition( m_ArrowParams[i].vPos );
			m_pArrowSprites[i]->SetScale( m_ArrowParams[i].Scale );
			m_pArrowSprites[i]->SetDeprh( false );
			m_pArrowSprites[i]->SetBlend( true );
			m_pArrowSprites[i]->RenderUI();
			m_pArrowSprites[i]->SetBlend( false );
			m_pArrowSprites[i]->SetDeprh( true );
		}
	} else {
		const int no = m_SelectMoveNo == EArrowSpriteNo_SelectArrowRight ? EArrowSpriteNo_ArrowLeft : EArrowSpriteNo_ArrowRight;

		m_pArrowSprites[no]->SetPosition( m_ArrowParams[no].vPos );
		m_pArrowSprites[no]->SetScale( m_ArrowParams[no].Scale );
		m_pArrowSprites[no]->SetDeprh( false );
		m_pArrowSprites[no]->SetBlend( true );
		m_pArrowSprites[no]->RenderUI();
		m_pArrowSprites[no]->SetBlend( false );
		m_pArrowSprites[no]->SetDeprh( true );

		m_pArrowSprites[m_SelectMoveNo]->SetPosition( m_ArrowParams[m_SelectMoveNo].vPos );
		m_pArrowSprites[m_SelectMoveNo]->SetScale( m_ArrowParams[m_SelectMoveNo].Scale );
		m_pArrowSprites[m_SelectMoveNo]->SetDeprh( false );
		m_pArrowSprites[m_SelectMoveNo]->SetBlend( true );
		m_pArrowSprites[m_SelectMoveNo]->RenderUI();
		m_pArrowSprites[m_SelectMoveNo]->SetBlend( false );
		m_pArrowSprites[m_SelectMoveNo]->SetDeprh( true );
	}
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

	const char* arrowSpriteName[] =
	{
		SPRITE_ARROW_RIGHT,
		SPRITE_ARROW_LEFT,
		SPRITE_SELECT_ARROW_RIGHT,
		SPRITE_SELECT_ARROW_LEFT,
	};
	SpriteMax = sizeof(arrowSpriteName) / sizeof(arrowSpriteName[0]);

	// メモリの最大値設定.
	m_pArrowSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pArrowSprites.emplace_back(CSpriteResource::GetSprite(arrowSpriteName[sprite]));
		if( m_pArrowSprites[sprite] == nullptr ) return false;
		m_ArrowParams[sprite].vPos = m_pSprites[0]->GetRenderPos();
		m_ArrowParams[sprite].vPos.y += m_pSprites[0]->GetSpriteSize().y/2.0f;
	}

	m_ArrowParams[EArrowSpriteNo_ArrowLeft].vPos.x			+= ARROW_RENDER_ADJ_POS_X;
	m_ArrowParams[EArrowSpriteNo_SelectArrowLeft].vPos.x	+= ARROW_RENDER_ADJ_POS_X;
	m_ArrowParams[EArrowSpriteNo_ArrowRight].vPos.x			+= m_pSprites[0]->GetSpriteSize().x - ARROW_RENDER_ADJ_POS_X;
	m_ArrowParams[EArrowSpriteNo_SelectArrowRight].vPos.x	+= m_pSprites[0]->GetSpriteSize().x - ARROW_RENDER_ADJ_POS_X;

	return true;
}