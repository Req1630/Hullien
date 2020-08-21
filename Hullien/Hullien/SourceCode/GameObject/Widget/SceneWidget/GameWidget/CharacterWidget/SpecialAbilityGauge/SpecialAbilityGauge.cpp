#include "SpecialAbilityGauge.h"

#include "..\..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/*******************************************
*	����\�̓Q�[�W�N���X.
**/
CSpecialAbilityGauge::CSpecialAbilityGauge()
	: m_pSprite			()
	, m_GaugeState	()
	, m_Alpha			()
	, m_WaitTime		( 0 )
{
	m_FadeState = CWidget::EFadeState::In;
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
	m_GaugeState[GAUGE_NUM].vScale.x = m_Parameter.SpecialAbilityGauge / 10.0f; 

	// ����\�̓Q�[�W�����܂��Ă��Ȃ���Ώ������Ȃ�.
	if (m_GaugeState[GAUGE_NUM].vScale.x != SCALE_MAX) return;
	// ���点�鏈��.
	ShineBar();

}

// �`��֐�.
void CSpecialAbilityGauge::Render()
{
	if (m_pSprite.size() == 0) return;

	for (size_t sprite = 0; sprite < m_pSprite.size(); sprite++)
	{
		if (sprite == 2 && m_GaugeState[GAUGE_NUM].vScale.x != SCALE_MAX) continue;

		m_pSprite[sprite]->SetPosition(m_GaugeState[sprite].vPosition);
		m_pSprite[sprite]->SetScale(m_GaugeState[sprite].vScale);
		m_pSprite[sprite]->SetAnimNumber(m_GaugeState[sprite].AnimNum);
		m_pSprite[sprite]->SetAlpha(m_Alpha[sprite]);
		m_pSprite[sprite]->SetBlend( true );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
		m_pSprite[sprite]->SetBlend( false );
	}
}

// �X�v���C�g�ݒ�֐�.
bool CSpecialAbilityGauge::SpriteSetting()
{
	if (m_pSprite.size() != 0) return true;

	//�ǂݍ��ރX�v���C�g���ݒ�.
	const char* spriteName[] =
	{
		SPRITE_GAUGEBACK,	//�Q�[�W�w�i.
		SPRITE_GAUGE,		    //�Q�[�W.
		SPRITE_GAUGE,		    //�Q�[�W.
	};
	const int spriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	// �e���̐ݒ�.
	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back();
		m_pSprite[sprite] = CSpriteResource::GetSprite(spriteName[sprite]);
		m_GaugeState.emplace_back();
		m_GaugeState[sprite].vPosition.y = POS_Y;
		m_Alpha.emplace_back(ALPHA_MAX);
		if (m_pSprite[sprite] == nullptr) return false;
	}

	m_GaugeState[0].AnimNum = 1;
	m_GaugeState[1].AnimNum = 0;
	m_GaugeState[2].AnimNum = 2;
	m_Alpha[2] = 0.0f;

	return true;
}

// �o�[�����点�鏈��.
void CSpecialAbilityGauge::ShineBar()
{
	switch (m_FadeState)
	{
	case CWidget::EFadeState::In:
		CWidget::FadeIn(m_Alpha[2], FADE_IN_SPEED);
		if (m_Alpha[2] >= ALPHA_MAX) { CWidget::SetFadeOut(); }
		break;
	case CWidget::EFadeState::Out:
		CWidget::FadeOut(m_Alpha[2], FADE_OUT_SPEED);
		if (m_Alpha[2] <= 0.0f) { m_FadeState = CWidget::EFadeState::Finish; }
		break;
	case CWidget::EFadeState::Finish:
		m_WaitTime++;

		if (m_WaitTime >= WAITTIME_MAX)
		{
			m_WaitTime = 0;
			CWidget::SetFadeIn();
		}
		break;
	default:
		break;
	}
}
