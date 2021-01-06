#include "ConfigCursor.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Sprite\BlendSprite\BlendSprite.h"

/********************************************
*	�J�[�\���N���X.
**/
CConfigCursor::CConfigCursor()
	:	m_pBlendSprite(nullptr)
	,	m_vOldPosition(D3DXVECTOR3( 0.0f, 0.0f ,0.0f ))
	,	m_Acceleration( 0.0f )
	,	m_TargetWidth(0.0f)
	,	m_IsSetting(false)
{
	m_pBlendSprite = std::make_unique<CBlendSprite>();
}

CConfigCursor::~CConfigCursor()
{
}

// �������֐�.
bool CConfigCursor::Init()
{
	if ( SpriteSetting() == false ) return false;
	if( FAILED( m_pBlendSprite->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ))) return false;
	m_pBlendSprite->SetSpriteData( m_pSprite->GetSpriteData() );
	return true;
}

// �X�V�֐�.
void CConfigCursor::Update()
{
	if (m_pSprite == nullptr) return;
//	MoveScale();	// �X�P�[���̓���.
}

// �`��֐�.
void CConfigCursor::Render()
{
	if (m_pSprite == nullptr) return;

	m_vPosition.x += ADJ_POSITOIN_X_MAIN;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetScale( m_vScale );

	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );
}

// �X�v���C�g�ݒ�֐�.
bool CConfigCursor::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;
	return true;
}

// �X�P�[�������֐�.
void CConfigCursor::MoveScale()
{
	// �ړ��O�̍��W�ƌ��݂̍��W���Ⴄ�ꍇ.
	if (m_vPosition != m_vOldPosition)
	{
		// ���l��������.
		m_vScale.x = 0.0f;
		m_Acceleration = 0.0f;
	}

	// �g��l���W���Ȃ�Ώ������Ȃ�.
	if (m_vScale.x >= SCALE_MAX) return;
	// �g��.
	IncreaseScale();
}

// �g��֐�.
void CConfigCursor::IncreaseScale()
{
	m_vOldPosition = m_vPosition;	// ���ݒn��ێ�����.

	m_vScale.x += SCALE_SPEED - m_Acceleration;
	m_Acceleration += ACC_SPEED;

	// �g��l���W���l�ɂȂ�����.
	if ( m_vScale.x >= SCALE_MAX)
	{
		m_vScale.x = SCALE_MAX;
		m_Acceleration = 0.0f;	// �����x�̏�����.
	}
}
