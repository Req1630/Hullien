#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Character\Girl\Girl.h"

/*************************************
*	�x���N���X.
**/
CWarning::CWarning()
	: m_IsDisp( false )
{
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
}

// �`��֐�.
void CWarning::Render()
{
	if ( m_pSprite == nullptr ) return;
	// �\���t���O�������Ă��Ȃ���Ε`�悵�Ȃ�.
	if (m_IsDisp != true) return;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetDeprh( false );
	m_pSprite->Render( true );	// �r���{�[�h�ɂ���.
	m_pSprite->SetDeprh( true );
}

// ���̎q�̏�Ԑݒ�֐�.
void CWarning::SetGirlState(CGirl* pGirl)
{
	// ���̎q���댯�ȏ�Ԃ�.
	if (pGirl->IsDanger() == true)
	{
		m_vPosition = pGirl->GetPosition();	//���̎q�̈ʒu�擾.
		m_IsDisp = true;								//�`��t���O�𗧂Ă�.
	}
	else
	{
		m_IsDisp = false;
	}
}

// �X�v���C�g�̐ݒ�֐�.
bool CWarning::SpriteSetting()
{
	if ( m_pSprite != nullptr ) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if (m_pSprite == nullptr) return false;

	return true;
}
