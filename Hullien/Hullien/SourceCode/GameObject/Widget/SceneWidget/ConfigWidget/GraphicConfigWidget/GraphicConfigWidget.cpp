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

// �������֐�.
bool CGraphicConfigWidget::Init()
{
	if( SpriteSetting()		== false ) return false;
	if( m_pCursor->Init()	== false ) return false;
	if( m_pSwitch->Init()	== false ) return false;

	return true;
}

// �X�V�֐�.
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
		FullScChoiceUpdate();	// �o�C�u�I���̍X�V.
		break;
	default:
		break;
	}
	m_pCursor->Update();
}

// �`��֐�.
void CGraphicConfigWidget::Render()
{
	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		// �J�[�\���̍��W�����ݑI�����Ă���ꏊ�ɐݒ�.
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

// �I��������.
bool CGraphicConfigWidget::IsEnd()
{
	if( m_NowConfigState != ESelectState_None ) return false;
	m_NowConfigState = ESelectState_Select;
	return true;
}

// ���ʂ̑I��.
void CGraphicConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// ��Ɉړ�.
	if( GetAsyncKeyState(VK_UP) & 0x0001 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState <= ESelectState_FullSC ? ESelectState_FullSC : m_NowSelectState;
	} 
	// ���Ɉړ�.
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState >= ESelectState_FullSC ? ESelectState_FullSC : m_NowSelectState;
	}
	// SE��炷.
	if( m_NowSelectState != m_OldNowSelectState ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldNowSelectState = m_NowSelectState;
	}
}

// ����.
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

// �t���X�N�I���̍X�V.
void CGraphicConfigWidget::FullScChoiceUpdate()
{
	// ����.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		CDirectX11::SetFullScreen( m_pSwitch->GetValue() );
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
	}
	// �L�����Z��.
	if( CInput::IsMomentPress( EKeyBind::Cancel ) == true ){
		CSoundManager::PlaySE("CancelDetermination");
		m_pSwitch->ReSetValue();
		m_NowConfigState = ESelectState_Select;
	}
}

// �X�v���C�g�ݒ�֐�.
bool CGraphicConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_FULL_SC_NAME,
		SPRITE_MARK_NAME,
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