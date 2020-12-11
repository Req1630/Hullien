#ifndef SWITCH_H
#define SWITCH_H

#include "..\Widget.h"

#include <vector>

class CSwitch : public CWidget
{
	inline static const char* SPRITE_ON_NAME		= "on";			// �I���摜.
	inline static const char* SPRITE_OFF_NAME		= "off";		// �I�t�摜.
	inline static const char* SPRITE_CHOICE_NAME	= "Choice";		// �I���摜.
	inline static const char* SPRITE_ARROW_RIGHT	= "arrowRight";	// ���摜.
	inline static const char* SPRITE_ARROW_LEFT		= "arrowLeft";	// ���摜.
	inline static const float ADJ_POSITION_X		= 200.0f;		// x���W�����l.
	inline static const float ARROW_ADJ_POSITION_X	= 50.0f;		// x���W�����l.
	inline static const int	INPUT_WAIT_TIME_MAX		= 30;			// ���͑ҋ@�t���[��.
	inline static const int	CHANGE_FRAME			= INPUT_WAIT_TIME_MAX/2;
	inline static const float ARROW_SCALE			= 1.0f;
	inline static const float ARROW_ADJ_SCALE		= 0.5f;
	inline static const float ARROW_SCALE_VALUE		= -0.2f;

	enum enSpriteNo : unsigned char
	{
		ESpriteNo_None,

		ESpriteNo_On = 0,
		ESpriteNo_Off,
		ESpriteNo_Choice,

		ESpriteNo_ArrowRight,
		ESpriteNo_ArrowLeft,

		ESpriteNo_Max,

		ESpriteNo_Begin	= ESpriteNo_ArrowRight,
		ESpriteNo_End	= ESpriteNo_Max,

	} typedef ESpriteNo;

	struct stArrowParam
	{
		D3DXVECTOR3 vPos;	// ���W.
		float		Scale;	// �傫��.

		stArrowParam()
			: vPos	( 0.0f, 0.0f, 0.0f )
			, Scale	( 1.0f )
		{}
	} typedef SArrowParam;

	enum enArrowNo : char
	{
		EArrowNo_None = -1,

		EArrowNo_Right,
		EArrowNo_Left,

		EArrowNo_Max,
	} typedef EArrowNo;

public:
	CSwitch();
	virtual ~CSwitch();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	inline void SetValue( const bool& value ){ m_IsValue = value; }
	// on,off �̎擾(true, false).
	inline bool GetValue() const	{ return m_IsValue; }
	// ���݂̒l��ۑ����Ă���.
	inline void SetNowValue()		{ m_IsOldValue = m_IsValue; }
	// �ۑ������l���Z�b�g�����.
	inline void ReSetValue()		{ m_IsValue = m_IsOldValue; }

	// ���W�ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

private:
	// �X�v���C�g�̐ݒ�֐�.
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::vector<SArrowParam>				m_ArrowParams;
	int			m_InputWaitTime;
	EArrowNo	m_SelectNo;
	bool		m_IsActive;
	bool		m_IsValue;
	bool		m_IsOldValue;
};

#endif	// #ifndef SWITCH_H.