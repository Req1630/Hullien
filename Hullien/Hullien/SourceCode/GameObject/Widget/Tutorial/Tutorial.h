#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include "..\Widget.h"

#include <vector>

class CTransition;
class CButtonExp;

class CTutorial : public CWidget
{
	const char* SPRITE_TUTORIAL_ONE_NAME	= "Tutorial1";	// �`���[�g���A���摜.
	const char* SPRITE_TUTORIAL_TWO_NAME	= "Tutorial2";	// �`���[�g���A���摜.
	const char* SPRITE_TUTORIAL_THREE_NAME	= "Tutorial3";	// �`���[�g���A���摜.
	const char* SPRITE_BACKGROUND_NAME		= "Fade";		// �w�i�摜.

	const char* SPRITE_ARROW_RIGHT			= "arrowRight";			// ���摜.
	const char* SPRITE_ARROW_LEFT			= "arrowLeft";			// ���摜.
	const char* SPRITE_SELECT_ARROW_RIGHT	= "selectArrowRight";	// �I����Ԃ̖��摜.
	const char* SPRITE_SELECT_ARROW_LEFT	= "selectArrowLeft";	// �I����Ԃ̖��摜.

	const float BACK_GROUND_ALPHA_SPEED		= 0.01f;	// �w�i�̃A���t�@���x.
	const float BACK_GROUND_ALPHA_MAX		= 0.4f;		// �w�i�̃A���t�@�ő�l.
	const float BACK_GROUND_ALPHA_MIN		= 0.0f;		// �w�i�̃A���t�@�ŏ��l.
	const float TRANSITION_SPEED			= 0.02f;	// �g�����W�V�����̑��x.
	const float TRANSITION_MAX				= 1.0f;		// �g�����W�V�����̍ő�l.
	const float TRANSITION_MIN				= 0.0f;		// �g�����W�V�����̍ŏ��l.

	const float ARROW_RENDER_ADJ_POS_X		= 5.0f;		// ���̕`����W�����l.

	const float ARROW_SCALE			= 1.0f;
	const float ARROW_ADJ_SCALE		= 0.5f;
	const float ARROW_SCALE_VALUE	= -0.2f;

	const float	INPUT_WAIT_TIME_MAX		= 30.0f;			// ���͑ҋ@�t���[��.

	const D3DXVECTOR2 BUTTON_EXP_RENDER_POS = { 650.0f, 600.0f };

	enum enTutorialState : unsigned char
	{
		ETutorialState_None,

		ETutorialState_One = 0,	// �ꖇ��.
		ETutorialState_Two,		// �񖇖�.
		ETutorialState_Three,	// �O����.

		ETutorialState_Max,

		ETutorialState_Start	= ETutorialState_One,	// �J�n�l.
		ETutorialState_End		= ETutorialState_Max,	// �I���l.

	} typedef ETutorialState;

	enum enArrowSpriteNo : char
	{
		EArrowSpriteNo_None = -1,

		EArrowSpriteNo_ArrowRight = 0,
		EArrowSpriteNo_ArrowLeft,

		EArrowSpriteNo_SelectArrowRight,
		EArrowSpriteNo_SelectArrowLeft,

		EArrowSpriteNo_Max,

		EArrowSpriteNo_Start	= EArrowSpriteNo_ArrowRight,		// �J�n�l.
		EArrowSpriteNo_End		= EArrowSpriteNo_SelectArrowLeft,	// �I���l.

	} typedef EArrowSpriteNo;

	struct stArrowParam
	{
		D3DXVECTOR3 vPos;	// ���W.
		float		Scale;	// �傫��.

		stArrowParam()
			: vPos	( 0.0f, 0.0f, 0.0f )
			, Scale	( 1.0f )
		{}
	} typedef SArrowParam;

public:
	CTutorial();
	virtual ~CTutorial();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �`���[�g���A�����J�n����.
	inline void SetupTutorial() { m_IsSetup = true; m_IsTutorialEnd = false; }
	// �`���[�g���A�����I��������.
	inline bool IsTutorialEnd() { return m_IsTutorialEnd; }

private:
	// ���̕`��.
	void ArrowRender();

	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::vector<std::shared_ptr<CSprite>>	m_pArrowSprites;
	std::vector<SArrowParam>				m_ArrowParams;
	std::unique_ptr<CTransition>			m_pTransition;
	std::unique_ptr<CButtonExp>				m_pButtonExp;
	int		m_NowSpriteNo;
	int		m_SelectMoveNo;
	float	m_InputWaitTime;
	float	m_BackGroundAlpha;
	float	m_TransitionValue;
	bool	m_IsSetup;
	bool	m_IsTutorialEnd;
};

#endif	// #ifndef TUTORIAL_WIDGET_H.