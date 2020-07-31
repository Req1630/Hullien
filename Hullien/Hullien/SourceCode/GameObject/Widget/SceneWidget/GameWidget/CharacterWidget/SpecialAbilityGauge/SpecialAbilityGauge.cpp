#include "SpecialAbilityGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*******************************************
*	����\�̓Q�[�W�N���X.
**/
CSpecialAbilityGauge::CSpecialAbilityGauge()
	: m_pSprite()
{
}

CSpecialAbilityGauge::~CSpecialAbilityGauge()
{
}

// �������֐�.
bool CSpecialAbilityGauge::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CSpecialAbilityGauge::Update()
{
	if (m_pSprite.size() == 0) return;
}

// �`��֐�.
void CSpecialAbilityGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	m_vPosition = D3DXVECTOR3(0.0f, 100.0f, 0.0f);

	for (const auto& s : m_pSprite)
	{
		s->SetPosition(m_vPosition);
		s->SetDeprh(false);
		s->RenderUI();
		s->SetDeprh(true);
	}
}

// �X�v���C�g�ݒ�֐�.
bool CSpecialAbilityGauge::SpriteSetting()
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
		if (m_pSprite[sprite] == nullptr) return false;
	}

	return true;
}
