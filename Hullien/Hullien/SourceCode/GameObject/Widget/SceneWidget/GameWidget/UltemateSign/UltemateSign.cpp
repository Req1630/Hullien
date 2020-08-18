#include "UltemateSign.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\Actor.h"

/*************************************************
*	�A���e�B���b�g�o���T�C���N���X.
**/
CUltemateSing::CUltemateSing()
	: m_IsAppUltemate	( false )
{
}

CUltemateSing::~CUltemateSing()
{
}

// �������֐�.
bool CUltemateSing::Init()
{
	// �X�v���C�g�̐ݒ�.
	if (SpriteSetting() == false) return false;
	return true;
}

// �X�V�֐�.
void CUltemateSing::Update()
{
	if (m_IsAppUltemate == false) return;
}

// �`��֐�.
void CUltemateSing::Render()
{
	if (m_IsAppUltemate == false) return;
	m_pSprite->SetDeprh(false);
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh(true);
}

// �A���e�B���b�g���o�����Ă��邩.
void CUltemateSing::IsAppUltemate(CActor * pActor)
{
	// �A���e�B���b�g�łȂ���Ώ������Ȃ�.
	if (pActor->GetObjectTag() != EObjectTag::Alien_D) return;
	// �o���t���O�𗧂Ă�.
	m_IsAppUltemate = true;
}

// �X�v���C�g�ݒ�֐�.
bool CUltemateSing::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite("ultemate signsize");
	if (m_pSprite == nullptr) return false;

	return true;
}
