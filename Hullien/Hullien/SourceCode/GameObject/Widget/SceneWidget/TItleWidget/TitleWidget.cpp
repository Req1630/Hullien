#include "TitleWidget.h"

#include "..\..\Cursor\Cursor.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	�^�C�g��UI���N���X.
**/
CTitleWidget::CTitleWidget()
	: m_pSprite		()
	, m_pCursor			( nullptr )
	, m_SelectState		( CTitleWidget::ESelectState::Start )
{
	m_pCursor = std::make_shared<CCursor>();
}

CTitleWidget::~CTitleWidget()
{
}

// �������֐�.
bool CTitleWidget::Init()
{
	// �X�v���C�g�̓ǂݍ���.
	if( SpriteSetting() == false ) return false;

	// �J�[�\���̓ǂݍ���.
	if( m_pCursor->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CTitleWidget::Update()
{
	// �J�[�\���̐ݒ�.
	if (m_pCursor == nullptr) return;
	CursorSetting();
}

// �`��֐�.
void CTitleWidget::Render()
{
	if (m_pSprite.size() == 0) return;

	//�w�i.
	m_pSprite[BACKGROUND]->SetDeprh(false);
	m_pSprite[BACKGROUND]->RenderUI();
	m_pSprite[BACKGROUND]->SetDeprh(true);

	// �J�[�\��.
	m_pCursor->Render();

	// ����.
	for (size_t sprite = START; sprite < m_pSprite.size(); sprite++)
	{
		m_pSprite[sprite]->SetDeprh(false);
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh(true);
	}
}

// �X�v���C�g�ǂݍ��݊֐�.
bool CTitleWidget::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_BACKGROUND,		//�w�i.
		SPRITE_SELECTSTART,		//�J�n.
		SPRITE_SELECTEXIT,			//�I��.
		SPRITE_TITLE,					//�^�C�g��.
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �������̍ő�l�ݒ�.
	m_pSprite.reserve(SpriteMax);
	for (int sprite = 0; sprite < SpriteMax; sprite++)
	{
		m_pSprite.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if (m_pSprite[sprite] == nullptr) return false;
	}

	return true;
}

// �J�[�\���ݒ�֐�.
void CTitleWidget::CursorSetting()
{
	if (GetAsyncKeyState( VK_UP ) & 0x8000)
	{
		m_SelectState = CTitleWidget::ESelectState::Start;
	}
	if (GetAsyncKeyState( VK_DOWN ) & 0x8000)
	{
		m_SelectState = CTitleWidget::ESelectState::End;
	}

	switch (m_SelectState)
	{
	case CTitleWidget::ESelectState::Start:
		m_pCursor->SetPosition( m_pSprite[START]->GetRenderPos() );
		break;
	case CTitleWidget::ESelectState::End:
		m_pCursor->SetPosition( m_pSprite[END]->GetRenderPos() );
		break;
	default:
		break;
	}
	m_pCursor->Update();

}
