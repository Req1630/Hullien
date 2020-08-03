#include "LifeGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*************************************
*	���C�t�Q�[�W�N���X.
**/
CLifeGauge::CLifeGauge()
	: m_pSprite ()
{
}

CLifeGauge::~CLifeGauge()
{
}

// �������֐�.
bool CLifeGauge::Init()
{
	// �X�v���C�g�ǂݍ���.
	if (SpriteSetting() == false) return false;

	return true;
}

// �X�V�֐�.
void CLifeGauge::Update()
{
	if ( m_pSprite.size() == 0 ) return;
}

// �`��֐�.
void CLifeGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (const auto& s : m_pSprite)
	{
		s->SetDeprh( false );
		s->SetPosition( m_vPosition );
		s->RenderUI();
		s->SetDeprh( true );
	}

}

// �X�v���C�g�ǂݍ��݊֐�.
bool CLifeGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//�ǂݍ��ރX�v���C�g���ݒ�.
	const char* spriteName[] =
	{
		SPRITE_GAGEBACK,	//�Q�[�W�w�i.
		SPRITE_GAGE,		//�Q�[�W.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite( spriteName[sprite] );
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
