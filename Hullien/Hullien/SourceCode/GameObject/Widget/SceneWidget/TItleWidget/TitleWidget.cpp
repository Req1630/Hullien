#include "TitleWidget.h"
#include "..\..\Cursor\Cursor.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/************************************
*	�^�C�g��UI���N���X.
**/
CTitleWidget::CTitleWidget()
	: m_pSprites		()
	, m_pCursor			( nullptr )
	, m_SelectState	( CTitleWidget::ESelectState::Start )
{
	m_pCursor = std::make_unique<CCursor>();
}

CTitleWidget::~CTitleWidget()
{
}

// �������֐�.
bool CTitleWidget::Init()
{
	// �X�v���C�g�̓ǂݍ���.
	const char* SpriteName[] =
	{	
		"BackGround",
		"SelectStart",
		"SelectExit",
		"Title",
	};
	int SpriteMax = sizeof(SpriteName) / sizeof(SpriteName[0]);

	m_pSprites.reserve(SpriteMax);
	for (int sprite = 0; sprite < SpriteMax; sprite++)
	{
		m_pSprites.emplace_back(CSpriteResource::GetSprite(SpriteName[sprite]));
		if (m_pSprites[sprite] == nullptr) return false;
	}

	// �J�[�\���̓ǂݍ���.
	if(m_pCursor->Init() == false) return false;

	return true;
}

// �X�V�֐�.
void CTitleWidget::Update()
{
	// �J�[�\���̐ݒ�.
	SettingCursor();
}

// �`��֐�.
void CTitleWidget::Render()
{
	//�w�i.
	m_pSprites[BACKGROUND]->SetDeprh(false);
	m_pSprites[BACKGROUND]->RenderUI();
	m_pSprites[BACKGROUND]->SetDeprh(true);

	// �J�[�\��.
	m_pCursor->Render();

	// ����.
	for (size_t sprite = START; sprite < m_pSprites.size(); sprite++)
	{
		m_pSprites[sprite]->SetBlend(true);
		m_pSprites[sprite]->SetDeprh(false);
		m_pSprites[sprite]->RenderUI();
		m_pSprites[sprite]->SetDeprh(true);
	}
}

// �J�[�\���ݒ�֐�.
void CTitleWidget::SettingCursor()
{
	if (GetAsyncKeyState( VK_UP ) & 0x0001)
	{
		m_SelectState = CTitleWidget::ESelectState::Start;
	}
	if (GetAsyncKeyState( VK_DOWN ) & 0x0001)
	{
		m_SelectState = CTitleWidget::ESelectState::End;
	}

	switch (m_SelectState)
	{
	case CTitleWidget::ESelectState::Start:
		m_pCursor->SetPosition(m_pSprites[START]->GetRenderPos());
		break;
	case CTitleWidget::ESelectState::End:
		m_pCursor->SetPosition(m_pSprites[END]->GetRenderPos());
		break;
	default:
		break;
	}
	m_pCursor->Update();

}
