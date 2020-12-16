#include "VolumeConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Slider\Slider.h"
#include "..\..\..\Cursor\ConfigCursor.h"

/********************************************
*	設定UIクラス.
**/
CVolumeConfigWidget::CVolumeConfigWidget()
	: m_pCursor				( nullptr )
	, m_pVolumeSlinders		( ESelectType_VolumeMax )
	, m_pSprites			()
	, m_SlinderPositions	( ESelectType_VolumeMax )
	, m_CursorPosition		( 0.0f, 0.0f, 0.0f )
	, m_NowConfigState		( EConfigState_Select )
	, m_NowSelectVolume		( ESelectType_Master )
	, m_OldSelectVolume		( ESelectType_Master )
	, m_OldVolume			( 0.0f )
	, m_InputWaitTime		( 0.0f )
	, m_IsOneStep			( false )
{
	m_pCursor = std::make_unique<CConfigCursor>();
	for( auto& v : m_pVolumeSlinders ) v = std::make_shared<CSlinder>();

	// 現在の音量の値の取得.
	m_pVolumeSlinders[ESelectType_Master]->SetValue( CSoundManager::GetInstance()->m_stSound.MasterVolume );
	m_pVolumeSlinders[ESelectType_BGM]->SetValue( CSoundManager::GetInstance()->m_stSound.BGMVolume );
	m_pVolumeSlinders[ESelectType_SE]->SetValue( CSoundManager::GetInstance()->m_stSound.SEVolume );
}

CVolumeConfigWidget::~CVolumeConfigWidget()
{
}

// 初期化関数.
bool CVolumeConfigWidget::Init()
{
	if( SpriteSetting() == false ) return false;
	if( m_pCursor->Init() == false ) return false;
	for( auto& v : m_pVolumeSlinders ) if( v->Init() == false ) return false;

	return true;
}

// 更新関数.
void CVolumeConfigWidget::Update()
{
	m_InputWaitTime--;
	switch( m_NowConfigState )
	{
	case EConfigState_Select:
		SelectType();		// 設定する種類の選択.
		Determination();	// 決定後の処理.
		// キャンセル.
		if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
			m_NowSelectVolume = EVolumeType_End;
			CSoundManager::PlaySE("CancelDetermination");
		}
		break;
	case EConfigState_Seting:
		m_pVolumeSlinders[m_NowSelectVolume]->Update();	// スライダーの更新.
		VolumeSeting();		// 音量の設定.
		// 決定.
		if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
			CSoundManager::PlaySE("Determination");
			m_NowConfigState = EConfigState_Select;
		}
		// キャンセル.
		if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
			m_pVolumeSlinders[m_NowSelectVolume]->SetValue(m_OldVolume);
			// 設定した値をサウンドマネージャーへ渡す.
			switch( m_NowSelectVolume )
			{
			case ESelectType_Master:
				CSoundManager::GetInstance()->m_stSound.MasterVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
				break;
			case ESelectType_BGM:
				CSoundManager::GetInstance()->m_stSound.BGMVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
				break;
			case ESelectType_SE:
				CSoundManager::GetInstance()->m_stSound.SEVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
				break;
			default:
				break;
			}
			m_NowConfigState = EConfigState_Select;
			CSoundManager::PlaySE("CancelDetermination");
		}
		break;
	default:
		break;
	}
	m_pCursor->Update();	// カーソルの更新.
}

// 描画関数.
void CVolumeConfigWidget::Render()
{
	// カーソルの座標設定.
	m_pCursor->SetWidth( m_pSprites[m_NowSelectVolume]->GetSpriteSize().x );
	m_pCursor->SetPosition( m_pSprites[m_NowSelectVolume]->GetRenderPos() );
	m_pCursor->Render();
	// スライダーの描画.
	for( int i = ESelectType_Begin; i < ESelectType_End; i++ ){
		if( i <= ESpriteNo_SE ){
			m_pVolumeSlinders[i]->SetPosition( m_SlinderPositions[i] );
			m_pVolumeSlinders[i]->Render();
		}
		if( i == EVolumeType_MarkIcon ){
			if( m_NowConfigState == EConfigState_Seting ){
				D3DXVECTOR3 pos = m_SlinderPositions[m_NowSelectVolume];
				pos.x -= MARK_ICON_POS_X;
				m_pSprites[EVolumeType_MarkIcon]->SetPosition( pos );
			} else {
				m_pSprites[EVolumeType_MarkIcon]->SetPosition( {-BAR_POSITION_X, -BAR_POSITION_X, 0.0f} );
			}
		}
		m_pSprites[i]->SetDeprh( false );
		m_pSprites[i]->SetBlend( true );
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetBlend( false );
		m_pSprites[i]->SetDeprh( true );
	}
}

