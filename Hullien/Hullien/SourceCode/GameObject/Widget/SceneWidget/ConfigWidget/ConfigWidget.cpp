#include "ConfigWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Common\Sprite\BlendSprite\BlendSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Common\Transition\Transition.h"
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\Cursor\ConfigCursor.h"
#include "..\..\ButtonExplanation\ButtonExplanation.h"
#include "VolumeConfigWidget/VolumeConfigWidget.h"
#include "CameraConfigWidget/CameraConfigWidget.h"
#include "ControllerConfigWidget/ControllerConfigWidget.h"
#include "GraphicConfigWidget/GraphicConfigWidget.h"
#include "..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

/********************************************
*	設定UIクラス.
**/
CConfigWidget::CConfigWidget()
	: CConfigWidget	( false )
{
}

CConfigWidget::CConfigWidget( const bool& isGame )
	: m_pSprites			()
	, m_pCursor				( nullptr )
	, m_pVolumeConfig		( nullptr )
	, m_pCameraConfig		( nullptr )
	, m_pControllerConfig	( nullptr )
	, m_pButtonExp			( nullptr )
	, m_pGraphicConfig		( nullptr )
	, m_pBlendSprite		( nullptr )
	, m_pTransition			( nullptr )
	, m_vPosition			( 0.0f, 0.0f, 0.0f )
	, m_ReturnTitlePosition	( 0.0f, 0.0f, 0.0f )
	, m_SelectState			( EConfigState_Volume )
	, m_OldSelectState		( EConfigState_Volume )
	, m_NowConfigState		( EConfigState_None )
	, m_InputWaitTime		( 0.0f )
	, m_FadeValue			( 0.0f )
	, m_IsNowGameScene		( isGame )
	, m_IsReturnToTitle		( false )
	, m_IsLoadEnd			( false )
{
	m_pCursor			= std::make_unique<CConfigCursor>();
	m_pVolumeConfig		= std::make_unique<CVolumeConfigWidget>();
	m_pCameraConfig		= std::make_unique<CCameraConfigWidget>();
	m_pControllerConfig	= std::make_unique<CControllerConfigWidget>();
	m_pButtonExp		= std::make_unique<CButtonExp>( BUTTON_EXP_RENDER_POS.x, BUTTON_EXP_RENDER_POS.y );
	m_pGraphicConfig	= std::make_unique<CGraphicConfigWidget>();
	m_pBlendSprite		= std::make_unique<CBlendSprite>();
	m_pTransition		= std::make_unique<CTransition>();

	m_pButtonExp->SetBackString();
}

CConfigWidget::~CConfigWidget()
{
}

// 初期化関数.
bool CConfigWidget::Init()
{
	if( GetInstance()->m_IsLoadEnd == true ) return true; 
	if( GetInstance()->SpriteSetting()				== false ) return false;
	if( GetInstance()->m_pCursor->Init()			== false ) return false;
	if( GetInstance()->m_pVolumeConfig->Init()		== false ) return false;
	if( GetInstance()->m_pCameraConfig->Init()		== false ) return false;
	if( GetInstance()->m_pControllerConfig->Init()	== false ) return false;
	if( GetInstance()->m_pButtonExp->Init()			== false ) return false;
	if( GetInstance()->m_pGraphicConfig->Init()	== false ) return false;
	if( FAILED( GetInstance()->m_pTransition->Init( CDirectX11::GetDevice(), CDirectX11::GetContext(), 
		"Data\\Texture\\UI\\Fade\\009TransitoinRule.png",
		GetInstance()->m_pSprites[0]->GetSpriteData()->SState.Disp.w,
		GetInstance()->m_pSprites[0]->GetSpriteData()->SState.Disp.h ))) return false;
	if( FAILED( GetInstance()->m_pBlendSprite->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ))) return false;
	GetInstance()->m_pBlendSprite->SetSpriteData( GetInstance()->m_pSprites[0]->GetSpriteData() );
	GetInstance()->m_pTransition->SetTexture( GetInstance()->m_pSprites[0]->GetSpriteData()->pTexture );
	GetInstance()->m_pTransition->SetPosition( GetInstance()->m_pSprites[0]->GetSpriteData()->SState.vPos );
	GetInstance()->m_pTransition->SetTransitionCutMode( ETRANSITION_OUT_MODE_HardLight );
	GetInstance()->m_IsLoadEnd = true;
	return true;
}

