#include "ControllerConfigWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Cursor\Cursor.h"
#include "..\..\..\Switch\Switch.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"

CControllerConfigWidget::CControllerConfigWidget()
	: m_pSprites			()
	, m_pCursor				( nullptr )
	, m_pSwitch				( nullptr )
	, m_NowSelectState		( ESelectState_Vibration )
	, m_OldNowSelectState	( ESelectState_Vibration )
	, m_NowConfigState		( ESelectState_Select )
	, m_InputWaitTime		( 0.0f )
{
	m_pCursor = std::make_unique<CCursor>();
	m_pSwitch = std::make_unique<CSwitch>();
	m_pSwitch->SetValue( CXInput::IsVibration() );
}

CControllerConfigWidget::~CControllerConfigWidget()
{
}

// �������֐�.
bool CControllerConfigWidget::Init()
{
	if( SpriteSetting()		== false ) return false;
	if( m_pCursor->Init()	== false ) return false;
	if( m_pSwitch->Init()	== false ) return false;

	return true;
}

// �X�V�֐�.
void CControllerConfigWidget::Update()
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
	case ESelectState_Vibration:
		VibChoiceUpdate();	// �o�C�u�I���̍X�V.
		m_pSwitch->Update();
		break;
	default:
		break;
	}
	m_pCursor->Update();
}

// �`��֐�.
void CControllerConfigWidget::Render()
{
	switch( m_NowConfigState )
	{
	case ESelectState_Select:
		// �J�[�\���̍��W�����ݑI�����Ă���ꏊ�ɐݒ�.
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
bool CControllerConfigWidget::IsEnd()
{
	if( m_NowConfigState != ESelectState_None ) return false;
	CFileManager::BinaryWriting( CXInput::VIBRATION_CONFIG_FILE_PATH, m_pSwitch->GetValue() );
	m_NowConfigState = ESelectState_Select;
	return true;
}

// ���ʂ̑I��.
void CControllerConfigWidget::SelectType()
{
	if( m_InputWaitTime > 0.0f ) return;

	// ��Ɉړ�.
	if( GetAsyncKeyState(VK_UP) & 0x0001 || CXInput::LThumbY_Axis() > IDLE_THUMB_MAX ){
		m_NowSelectState--;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState <= ESelectState_Vibration ? ESelectState_Vibration : m_NowSelectState;
	}
	// ���Ɉړ�.
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 || CXInput::LThumbY_Axis() < IDLE_THUMB_MIN ){
		m_NowSelectState++;
		m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		m_NowSelectState = m_NowSelectState >= ESelectState_Vibration ? ESelectState_Vibration : m_NowSelectState;
	}
	// SE��炷.
	if( m_NowSelectState != m_OldNowSelectState ){
		CSoundManager::PlaySE("MoveButtonSE");
		m_OldNowSelectState = m_NowSelectState;
	}
}

// ����.
void CControllerConfigWidget::Determination()
{
	if(!(GetAsyncKeyState(VK_RETURN) & 0x0001 ) && CXInput::B_Button() != CXInput::enPRESSED_MOMENT ) return;
	switch( m_NowSelectState )
	{
	case ESelectState_Vibration:
		m_NowConfigState = ESelectState_Vibration;
		m_pSwitch->SetNowValue();
		break;
	default:
		break;
	}
	CSoundManager::PlaySE("Determination");
}

// �o�C�u�I���̍X�V.
void CControllerConfigWidget::VibChoiceUpdate()
{
	// ����.
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 || CXInput::B_Button() == CXInput::enPRESSED_MOMENT ){
		CXInput::SetVibration( m_pSwitch->GetValue() );
		CSoundManager::PlaySE("Determination");
		m_NowConfigState = ESelectState_Select;
	}
	// �L�����Z��.
	if( GetAsyncKeyState(VK_BACK) & 0x0001 || CXInput::A_Button() == CXInput::enPRESSED_MOMENT ){
		CSoundManager::PlaySE("CancelDetermination");
		m_pSwitch->ReSetValue();
		m_NowConfigState = ESelectState_Select;
	}
}

// �X�v���C�g�ݒ�֐�.
bool CControllerConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_CONTROL_NAME,
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