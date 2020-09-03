#include "UltemateSign.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/*************************************************
*	�A���e�B���b�g�o���T�C���N���X.
**/
CUltemateSing::CUltemateSing()
	: m_IsAppUltemate	( false )
	, m_ObjCount			( 0 )
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
	// �T�C���\��.
	DispSign();
}

// �`��֐�.
void CUltemateSing::Render()
{
	if (m_IsAppUltemate == false) return;
	m_pSprite->SetAlpha(m_Alpha);

	m_pSprite->SetBlend( true );
	m_pSprite->SetDeprh( false );
	m_pSprite->RenderUI();
	m_pSprite->SetDeprh( true );
	m_pSprite->SetBlend( false );
}

// �A���e�B���b�g���o�����Ă��邩.
void CUltemateSing::IsAppUltemate(CGameActorManager* pActor)
{
	auto l = pActor->GetObjPositionList();

	// ���łɏ��������I�u�W�F�N�g���͖�������.
	if (m_ObjCount >= l.size()) return;
	for (auto a = l.begin() + m_ObjCount; a < l.end(); a++)
	{
		m_ObjCount++;
		// �A���e�B���b�g�łȂ���Ώ������Ȃ�.
		if (a->first != EObjectTag::Alien_D) continue;
		// �o���t���O�𗧂Ă�.
		m_IsAppUltemate = true;
	}
}

// �X�v���C�g�ݒ�֐�.
bool CUltemateSing::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite( SPRITE_NAME );
	if (m_pSprite == nullptr) return false;

	return true;
}

// �T�C���\���֐�.
void CUltemateSing::DispSign()
{
	if (m_Alpha < ALPHA_MAX && m_DispTime == 0)
	{
		// �t�F�[�h�C���̐ݒ�.
		if ( m_FadeState != EFadeState::In ) { SetFadeIn(ALPHA_SPEED); }
	}
	else
	{
		m_DispTime++;

		// �ő�\�����Ԃ𒴂��Ă��Ȃ���Ώ������Ȃ�.
		if (m_DispTime < DISPTIME_MAX) return;
		// �t�F�[�h�A�E�g�̐ݒ�.
		if (m_FadeState != EFadeState::Out) { SetFadeOut(ALPHA_SPEED); }

		// ���ߒl��0�Ȃ�Δ�\���ɂ���.
		if (m_Alpha < 0.0f)
		{
			m_DispTime = 0;
			m_IsAppUltemate = false;
		}
	}

	// �t�F�[�h�̍X�V.
	FadeUpdate(m_Alpha);
}