// 更新関数.
void CConfigWidget::Update()
{
	if( GetInstance()->m_NowConfigState == EConfigState_End ){
		if( GetInstance()->m_FadeValue > GetInstance()->FADE_VALUE_MIN ){
			GetInstance()->m_FadeValue -= GetInstance()->FADE_SPEED;
			return;
		}
	} else {
		if( GetInstance()->m_FadeValue < GetInstance()->FADE_VALUE_MAX ){
			GetInstance()->m_FadeValue += GetInstance()->FADE_SPEED;
			return;
		}
	}
	GetInstance()->m_InputWaitTime--;
	GetInstance()->m_pVolumeConfig->OnVolumeSeting();	// 音量の設定を有効にする.
	switch( GetInstance()->m_NowConfigState )
	{
	case EConfigState_None:
		GetInstance()->CursorSetting();
		GetInstance()->m_pCursor->Update();
		if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
			GetInstance()->m_NowConfigState = GetInstance()->m_SelectState; 
			CSoundManager::PlaySE("Determination");
		}
		if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
			GetInstance()->m_NowConfigState = EConfigState_End;
			CSoundManager::PlaySE("CancelDetermination");
		}
		break;
	case EConfigState_Volume:
		GetInstance()->m_pVolumeConfig->Update();	// 音量設定UIの更新.
		if( GetInstance()->m_pVolumeConfig->IsSaveEnd() == true ) GetInstance()->m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_Camera:
		GetInstance()->m_pCameraConfig->Update();
		if( GetInstance()->m_pCameraConfig->IsEnd() == true ) GetInstance()->m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_Controller:
		GetInstance()->m_pControllerConfig->Update();
		if( GetInstance()->m_pControllerConfig->IsEnd() == true ) GetInstance()->m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_Graphic:
		GetInstance()->m_pGraphicConfig->Update();
		if( GetInstance()->m_pGraphicConfig->IsEnd() == true ) GetInstance()->m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_BackTitle:
		if( GetInstance()->m_IsNowGameScene == true ) GetInstance()->m_IsReturnToTitle = true;
		break;
	default:
		break;
	}
}

// 描画関数.
void CConfigWidget::Render()
{
	if( GetInstance()->m_FadeValue < GetInstance()->FADE_VALUE_MAX ){
		GetInstance()->m_pTransition->SetValue( GetInstance()->m_FadeValue );
		GetInstance()->m_pTransition->SetDestTexture( CSceneTexRenderer::GetBlurTexture() );
		GetInstance()->m_pTransition->Render();
		return;
	}

	for( size_t i = 0; i < GetInstance()->m_pSprites.size(); i++ ){
		if( i == EConfigState_BackTitle ){
			if( GetInstance()->m_IsNowGameScene == false ){
				continue;
			} else {
				GetInstance()->m_pSprites[i]->SetPosition( GetInstance()->m_ReturnTitlePosition );
			}
		}
		if( i == 0 ){
			GetInstance()->m_pBlendSprite->SetDestTexture( CSceneTexRenderer::GetBlurTexture() );
			GetInstance()->m_pBlendSprite->SetDeprh(false);
			GetInstance()->m_pBlendSprite->SetBlend(true);
			GetInstance()->m_pBlendSprite->RenderUI();
			GetInstance()->m_pBlendSprite->SetBlend(false);
			GetInstance()->m_pBlendSprite->SetDeprh(false);
			if( GetInstance()->m_NowConfigState != EConfigState_None || i != GetInstance()->BACKGROUND ) continue;
			GetInstance()->m_pCursor->Render();
			continue;
		}
		GetInstance()->m_pSprites[i]->SetAlpha(1.0f);
		GetInstance()->m_pSprites[i]->SetDeprh(false);
		GetInstance()->m_pSprites[i]->SetBlend(true);
		GetInstance()->m_pSprites[i]->RenderUI();
		GetInstance()->m_pSprites[i]->SetBlend(false);
		GetInstance()->m_pSprites[i]->SetDeprh(false);
	}
	switch( GetInstance()->m_NowConfigState )
	{
	case EConfigState_None:
		GetInstance()->m_pButtonExp->Render();
		break;
	case EConfigState_Volume:
		GetInstance()->m_pVolumeConfig->Render();	// 音量設定UIの描画.
		break;
	case EConfigState_Camera:
		GetInstance()->m_pCameraConfig->Render();
		break;
	case EConfigState_Controller:
		GetInstance()->m_pControllerConfig->Render();
		break;
	case EConfigState_Graphic:
		GetInstance()->m_pGraphicConfig->Render();
		break;
	default:
		break;
	}
}

