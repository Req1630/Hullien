#include "ContinueWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Cursor\Cursor.h"

/********************************************
*	�R���e�j���[UI�N���X.
**/
CContinueWidget::CContinueWidget()
	: m_pSprite			()
	, m_pCursor			( nullptr )
	, m_SelectState	( ESelectState::Yes )
{
	m_pCursor = std::make_shared<CCursor>();
}

CContinueWidget::~CContinueWidget()
{
}

// �������֐�.
bool CContinueWidget::Init()
{
	// �X�v���C�g�̓ǂݍ���.
	if ( SpriteSetting() == false ) return false;
	// �J�[�\���̓ǂݍ���.
	if ( m_pCursor->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CContinueWidget::Update()
{
	// �J�[�\���̐ݒ�.
	if (m_pCursor == nullptr) return;
	CursorSetting();
}

// �`��֐�.
void CContinueWidget::Render()
{
	if ( m_pSprite.size() == 0 )return;

	//�w�i.
	m_pSprite[BACKGROUND]->SetBlend(true);
	m_pSprite[BACKGROUND]->SetAlpha( BACKGROUND_ALPHA );
	m_pSprite[BACKGROUND]->SetDeprh(false);
	m_pSprite[BACKGROUND]->RenderUI();
	m_pSprite[BACKGROUND]->SetDeprh(true);
	m_pSprite[BACKGROUND]->SetBlend(false);

	// �J�[�\��.
	m_pCursor->Render();

	// ����.
	for (size_t sprite = YES; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetDeprh(false);
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh(true);
	}
}

// �X�v���C�g�ݒ�֐�.
bool CContinueWidget::SpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	// �ǂݍ��ރX�v���C�g���̐ݒ�.
	const char* spriteName[] =
	{
		SPRITE_BACK,		//�w�i.
		SPRITE_YES,			//�C�G�X.
		SPRITE_NO,			//�m�[.
		SPRITE_CONTINUE,	//�R���e�j���[.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �X�v���C�g�̐ݒ�.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}

// �J�[�\���ݒ�֐�.
void CContinueWidget::CursorSetting()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_SelectState = CContinueWidget::ESelectState::Yes;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_SelectState = CContinueWidget::ESelectState::No;
	}

	switch (m_SelectState)
	{
	case CContinueWidget::ESelectState::Yes:
		m_pCursor->SetPosition( m_pSprite[YES]->GetRenderPos() );
		break;
	case CContinueWidget::ESelectState::No:
		m_pCursor->SetPosition( m_pSprite[NO]->GetRenderPos() );
		break;
	}
	m_pCursor->Update();
}
