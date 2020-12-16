#include "GraphicConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Cursor\ConfigCursor.h"
#include "..\..\..\Switch\Switch.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

CGraphicConfigWidget::CGraphicConfigWidget()
	: m_pSprites			()
	, m_pCursor				( nullptr )
	, m_pSwitch				( nullptr )
	, m_IsFullScreen		( true )
	, m_NowSelectState		( ESelectState_FullSC )
	, m_OldNowSelectState	( ESelectState_FullSC )
	, m_NowConfigState		( ESelectState_Select )
	, m_InputWaitTime		( 0.0f )
{
	m_pCursor = std::make_unique<CConfigCursor>();
	m_pSwitch = std::make_unique<CSwitch>();
	m_pSwitch->SetValue( CDirectX11::IsFullScreen() );
}

CGraphicConfigWidget::~CGraphicConfigWidget()
{
}

// 初期化関数.
bool CGraphicConfigWidget::Init()
{
	if( SpriteSetting()		== false ) return false;
	if( m_pCursor->Init()	== false ) return false;
	if( m_pSwitch->Init()	== false ) return false;

	return true;
}

// 更新関数.
void CGraphicConfigWidget::Update()
{
	m_InputWaitTime--;

	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		SelectType();
		Determination();
		if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
			CSoundManager::PlaySE("CancelDetermination");
			m_NowConfigState = ESelectState_None;
		}
		break;
	case ESelectState_FullSC:
		m_pSwitch->Update();
		FullScChoiceUpdate();	// バイブ選択の更新.
		break;
	default:
		break;
	}
	m_pCursor->Update();
}

// 描画関数.
void CGraphicConfigWidget::Render()
{
	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		// カーソルの座標を現在選択している場所に設定.
		m_pCursor->SetWidth( m_pSprites[m_NowSelectState-2]->GetSpriteSize().x );
		m_pCursor->SetPosition( m_pSprites[m_NowSelectState-2]->GetRenderPos() );
		m_pCursor->Render();
		break;
	default:
		break;
	}

	for( auto& s : m_pSprites ){
		s->SetDeprh(false);
		s->SetBlend(true);
		s->RenderUI();
		s->SetBlend(false);
		s->SetDeprh(true);
	}

	m_pSwitch->SetPosition( m_pSprites[m_NowSelectState-2]->GetRenderPos() );
	m_pSwitch->Render();
}

// 終了したか.
bool CGraphicConfigWidget::IsEnd()
{
	if( m_NowConfigState != ESelectState_None ) return false;
	m_NowConfigState = ESelectState_Select;
	return true;
}

// 音量の選択.
void CGraphicConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// 上に移動.
	if( GetAsyncKeyState(VK_UP) & 0x0001 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState <= ESelectState_FullSC ? ESelectState_FullSC : m_NowSelectState;
	} 
	// 下に移動.
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState >= ESelectState_FullSC ? ESelectState_FullSC : m_NowSelectState;
	}
	// SEを鳴らす.
	if( m_NowSelectState != m_OldNowSelectState ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldNowSelectState = m_NowSelectState;
	}
}

// 決定.
void CGraphicConfigWidget::Determination()
{
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		switch( m_NowSelectState )
		{
		case ESelectState_FullSC:
			m_NowConfigState = ESelectState_FullSC;
			m_pSwitch->SetNowValue();
			break;
		default:
			break;
		}
		CSoundManager::PlaySE("Determination");
	}
}

// フルスク選択の更新.
void CGraphicConfigWidget::FullScChoiceUpdate()
{
	// 決定.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		CDirectX11::SetFullScreen( m_pSwitch->GetValue() );
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
	}
	// キャンセル.
	if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
		CSoundManager::PlaySE("CancelDetermination");
		m_pSwitch->ReSetValue();
		m_NowConfigState = ESelectState_Select;
	}
}

// スプライト設定関数.
bool CGraphicConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_FULL_SC_NAME,
		SPRITE_MARK_NAME,
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