// 音量の設定をできないようにする.
void CConfigWidget::OffVolumeSeting()
{
	GetInstance()->m_pVolumeConfig->OffVolumeSeting();
}

// 設定を終了したか.
bool CConfigWidget::IsEndConfig()
{
	if( GetInstance()->m_NowConfigState == EConfigState_End ){
		if( GetInstance()->m_FadeValue > GetInstance()->FADE_VALUE_MIN ) return false;
		GetInstance()->m_NowConfigState = EConfigState_None;
		GetInstance()->m_OldSelectState = GetInstance()->m_SelectState = EConfigState_Volume;
		GetInstance()->m_vPosition = { -400.0f, 20.0f, 0.0f };
		GetInstance()->m_pCursor->SetPosition( GetInstance()->m_vPosition );
		return true;
	}
	return false;
}

// タイトルに戻るか.
bool CConfigWidget::IsReturnToTitle()
{
	if( GetInstance()->m_IsReturnToTitle == true ){
		GetInstance()->m_IsReturnToTitle = false;
		GetInstance()->m_NowConfigState = EConfigState_End;
		return true;
	}
	return false;
}


// カーソルの設定.
void CConfigWidget::CursorSetting()
{
	if( m_InputWaitTime <= 0.0f ){
		if( GetAsyncKeyState( VK_UP ) & 0x8000 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX )
		{
			m_SelectState--;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			if( m_SelectState <= EConfigState_Volume ){
				m_SelectState = EConfigState_Volume;
				m_InputWaitTime = 0.0f;
			}
		}
		if( GetAsyncKeyState( VK_DOWN ) & 0x8000 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN )
		{
			m_SelectState++;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
			const int MAX_STATE = m_IsNowGameScene == true ? EConfigState_BackTitle : EConfigState_Graphic;
			if( m_SelectState >= MAX_STATE ){
				m_SelectState = MAX_STATE;
				m_InputWaitTime = 0.0f;
			}
		}
		// SEを鳴らす.
		if( m_SelectState != m_OldSelectState ){
			CSoundManager::PlaySE("MoveButtonSE");
			m_OldSelectState = m_SelectState;
		}
	}

	switch( m_SelectState )
	{
	case EConfigState_Volume:
		m_vPosition = m_pSprites[VOLUME]->GetRenderPos();
		m_pCursor->SetWidth( m_pSprites[VOLUME]->GetSpriteSize().x );
		m_pCursor->SetPosition( m_vPosition );
		break;
	case EConfigState_Camera:
		m_pCursor->SetWidth( m_pSprites[CAMERA]->GetSpriteSize().x );
		m_vPosition = m_pSprites[CAMERA]->GetRenderPos();
		m_pCursor->SetPosition( m_vPosition );
		break;
	case EConfigState_Controller:
		m_pCursor->SetWidth( m_pSprites[CONTROLLER]->GetSpriteSize().x );
		m_vPosition = m_pSprites[CONTROLLER]->GetRenderPos();
		m_pCursor->SetPosition( m_vPosition );
		break;
	case EConfigState_Graphic:
		m_pCursor->SetWidth( m_pSprites[GRAPHIC]->GetSpriteSize().x );
		m_vPosition = m_pSprites[GRAPHIC]->GetRenderPos();
		m_pCursor->SetPosition( m_vPosition );
		break;
	case EConfigState_BackTitle:
		m_pCursor->SetWidth( m_pSprites[EConfigState_BackTitle]->GetSpriteSize().x );
		m_vPosition = m_ReturnTitlePosition;
		m_pCursor->SetPosition( m_vPosition );
		break;
	default:
		break;
	}

	m_pCursor->Update();
}

// スプライト設定関数.
bool CConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_BACK,
		SPRITE_ICON,
		SPRITE_VOLUME,
		SPRITE_CAMERA,
		SPRITE_CONTROLLER,
		SPRITE_GRAPHIC,
		SPRITE_RETURN_TITLE,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
	}
	m_ReturnTitlePosition = m_pSprites[EConfigState_Graphic]->GetRenderPos();
	m_ReturnTitlePosition.y += fabsf( m_pSprites[1]->GetRenderPos().y - m_pSprites[2]->GetRenderPos().y );
	return true;
}