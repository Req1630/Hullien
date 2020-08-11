#include "Warning.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Actor.h"
#include "..\..\..\..\Actor\Character\Girl\Girl.h"

/*************************************
*	�x���N���X.
**/
CWarning::CWarning()
	: m_vPlayePos		(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vPlayeRot		(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vTargetPos	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vTargetRot	(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_IsDisp			( false )
	, m_IsUI				( false )
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
	m_vTargetRot.y = atan2f(
		m_vTargetPos.x - m_vPlayePos.x,
		m_vTargetPos.z - m_vPlayePos.z );

	if (m_vTargetRot.y > D3DX_PI)
	{
		m_vTargetRot.y = 0.0f;
	}
	else if (m_vTargetRot.y < 0.0f)
	{
		m_vTargetRot.y = D3DX_PI;
	}

	if (m_vTargetRot.y <=  m_vPlayeRot.y
		&& m_vTargetRot.y >=  m_vPlayeRot.y)
	{
		m_vPosition = m_vTargetPos;	
		m_IsUI = false;
	}
	else
	{
		m_vPosition = D3DXVECTOR3(100.0f, 450.0f, 0.0f);
		m_IsUI = true;
	}
}

// �`��֐�.
void CWarning::Render()
{
	if ( m_pSprite == nullptr ) return;
	// �\���t���O�������Ă��Ȃ���Ε`�悵�Ȃ�.
//	if (m_IsDisp != true) return;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetColor(D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	m_pSprite->SetDeprh( false );
	if (m_IsUI == false)
	{
		m_pSprite->Render(true);	// �r���{�[�h�ɂ���.
	}
	else
	{
		m_pSprite->RenderUI();
	}
	m_pSprite->SetDeprh( true );
}

// ���̎q�̏�Ԑݒ�֐�.
void CWarning::SetGirlState(CGirl* pGirl)
{
	// ���̎q�̈ʒu�擾.
	m_vTargetPos = pGirl->GetPosition();

	// ���̎q���댯�ȏ�Ԃ�.
	if (pGirl->IsDanger() == true)
	{
		m_IsDisp = true;					//�`��t���O�𗧂Ă�.
	}
	else
	{
		m_IsDisp = false;
	}
}

// �v���C���[�ʒu�擾.
void CWarning::SetPlayerPos(CActor * pActor)
{
	// �v���C���[�łȂ���Ώ������Ȃ�.
	if ( pActor->GetObjectTag() != EObjectTag::Player ) return;
	m_vPlayePos = pActor->GetPosition();
}

// �X�v���C�g�̐ݒ�֐�.
bool CWarning::SpriteSetting()
{
	if ( m_pSprite != nullptr ) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if (m_pSprite == nullptr) return false;

	return true;
}