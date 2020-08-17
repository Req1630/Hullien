#include "LifeGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\Actor\Character\Character.h"

/*************************************
*	���C�t�Q�[�W�N���X.
**/
CLifeGauge::CLifeGauge()
	: m_pSprite			()
	, m_GaugeState	()
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
	m_GaugeState[1].vScale.x = m_Parameter.Life / 10.0f;
}

// �`��֐�.
void CLifeGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	int sprite = 0;
	for (const auto& s : m_pSprite)
	{
		s->SetPosition( m_GaugeState[sprite].vPosition );
		s->SetScale(m_GaugeState[sprite].vScale );
		s->SetAnimNumber(m_GaugeState[sprite].AnimNum);
		s->SetDeprh(false);
		s->RenderUI();
		s->SetDeprh( true );
		sprite++;
	}

}

// �p�����[�^�ݒ�֐�.
void CLifeGauge::SetParameter(CCharacter& pChara)
{
}

// �X�v���C�g�ǂݍ��݊֐�.
bool CLifeGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//�ǂݍ��ރX�v���C�g���ݒ�.
	const char* spriteName[] =
	{
		SPRITE_GAGEBACK,	//�Q�[�W�w�i.
		SPRITE_GAGE,		    //�Q�[�W.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite( spriteName[sprite] );
		m_GaugeState.emplace_back();
		m_GaugeState[sprite].AnimNum = 1- sprite;
		if ( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
