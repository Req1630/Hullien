#ifndef TUTORIAL_WIDGET_H
#define TUTORIAL_WIDGET_H

#include "..\Widget.h"

#include <vector>

class CTransition;

class CTutorial : public CWidget
{
	enum enTutorialState : unsigned char
	{
		ETutorialState_None,

		ETutorialState_One = 0,
		ETutorialState_Two,
		ETutorialState_Three,

		ETutorialState_Max,

		ETutorialState_Start	= ETutorialState_One,
		ETutorialState_End		= ETutorialState_Max,

	} typedef ETutorialState;

public:
	CTutorial();
	virtual ~CTutorial();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// チュートリアルを開始する.
	inline void SetupTurorial() { m_IsSetup = true; m_IsTurorialEnd = false; }
	// チュートリアルが終了したか.
	inline bool IsTurorialEnd() { return m_IsTurorialEnd; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::unique_ptr<CTransition>			m_pTransition;
	int		m_NowSpriteNo;
	float	m_BackGroundAlpha;
	float	m_TransitionValue;
	bool	m_IsSetup;
	bool	m_IsTurorialEnd;
};

#endif	// #ifndef TUTORIAL_WIDGET_H.