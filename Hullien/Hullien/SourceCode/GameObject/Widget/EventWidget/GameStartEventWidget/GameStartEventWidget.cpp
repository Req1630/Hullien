#include "GameStartEventWidget.h"

#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*****************************************
*	�Q�[���X�^�[�g�V�[����UI�N���X.
**/
CGameStartEventWidget::CGameStartEventWidget()
	: m_pSprite		()
	, m_IsDisp		()
	, m_WidgetState	( EWidgetState::None )
	, m_Alpha		( 0.0f )
	, m_WaitCount	( 0.0f ) 
{
}

CGameStartEventWidget::~CGameStartEventWidget()
{
}

// �������֐�.
bool CGameStartEventWidget::Init()
{
	if (CEventWidget::Init() == false) return false;
	if( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CGameStartEventWidget::Update()
{
	CEventWidget::Update();

	switch (m_WidgetState)
	{
	case CGameStartEventWidget::EWidgetState::None:
		if(m_Alpha == 0.0f) return;

		for (auto d : m_IsDisp)
		{
			if (d == false) continue;
			d = false;
		}
		m_Alpha = 0.0f;
		break;

	case CGameStartEventWidget::EWidgetState::Push_YButton:		
		if(m_IsDisp[PUSH_YBUTTON] == false) m_IsDisp[PUSH_YBUTTON] = true;
		if (m_Alpha < ALPHA_MAX) m_Alpha += ALPHA_SPEED;
		break;

	case CGameStartEventWidget::EWidgetState::Preserve_Girl:
		if (m_IsDisp[PRESERVE_GIRL] == false) m_IsDisp[PRESERVE_GIRL] = true;

		if (m_WaitCount == 0 && m_Alpha < ALPHA_MAX) m_Alpha += ALPHA_SPEED;
		else {
			m_WaitCount++;

			if (m_WaitCount >= WAITCOUNT_MAX)
			{
				m_Alpha -= ALPHA_SPEED;
				if (m_Alpha <= 0.0f) { m_WidgetState = EWidgetState::None; }
			}
		}
		break;
	default:
		break;
	}
}

// �`��֐�.
void CGameStartEventWidget::Render()
{
	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		// �`��t���O�������Ă��Ȃ���Ώ������Ȃ�.
		if( m_IsDisp[sprite] == false ) continue;

		// ���ߒl�̐ݒ�.
		m_pSprite[sprite]->SetAlpha( m_Alpha );

		// �`��̐ݒ�.
		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
		m_pSprite[sprite]->SetBlend( false );
	}

	CEventWidget::Render();
}

// UI�̏�Ԑݒ�֐�.
void CGameStartEventWidget::SetWidgetState(const EWidgetState& state)
{
	m_WidgetState = state;
}

// �X�v���C�g�ݒ�֐�.
bool CGameStartEventWidget::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;
	// �摜�̐ݒ�.
	m_pSprite.emplace_back(CSpriteResource::GetSprite("PushYButton"));
	m_pSprite.emplace_back(CSpriteResource::GetSprite("PreserveGirl"));
	// �`��t���O�̐ݒ�.
	if (m_IsDisp.size() != 0) return true;
	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		m_IsDisp.emplace_back( false );
	}

	if(m_pSprite.size() == 0) return false;
	if(m_IsDisp.size() == 0) return false;
	return true;
}
