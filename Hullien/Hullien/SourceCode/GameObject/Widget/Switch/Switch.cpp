#include "Switch.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Utility\Input\XInput\XInput.h"

CSwitch::CSwitch()
	: CSwitch( SPRITE_ON_NAME, SPRITE_OFF_NAME )
{
}

CSwitch::CSwitch( const char* onName, const char* offName )
	: m_pSprites		()
	, m_OnSpriteName	( onName )
	, m_OffSpriteName	( offName )
	, m_InputWaitTime	( 0 )
	, m_MaxSpriteWidth	( 0.0f )
	, m_ArrowParams		( ESpriteNo_End - ESpriteNo_Begin )
	, m_SelectNo		( EArrowNo_None )
	, m_IsActive		( false )
	, m_IsValue			( false )
	, m_IsOldValue		( false )
{
}

CSwitch::~CSwitch()
{
}

// �������֐�.
bool CSwitch::Init()
{
	if( SpriteSetting() == false ) return false;

	return true;
}

// �X�V�֐�.
void CSwitch::Update()
{
	m_IsActive = true;
	if( m_InputWaitTime <= 0.0f ){
		m_SelectNo = EArrowNo_None;
		if( GetAsyncKeyState(VK_RIGHT) & 0x0001 || CXInput::LThumbX_Axis() > IDLE_THUMB_MAX || CXInput::DPadRIGHT() == CXInput::enSEPARATED ){
			m_SelectNo = EArrowNo_Right;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		}
		if( GetAsyncKeyState(VK_LEFT) & 0x0001 || CXInput::LThumbX_Axis() < IDLE_THUMB_MIN || CXInput::DPadLEFT() == CXInput::enSEPARATED ){
			m_SelectNo = EArrowNo_Left;
			m_InputWaitTime = INPUT_WAIT_TIME_MAX;
		}
	} else {
		m_InputWaitTime--;
	}
	if( m_SelectNo == EArrowNo_None ) return;
	if( m_InputWaitTime == CHANGE_FRAME ) m_IsValue = !m_IsValue;

	m_ArrowParams[m_SelectNo].Scale = 
		ARROW_SCALE + fabsf(sinf( static_cast<float>(D3DX_PI)/
			(static_cast<float>(m_InputWaitTime)+ARROW_ADJ_SCALE) )) * ARROW_SCALE_VALUE;
}

// �`��֐�.
void CSwitch::Render()
{
	// �����ԂȂ�I���摜��`��.
	if( m_IsActive == true ){
		m_pSprites[ESpriteNo_Choice]->SetPosition( m_vPosition );
		m_pSprites[ESpriteNo_Choice]->SetDeprh( false );
		m_pSprites[ESpriteNo_Choice]->SetBlend( true );
		m_pSprites[ESpriteNo_Choice]->RenderUI();
		m_pSprites[ESpriteNo_Choice]->SetBlend( false );
		m_pSprites[ESpriteNo_Choice]->SetDeprh( true );
	}

	int spriteIndex = ESpriteNo_On;
	if( m_IsValue == false ) spriteIndex = ESpriteNo_Off;
	// �I���E�I�t�摜�̕`��.
	m_pSprites[spriteIndex]->SetPosition( m_vPosition );
	m_pSprites[spriteIndex]->SetDeprh( false );
	m_pSprites[spriteIndex]->SetBlend( true );
	m_pSprites[spriteIndex]->RenderUI();
	m_pSprites[spriteIndex]->SetBlend( false );
	m_pSprites[spriteIndex]->SetDeprh( true );

	for( int i = ESpriteNo_Begin; i < ESpriteNo_End; i++ ){
		m_pSprites[i]->SetPosition( m_ArrowParams[i-ESpriteNo_ArrowRight].vPos );
		m_pSprites[i]->SetScale( m_ArrowParams[i-ESpriteNo_ArrowRight].Scale );
		m_pSprites[i]->SetDeprh( false );
		m_pSprites[i]->SetBlend( true );
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetBlend( false );
		m_pSprites[i]->SetDeprh( true );
	}

	m_IsActive = false;
}

// ���W�ݒ�֐�.
void CSwitch::SetPosition( const D3DXVECTOR3& vPos )
{
	m_vPosition = vPos;
	m_vPosition.x += ADJ_POSITION_X;

	// ���̍��W�̐ݒ�.
	m_ArrowParams[EArrowNo_Right].vPos		= m_vPosition;
	m_ArrowParams[EArrowNo_Right].vPos.x	+= m_MaxSpriteWidth;
	m_ArrowParams[EArrowNo_Left].vPos		= m_vPosition;
	m_ArrowParams[EArrowNo_Left].vPos.x		-= m_MaxSpriteWidth;
}

// �X�v���C�g�̐ݒ�֐�.
bool CSwitch::SpriteSetting()
{
	const char* spriteName[] =
	{
		m_OnSpriteName.c_str(),
		m_OffSpriteName.c_str(),
		SPRITE_CHOICE_NAME,
		SPRITE_ARROW_RIGHT,
		SPRITE_ARROW_LEFT,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprites.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
		if( m_MaxSpriteWidth <= m_pSprites[sprite]->GetSpriteSize().x ){
			m_MaxSpriteWidth = m_pSprites[sprite]->GetSpriteSize().x;
		}
	}
	m_MaxSpriteWidth *= ARROW_ADJ_POSITION_X;
	return true;
}