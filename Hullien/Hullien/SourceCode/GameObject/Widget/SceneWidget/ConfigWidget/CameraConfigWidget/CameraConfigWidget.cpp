#include "CameraConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Cursor\ConfigCursor.h"
#include "..\..\..\ButtonExplanation\ButtonExplanation.h"
#include "..\..\..\Slider\Slider.h"
#include "..\..\..\Switch\Switch.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

CCameraConfigWidget::CCameraConfigWidget()
	: m_pSprites			()
	, m_pCursor				( nullptr )
	, m_pSpeedSlinder		( nullptr )
	, m_pSwitch				( nullptr )
	, m_pButtonExp			( nullptr )
	, m_ConfigState			()
	, m_SlinderPosition		( 0.0f, 0.0f, 0.0f )
	, m_IsOlsReverse		( false )
	, m_OldMoveSpeed		( 0.0f )
	, m_NowSelectState		( ESelectState_CameraControl )
	, m_OldNowSelectState	( ESelectState_CameraControl )
	, m_NowConfigState		( ESelectState_Select )
	, m_InputWaitTime		( 0.0f )
{
	m_pCursor		= std::make_unique<CConfigCursor>();
	m_pSpeedSlinder	= std::make_unique<CSlinder>( 0.1f, 0.005f );
	m_pSwitch		= std::make_unique<CSwitch>( SPRITE_NORMAL_NAME, SPRITE_INVERSION_NAME );
	m_pButtonExp	= std::make_unique<CButtonExp>( BUTTON_EXP_RENDER_POS.x, BUTTON_EXP_RENDER_POS.y );

}

CCameraConfigWidget::~CCameraConfigWidget()
{
}

// 初期化関数.
bool CCameraConfigWidget::Init()
{
	if( SpriteSetting()			== false ) return false;
	if( m_pCursor->Init()		== false ) return false;
	if( m_pSpeedSlinder->Init() == false ) return false;
	if( m_pSwitch->Init()		== false ) return false;
	if( m_pButtonExp->Init()	== false ) return false;
	if( CFileManager::BinaryReading( CRotLookAtCenter::CONFIG_FILE_PATH, m_ConfigState ) == false ) return false;
	m_pSpeedSlinder->SetValue( m_ConfigState.MoveSpeed );
	m_pSwitch->SetValue( !m_ConfigState.IsReverse );

	return true;
}

// 更新関数.
void CCameraConfigWidget::Update()
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
	case ESelectState_CameraControl:
		m_pSwitch->Update();
		CameraControlUpdate();	// カメラ操作設定の更新.
		break;
	case ESelectState_CameraSpeed:
		CameraSpeedUpdate();	// カメラ速度設定の更新.
		break;
	default:
		break;
	}
	m_pCursor->Update();
}

// 描画関数.
void CCameraConfigWidget::Render()
{
	m_pSpeedSlinder->SetPosition( m_SlinderPosition );
	m_pSpeedSlinder->Render();

	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		// カーソルの座標を現在選択している場所に設定.
		m_pCursor->SetWidth( m_pSprites[m_NowSelectState-2]->GetSpriteSize().x );
		m_pCursor->SetPosition( m_pSprites[m_NowSelectState-2]->GetRenderPos() );
		m_pCursor->Render();
		break;
	case ESelectState_CameraControl:
		break;
	case ESelectState_CameraSpeed:
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
	
	m_pSwitch->SetPosition( m_pSprites[ESpriteNo_Control]->GetRenderPos() );
	m_pSwitch->Render();
	m_pButtonExp->Render();
}

// 終了したか.
bool CCameraConfigWidget::IsEnd()
{
	if( m_NowConfigState != ESelectState_None ) return false;
	m_NowConfigState = ESelectState_Select;
	CFileManager::BinaryWriting( CRotLookAtCenter::CONFIG_FILE_PATH, m_ConfigState );
	return true;
}

// 音量の選択.
void CCameraConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// 上に移動.
	if( GetAsyncKeyState(VK_UP) & 0x8000 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState <= ESelectState_CameraControl ? ESelectState_CameraControl : m_NowSelectState;
	} 
	// 下に移動.
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState >= ESelectState_CameraSpeed ? ESelectState_CameraSpeed : m_NowSelectState;
	}
	// SEを鳴らす.
	if( m_NowSelectState != m_OldNowSelectState ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldNowSelectState = m_NowSelectState;
	}
}

// 決定.
void CCameraConfigWidget::Determination()
{
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		switch( m_NowSelectState )
		{
		case ESelectState_CameraControl:
			m_NowConfigState = ESelectState_CameraControl;
			m_pSwitch->SetNowValue();
			break;
		case ESelectState_CameraSpeed:
			m_NowConfigState = ESelectState_CameraSpeed;
			m_OldMoveSpeed = m_ConfigState.MoveSpeed;
			break;
		default:
			break;
		}
		CSoundManager::PlaySE("Determination");
	}
}

// カメラ操作の更新.
void CCameraConfigWidget::CameraControlUpdate()
{
	// 決定.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		CSoundManager::PlaySE("Determination");
		m_ConfigState.IsReverse = !m_pSwitch->GetValue();
		m_NowConfigState = ESelectState_Select;
	}
	// キャンセル.
	if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
		CSoundManager::PlaySE("CancelDetermination");
		m_NowConfigState = ESelectState_Select;
		m_pSwitch->ReSetValue();
	}
}

// カメラ速度の更新.
void CCameraConfigWidget::CameraSpeedUpdate()
{
	// 値を増やす.
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX )
		m_pSpeedSlinder->AddValue( 0.001f );
	// 値を減らす.
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN )
		m_pSpeedSlinder->SubValue( 0.001f );

	// 決定.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
		m_ConfigState.MoveSpeed = m_pSpeedSlinder->GetValue();
	}

	// キャンセル.
	if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
		CSoundManager::PlaySE("CancelDetermination");
		m_NowConfigState = ESelectState_Select;
		m_ConfigState.MoveSpeed = m_OldMoveSpeed;
		m_pSpeedSlinder->SetValue( m_OldMoveSpeed );
	}

	// スライダーの更新.
	m_pSpeedSlinder->Update();
}

// スプライト設定関数.
bool CCameraConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_CONTROL_NAME,
		SPRITE_SPEED_NAME,
		SPRITE_MARK_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
		if( sprite != ESpriteNo_Speed ) continue;
		m_SlinderPosition = m_pSprites[sprite]->GetRenderPos();
		m_SlinderPosition.x += 200.0f;
	}
	return true;
}
