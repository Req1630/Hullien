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
	, m_pVolumeSlinders		( EVolumeType_Max )
	, m_pSprites			()
	, m_SlinderPositions	( EVolumeType_Max )
	, m_CursorPosition		( 0.0f, 0.0f, 0.0f )
	, m_NowSelectVolume		( EVolumeType_Master )
{
	m_pCursor = std::make_unique<CCursor>();
	for( auto& v : m_pVolumeSlinders ) v = std::make_shared<CSlinder>();
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
	SelectVolumeType();	// �ݒ肷�鉹�ʎ�ނ̑I��.
	VolumeSeting();		// ���ʂ̐ݒ�.
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
	for( int i = EVolumeType_Begin; i < EVolumeType_End; i++ ){
		m_pVolumeSlinders[i]->SetPosition(m_SlinderPositions[i] );
		m_pVolumeSlinders[i]->Render();
		m_pSprites[i]->SetDeprh( false );
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetDeprh( true );
	}
}

// ���ʂ̑I��.
void CVolumeConfigWidget::SelectVolumeType()
{
	if( GetAsyncKeyState('W') & 0x0001 ){
		m_NowSelectVolume--;
		m_NowSelectVolume = m_NowSelectVolume <= EVolumeType_Master ? EVolumeType_Master : m_NowSelectVolume;
	}
	if( GetAsyncKeyState('S') & 0x0001){
		m_NowSelectVolume++;
		m_NowSelectVolume = m_NowSelectVolume >= EVolumeType_SE ? EVolumeType_SE : m_NowSelectVolume;
	}
}

// ���ʂ̐ݒ�.
void CVolumeConfigWidget::VolumeSeting()
{
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )
		m_pVolumeSlinders[m_NowSelectVolume]->AddValue( 0.01f );
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )
		m_pVolumeSlinders[m_NowSelectVolume]->SubValue( 0.01f );
}

// �X�v���C�g�ݒ�֐�.
bool CVolumeConfigWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_MASTER_NAME,
		SPRITE_BGM_NAME,
		SPRITE_SE_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if (m_pSprites[sprite] == nullptr) return false;
		m_SlinderPositions[sprite] = m_pSprites[sprite]->GetRenderPos();
		m_SlinderPositions[sprite].x += BAR_POSITION_X;
		m_SlinderPositions[sprite].y += m_pSprites[sprite]->GetSpriteSize().y/2.0f;
	}

	return true;
}