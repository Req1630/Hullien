#include "GraphicConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Cursor\Cursor.h"
#include "..\..\..\Slider\Slider.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"

CGraphicConfigWidget::CGraphicConfigWidget()
	: m_pSprites			()
	, m_pCursor				( nullptr )
	, m_IsFullScreen			( true )
	, m_NowSelectState		( ESelectState_FullSC )
	, m_OldNowSelectState	( ESelectState_FullSC )
	, m_NowConfigState		( ESelectState_Select )
	, m_NowControlState		( EControlllerState_FullSCOff )
	, m_InputWaitTime		( 0.0f )
{
	m_pCursor = std::make_unique<CCursor>();
	m_IsFullScreen = CDirectX11::IsFullScreen();
}

CGraphicConfigWidget::~CGraphicConfigWidget()
{
}

// 初期化関数.
bool CGraphicConfigWidget::Init()
{
	if( SpriteSetting()		== false ) return false;
	if( m_pCursor->Init()	== false ) return false;

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
		if( GetAsyncKeyState(VK_BACK) & 0x0001 ||
			CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
			CSoundManager::PlaySE("CancelDetermination");
			m_NowConfigState = ESelectState_None;
		}
		break;
	case ESelectState_FullSC:
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
		m_pCursor->SetPosition( m_pSprites[m_NowSelectState-2]->GetRenderPos() );
		m_pCursor->Render();
		break;
	case ESelectState_FullSC:
		// カーソルの座標を現在選択している場所に設定.
		m_pCursor->SetPosition( m_pSprites[m_NowControlState+1]->GetRenderPos() );
		m_pCursor->Render();
		break;
	default:
		break;
	}

	int no = 0;
	for( auto& s : m_pSprites ){
		if( no == ESpriteNo_Choise ){
			// 通常か反転かの選択しているカーソルの座標を調整する.
			D3DXVECTOR3 pos;
			if( m_IsFullScreen == true ){
				pos = m_pSprites[ESpriteNo_On]->GetRenderPos();
			} else {
				pos = m_pSprites[ESpriteNo_Off]->GetRenderPos();
			}
			pos.y += s->GetSpriteSize().y/2.3f;
			s->SetPosition( pos );
		}
		s->SetDeprh(false);
		s->RenderUI();
		s->SetDeprh(true);
		no++;
	}
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
	if(!(GetAsyncKeyState(VK_RETURN) & 0x0001 ) && CXInput::B_Button() != CXInput::enPRESSED_MOMENT ) return;
	switch( m_NowSelectState )
	{
	case ESelectState_FullSC:
		m_NowConfigState = ESelectState_FullSC;
		break;
	default:
		break;
	}
	CSoundManager::PlaySE("Determination");
}

// フルスク選択の更新.
void CGraphicConfigWidget::FullScChoiceUpdate()
{
	// 右に移動.
	if( GetAsyncKeyState(VK_RIGHT) & 0x0001 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX ){
		m_NowControlState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowControlState = m_NowControlState >= EControlllerState_FullSCOff ? EControlllerState_FullSCOff : m_NowControlState;
	}
	// 左に移動.
	if( GetAsyncKeyState(VK_LEFT) & 0x0001 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN ){
		m_NowControlState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowControlState = m_NowControlState <= EControlllerState_FullSCOn ? EControlllerState_FullSCOn : m_NowControlState;
	} 

	// 決定.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 || CXInput::B_Button() == CXInput::enPRESSED_MOMENT ){
		m_IsFullScreen = m_NowControlState == EControlllerState_FullSCOn ? true : false;
		CDirectX11::SetFullScreen( m_IsFullScreen );
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
	}
	// キャンセル.
	if( GetAsyncKeyState(VK_BACK) & 0x0001 || CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
		CSoundManager::PlaySE("CancelDetermination");
		m_NowConfigState = ESelectState_Select;
	}
}

// スプライト設定関数.
bool CGraphicConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_FULL_SC_NAME,
		SPRITE_CHOICE_NAME,
		SPRITE_ON_NAME,
		SPRITE_OFF_NAME,
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