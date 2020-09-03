#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Character\Girl\Girl.h"
#include "..\..\..\..\..\Camera\CameraManager\CameraManager.h"

/*************************************
*	�x���N���X.
**/
CWarning::CWarning()
	: m_vTargetPos	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vTargetRot	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_IsUI				( false )
	, m_pCamera		( nullptr )
{
	m_pCamera = std::make_shared<CCameraManager>();
}

CWarning::~CWarning()
{
}

// �������֐�.
bool CWarning::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CWarning::Update()
{
	m_vPosition = m_vTargetPos;
	m_vPosition.y = m_vTargetPos.y + ADJ_POS_Y; // ���̎q�̓���ɐݒ�.
}

// �`��֐�.
void CWarning::Render()
{
	if ( m_pSprite == nullptr ) return;
	// �\���t���O�������Ă��Ȃ���Ε`�悵�Ȃ�.
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->Render(true);	// �r���{�[�h�ɂ���.
}

// �X�v���C�g�̐ݒ�֐�.
bool CWarning::SpriteSetting()
{
	if ( m_pSprite != nullptr ) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if (m_pSprite == nullptr) return false;

	return true;
}