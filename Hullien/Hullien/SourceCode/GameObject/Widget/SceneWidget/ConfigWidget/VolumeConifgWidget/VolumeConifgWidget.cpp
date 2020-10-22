#include "VolumeConifgWidget.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"	
#include "..\..\..\Slider\Slider.h"
#include "..\..\..\Cursor\Cursor.h"

/********************************************
*	�ݒ�UI�N���X.
**/
CVolumeConfigWidget::CVolumeConfigWidget()
	: m_pCursor				( nullptr )
	, m_pVolumeSlinders		( ESelectType_VolumeMax )
	, m_pSprites			()
	, m_SlinderPositions	( ESelectType_VolumeMax )
	, m_CursorPosition		( 0.0f, 0.0f, 0.0f )
	, m_NowConfigState		( EConfigState_Select )
	, m_NowSelectVolume		( ESelectType_Master )
{
	m_pCursor = std::make_unique<CCursor>();
	for( auto& v : m_pVolumeSlinders ) v = std::make_shared<CSlinder>();

	// ���݂̉��ʂ̒l�̎擾.
	m_pVolumeSlinders[ESelectType_Master]->SetValue( CSoundManager::GetInstance()->m_fMasterVolume );
	m_pVolumeSlinders[ESelectType_BGM]->SetValue( CSoundManager::GetInstance()->m_fMaxBGMVolume );
	m_pVolumeSlinders[ESelectType_SE]->SetValue( CSoundManager::GetInstance()->m_fMaxSEVolume );
}

CVolumeConfigWidget::~CVolumeConfigWidget()
{
}

// �������֐�.
bool CVolumeConfigWidget::Init()
{
	if( SpriteSetting() == false ) return false;
	if( m_pCursor->Init() == false ) return false;
	for( auto& v : m_pVolumeSlinders ) if( v->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CVolumeConfigWidget::Update()
{
	switch( m_NowConfigState )
	{
	case EConfigState_Select:
		SelectType();		// �ݒ肷���ނ̑I��.
		Determination();	// �����̏���.
		break;
	case EConfigState_Seting:
		VolumeSeting();		// ���ʂ̐ݒ�.
		if( GetAsyncKeyState(VK_BACK) & 0x0001 || GetAsyncKeyState(VK_RETURN) & 0x0001 )
			m_NowConfigState = EConfigState_Select;
		break;
	default:
		break;
	}
	for( auto& v : m_pVolumeSlinders ) v->Update();	// �X���C�_�[�̍X�V.
	m_pCursor->Update();	// �J�[�\���̍X�V.
}

// �`��֐�.
void CVolumeConfigWidget::Render()
{
	// �J�[�\���̍��W�ݒ�.
	m_pCursor->SetPosition( m_pSprites[m_NowSelectVolume]->GetRenderPos() );
	m_pCursor->Render();
	// �X���C�_�[�̕`��.
	for( int i = ESelectType_Begin; i < ESelectType_End; i++ ){
		if( i == ESpriteNo_SelectIcon ) continue;
		if( i <= ESpriteNo_SE ){
			m_pVolumeSlinders[i]->SetPosition(m_SlinderPositions[i] );
			m_pVolumeSlinders[i]->Render();
		}
		m_pSprites[i]->SetDeprh( false );
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetDeprh( true );
	}
	// �I�𒆂̃A�C�R���̕\��.
	if( m_NowConfigState == EConfigState_Seting ){
		m_pSprites[ESpriteNo_SelectIcon]->SetPosition( m_pVolumeSlinders[m_NowSelectVolume]->GetIconPosition() );
		m_pSprites[ESpriteNo_SelectIcon]->SetDeprh( false );
		m_pSprites[ESpriteNo_SelectIcon]->RenderUI();
		m_pSprites[ESpriteNo_SelectIcon]->SetDeprh( true );
	}
}

// ���ʂ̑I��.
void CVolumeConfigWidget::SelectType()
{
	if( GetAsyncKeyState(VK_UP) & 0x0001 ){
		m_NowSelectVolume--;
		m_NowSelectVolume = m_NowSelectVolume <= ESelectType_Master ? ESelectType_Master : m_NowSelectVolume;
	}
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 ){
		m_NowSelectVolume++;
		m_NowSelectVolume = m_NowSelectVolume >= ESelectType_Save ? ESelectType_Save : m_NowSelectVolume;
	}
	if( GetAsyncKeyState(VK_LEFT) & 0x0001 ){
		if( m_NowSelectVolume >= ESelectType_Reset ){
			m_NowSelectVolume--;
			m_NowSelectVolume = m_NowSelectVolume <= ESelectType_Master ? ESelectType_Master : m_NowSelectVolume;
		}
	}
	if( GetAsyncKeyState(VK_RIGHT) & 0x0001 ){
		if( m_NowSelectVolume >= ESelectType_SE ){
			m_NowSelectVolume++;
			m_NowSelectVolume = m_NowSelectVolume >= ESelectType_Save ? ESelectType_Save : m_NowSelectVolume;
		}
	}
}

// ����.
void CVolumeConfigWidget::Determination()
{
	if(!(GetAsyncKeyState(VK_RETURN) & 0x0001 )) return;
	switch( m_NowSelectVolume )
	{
	case ESelectType_Master:
	case ESelectType_BGM:
	case ESelectType_SE:
		m_NowConfigState = EConfigState_Seting;
		break;
	case ESelectType_Reset:
		m_pVolumeSlinders[ESelectType_Master]->SetValue( DEFALUT_VOLUME );
		m_pVolumeSlinders[ESelectType_BGM]->SetValue( DEFALUT_VOLUME );
		m_pVolumeSlinders[ESelectType_SE]->SetValue( DEFALUT_VOLUME );
		CSoundManager::GetInstance()->m_fMasterVolume	= DEFALUT_VOLUME;
		CSoundManager::GetInstance()->m_fMaxBGMVolume	= DEFALUT_VOLUME;
		CSoundManager::GetInstance()->m_fMaxSEVolume	= DEFALUT_VOLUME;
		m_NowSelectVolume = ESelectType_Master;
		break;
	case ESelectType_Save:
		if( CSoundManager::SaveVolume() == true ) m_NowSelectVolume = ESelectType_Master;
		break;
	default:
		break;
	}
}

// ���ʂ̐ݒ�.
void CVolumeConfigWidget::VolumeSeting()
{
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )
		m_pVolumeSlinders[m_NowSelectVolume]->AddValue( 0.01f );
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )
		m_pVolumeSlinders[m_NowSelectVolume]->SubValue( 0.01f );

	switch( m_NowSelectVolume )
	{
	case ESelectType_Master:
		CSoundManager::GetInstance()->m_fMasterVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	case ESelectType_BGM:
		CSoundManager::GetInstance()->m_fMaxBGMVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	case ESelectType_SE:
		CSoundManager::GetInstance()->m_fMaxSEVolume = m_pVolumeSlinders[m_NowSelectVolume]->GetValue();
		break;
	default:
		break;
	}
}