// 音量の選択.
void CVolumeConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// 上に移動.
	if( GetAsyncKeyState(VK_UP) & 0x8000 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectVolume--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectVolume = m_NowSelectVolume <= ESelectType_Master ? ESelectType_Master : m_NowSelectVolume;
	} 
	// 下に移動.
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectVolume++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectVolume = m_NowSelectVolume >= ESelectType_SE ? ESelectType_SE : m_NowSelectVolume;
	}
	// SEを鳴らす.
	if( m_NowSelectVolume != m_OldSelectVolume ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldSelectVolume = m_NowSelectVolume;
	}
}

// 決定.
void CVolumeConfigWidget::Determination()
{
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		switch( m_NowSelectVolume )
		{
		case ESelectType_Master:
		case ESelectType_BGM:
		case ESelectType_SE:
			// 音量の設定へ移動.
			m_NowConfigState = EConfigState_Seting;
			CSoundManager::PlaySE("Determination");
			break;
		default:
			break;
		}
		m_OldVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
	}
}

// 音量の設定.
void CVolumeConfigWidget::VolumeSeting()
{
	// 値を増やす.
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX )
		m_pVolumeSlinders[m_NowSelectVolume]->AddValue( 0.01f );
	// 値を減らす.
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN )
		m_pVolumeSlinders[m_NowSelectVolume]->SubValue( 0.01f );
	
	// 設定した値をサウンドマネージャーへ渡す.
	switch( m_NowSelectVolume )
	{
	case ESelectType_Master:
		CSoundManager::GetInstance()->m_stSound.MasterVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	case ESelectType_BGM:
		CSoundManager::GetInstance()->m_stSound.BGMVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	case ESelectType_SE:
		CSoundManager::GetInstance()->m_stSound.SEVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	default:
		break;
	}
}

// 音量の設定をできるようにする.
void CVolumeConfigWidget::OnVolumeSeting()
{
	if( m_IsOneStep == true ) return;
	// 音量の設定を許可する.
	CSoundManager::CreateChangeSoundVolumeThread();
	CSoundManager::StateChangeVolumeThread( true );

	// 現在の音量の値の取得.
	m_pVolumeSlinders[ESelectType_Master]->SetValue( CSoundManager::GetInstance()->m_stSound.MasterVolume );
	m_pVolumeSlinders[ESelectType_BGM]->SetValue( CSoundManager::GetInstance()->m_stSound.BGMVolume );
	m_pVolumeSlinders[ESelectType_SE]->SetValue( CSoundManager::GetInstance()->m_stSound.SEVolume );
	m_IsOneStep = true;
}

// 音量の設定をできないようにする.
void CVolumeConfigWidget::OffVolumeSeting()
{
	// 音量の設定を許可する.
	CSoundManager::StateChangeVolumeThread( false );
}

// 保存終了.
bool CVolumeConfigWidget::IsSaveEnd()
{
	if( CSoundManager::SaveVolume() == true ){}
	if( m_NowSelectVolume == EVolumeType_End ){
		m_OldSelectVolume = m_NowSelectVolume = ESelectType_Master;
		return true;
	}
	return false;
}

// スプライト設定関数.
bool CVolumeConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_MASTER_NAME,	// マスター.
		SPRITE_BGM_NAME,	// BGM.
		SPRITE_SE_NAME,		// SE.
		SPRITE_MARK_NAME,
		SPRITE_MARK_ICON_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
		if( sprite <= ESpriteNo_SE ){
			m_SlinderPositions[sprite] = m_pSprites[sprite]->GetRenderPos();
			m_SlinderPositions[sprite].x += BAR_POSITION_X;
		}
	}

	return true;
}