#include "LimitTime.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*****************************************
*	�������ԃN���X.
**/
CLimitTime::CLimitTime()
	: m_pSprites	()
	, m_vPosition	()
	, m_Anim		()
	, m_FrameCount	( 0 )
	, m_Seconds		( LIMITTIME_MAX )
	, m_IsFinish	( false )
{
}

CLimitTime::~CLimitTime()
{
}

// �������֐�.
bool CLimitTime::Init()
{
	// �X�v���C�g�ǂݍ���.
	if( SpriteSetting() == false ) return false;
	// �ʒu�ݒ�.
	if( PositionSetting() == false ) return false;
	// �A�j���[�V�����ԍ��ǂݍ���.
	if( AnimSetting() == false ) return false;

	return true;
}

// �X�V�֐�.
void CLimitTime::Update()
{
	if (m_pSprites.size() == 0) return;

	CountDown();	//�J�E���g�_�E��.
}

// �`��֐�.
void CLimitTime::Render()
{
	if( m_pSprites.size() == 0 ) return;

	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	for (size_t sprite = 0; sprite < m_pSprites.size(); sprite++)
	{
		m_pSprites[sprite]->SetPosition( m_vPosition[sprite] );
		m_pSprites[sprite]->SetAnimNumber( m_Anim[sprite] );
		m_pSprites[sprite]->SetDeprh( false );
		m_pSprites[sprite]->SetBlend( true );
		m_pSprites[sprite]->RenderUI();
		m_pSprites[sprite]->SetBlend( false );
		m_pSprites[sprite]->SetDeprh( true );
	}
}

// �X�v���C�g�ݒ�֐�.
bool CLimitTime::SpriteSetting()
{
	if( m_pSprites.size() != 0 ) return true;

	const char* spriteName[] =
	{
		SPRITE_TIME,
		SPRITE_TIME,
	};
	const int spriteMax = sizeof( spriteName ) / sizeof( spriteName[0] );

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprites.emplace_back( CSpriteResource::GetSprite( spriteName[sprite] ));
		if ( m_pSprites[sprite] == nullptr ) return false;
	}

	m_pSprite = CSpriteResource::GetSprite( SPRITE_TIME_BACK );
	if ( m_pSprite == nullptr ) return false;


	return true;
}

// �ʒu�ݒ�֐�.
bool CLimitTime::PositionSetting()
{
	if ( m_vPosition.size() != 0 ) return true;

	const D3DXVECTOR3 vPos[] =
	{
		{ONETIME_INITPOS},
		{TENTIME_INITPOS},
	};

	for (size_t pos = 0; pos < m_pSprites.size(); pos++)
	{
		m_vPosition.emplace_back( vPos[pos] );
		if ( m_vPosition[pos] == nullptr ) return false;
	}

	return true;
}

// �A�j���[�V�����ԍ��ݒ�֐�.
bool CLimitTime::AnimSetting()
{
	if ( m_Anim.size() != 0 ) return true;

	for (size_t anim = 0; anim < m_pSprites.size(); anim++)
	{
		m_Anim.emplace_back( 0 );
	}

	return true;
}

// �J�E���g�_�E���֐�.
void CLimitTime::CountDown()
{
	FramesToSeconds();	// �t���[������b�ɕϊ�.

	int Array = 0;
	for (size_t sprite = 0; sprite < m_pSprites.size(); sprite++)
	{
		const int fPow = static_cast<int>(pow(TEN, Array));
		// �A�j���[�V�����ԍ��̐ݒ�.
		m_Anim[sprite] = static_cast<int>( m_Seconds / fPow % ANIM_MAX);

		Array++;
	}

	// �������Ԃ�0�ɂȂ����ꍇ.
	if( m_Seconds == 0 ) m_IsFinish = true;
}

// �t���[������b�ɕϊ��֐�.
void CLimitTime::FramesToSeconds()
{
	m_FrameCount++;

	if ( m_FrameCount % FRAME_DIVISION == 0 )
	{
		m_Seconds--;
	}
}