// ���ʂ̐ݒ���ł���悤�ɂ���.
void CVolumeConfigWidget::OnVolumeSeting()
{
	// ���ʂ̐ݒ��������.
	CSoundManager::CreateChangeSoundVolumeThread();
	CSoundManager::StateChangeVolumeThread( true );

	// ���݂̉��ʂ̒l�̎擾.
	m_pVolumeSlinders[ESelectType_Master]->SetValue( CSoundManager::GetInstance()->m_fMasterVolume );
	m_pVolumeSlinders[ESelectType_BGM]->SetValue( CSoundManager::GetInstance()->m_fMaxBGMVolume );
	m_pVolumeSlinders[ESelectType_SE]->SetValue( CSoundManager::GetInstance()->m_fMaxSEVolume );
}

// ���ʂ̐ݒ���ł��Ȃ��悤�ɂ���.
void CVolumeConfigWidget::OffVolumeSeting()
{
	// ���ʂ̐ݒ��������.
	CSoundManager::StateChangeVolumeThread( false );
}

// �X�v���C�g�ݒ�֐�.
bool CVolumeConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_MASTER_NAME,
		SPRITE_BGM_NAME,
		SPRITE_SE_NAME,
		SPRITE_RESET,
		SPRITE_SAVE,
		SPRITE_ICON_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
		if( sprite <= ESpriteNo_SE ){
			m_SlinderPositions[sprite] = m_pSprites[sprite]->GetRenderPos();
			m_SlinderPositions[sprite].x += BAR_POSITION_X;
			m_SlinderPositions[sprite].y += m_pSprites[sprite]->GetSpriteSize().y/2.0f;
		}
	}

	return true;
}