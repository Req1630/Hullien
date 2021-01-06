#include "ConfigWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Common\Sprite\BlendSprite\BlendSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Common\Transition\Transition.h"
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\Cursor\ConfigCursor.h"
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
	, m_pGraphicConfig		( nullptr )
	, m_pBlendSprite		( nullptr )
	, m_pTransition			( nullptr )
	, m_ReturnTitlePosition	( 0.0f, 0.0f, 0.0f )
	, m_SelectState			( EConfigState_Volume )
	, m_OldSelectState		( EConfigState_Volume )
	, m_NowConfigState		( EConfigState_None )
	, m_FadeValue			( 0.0f )
	, m_IsNowGameScene		( isGame )
	, m_IsReturnToTitle		( false )
{
	m_pCursor			= std::make_unique<CConfigCursor>();
	m_pVolumeConfig		= std::make_unique<CVolumeConfigWidget>();
	m_pCameraConfig		= std::make_unique<CCameraConfigWidget>();
	m_pControllerConfig	= std::make_unique<CControllerConfigWidget>();
	m_pGraphicConfig	= std::make_unique<CGraphicConfigWidget>();
	m_pBlendSprite		= std::make_unique<CBlendSprite>();
	m_pTransition		= std::make_unique<CTransition>();
}

CConfigWidget::~CConfigWidget()
{
}

// 初期化関数.
bool CConfigWidget::Init()
{
	if( SpriteSetting()				== false ) return false;
	if( m_pCursor->Init()			== false ) return false;
	if( m_pVolumeConfig->Init()		== false ) return false;
	if( m_pCameraConfig->Init()		== false ) return false;
	if( m_pControllerConfig->Init()	== false ) return false;
	if( m_pGraphicConfig->Init()	== false ) return false;
	if( FAILED( m_pTransition->Init( CDirectX11::GetDevice(), CDirectX11::GetContext(), 
		"Data\\Texture\\UI\\Fade\\009TransitoinRule.png",
		m_pSprites[0]->GetSpriteData()->SState.Disp.w,
		m_pSprites[0]->GetSpriteData()->SState.Disp.h ))) return false;
	if( FAILED( m_pBlendSprite->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ))) return false;
	m_pBlendSprite->SetSpriteData( m_pSprites[0]->GetSpriteData() );
	m_pTransition->SetTexture( m_pSprites[0]->GetSpriteData()->pTexture );
	m_pTransition->SetPosition( m_pSprites[0]->GetSpriteData()->SState.vPos );
	return true;
}

// 更新関数.
void CConfigWidget::Update()
{
	if( m_NowConfigState == EConfigState_End ){
		if( m_FadeValue > FADE_VALUE_MIN ){
			m_FadeValue -= FADE_SPEED;
			return;
		}
	} else {
		if( m_FadeValue < FADE_VALUE_MAX ){
			m_FadeValue += FADE_SPEED;
			return;
		}
	}
	m_InputWaitTime--;
	m_pVolumeConfig->OnVolumeSeting();	// 音量の設定を有効にする.
	switch( m_NowConfigState )
	{
	case EConfigState_None:
		CursorSetting();
		m_pCursor->Update();
		if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
			m_NowConfigState = m_SelectState; 
			CSoundManager::PlaySE("Determination");
		}
		if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
			m_NowConfigState = EConfigState_End;
			CSoundManager::PlaySE("CancelDetermination");
		}
		break;
	case EConfigState_Volume:
		m_pVolumeConfig->Update();	// 音量設定UIの更新.
		if( m_pVolumeConfig->IsSaveEnd() == true ) m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_Camera:
		m_pCameraConfig->Update();
		if( m_pCameraConfig->IsEnd() == true ) m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_Controller:
		m_pControllerConfig->Update();
		if( m_pControllerConfig->IsEnd() == true ) m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_Graphic:
		m_pGraphicConfig->Update();
		if( m_pGraphicConfig->IsEnd() == true ) m_NowConfigState = EConfigState_None;
		break;
	case EConfigState_BackTitle:
		if( m_IsNowGameScene == true ) m_IsReturnToTitle = true;
		break;
	default:
		break;
	}
}

// 描画関数.
void CConfigWidget::Render()
{
	if( m_FadeValue < FADE_VALUE_MAX ){
		m_pTransition->SetValue( m_FadeValue );
		m_pTransition->SetDestTexture( CSceneTexRenderer::GetGBuffer()[0] );
		m_pTransition->Render();
		return;
	}

	for( size_t i = 0; i < m_pSprites.size(); i++ ){
		if( i == EConfigState_BackTitle ){
			if( m_IsNowGameScene == false ){
				continue;
			} else {
				m_pSprites[i]->SetPosition( m_ReturnTitlePosition );
			}
		}
		if( i == 0 ){
			m_pBlendSprite->SetDestTexture( CSceneTexRenderer::GetGBuffer()[0] );
			m_pBlendSprite->SetDeprh(false);
			m_pBlendSprite->SetBlend(true);
			m_pBlendSprite->RenderUI();
			m_pBlendSprite->SetBlend(false);
			m_pBlendSprite->SetDeprh(false);
			if( m_NowConfigState != EConfigState_None || i != BACKGROUND ) continue;
			m_pCursor->Render();
			continue;
		}
		m_pSprites[i]->SetDeprh(false);
		m_pSprites[i]->SetBlend(true);
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetBlend(false);
		m_pSprites[i]->SetDeprh(false);
	}
	switch( m_NowConfigState )
	{
	case EConfigState_None:
		break;
	case EConfigState_Volume:
		m_pVolumeConfig->Render();	// 音量設定UIの描画.
		break;
	case EConfigState_Camera:
		m_pCameraConfig->Render();
		break;
	case EConfigState_Controller:
		m_pControllerConfig->Render();
		break;
	case EConfigState_Graphic:
		m_pGraphicConfig->Render();
		break;
	default:
		break;
	}
}

// 音量の設定をできないようにする.
void CConfigWidget::OffVolumeSeting()
{
	m_pVolumeConfig->OffVolumeSeting();
}

// 設定を終了したか.
bool CConfigWidget::IsEndConfig()
{
	if( m_NowConfigState == EConfigState_End ){
		if( m_FadeValue > FADE_VALUE_MIN ) return false;
		m_NowConfigState = EConfigState_None;
		m_SelectState = EConfigState_Volume;
		return true;
	}
	return false;
}

// タイトルに戻るか.
bool CConfigWidget::IsReturnToTitle()
{
	return m_IsReturnToTitle;
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