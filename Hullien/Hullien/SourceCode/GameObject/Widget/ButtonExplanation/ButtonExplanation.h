#ifndef BUTTON_EXPLANATION_H
#define BUTTON_EXPLANATION_H

#include "..\Widget.h"

#include <vector>

class CButtonExp : public CWidget
{
	const char* SPRITE_L_STICK_NAME			= "stickL";			// L�{�^���̉摜.
	const char* SPRITE_SELECT_NAME			= "move";			// select�̉摜.
	const char* SPRITE_B_BUTTON_NAME		= "buttonB";		// B�{�^���̉摜.
	const char* SPRITE_DETERMINATION_NAME	= "determination";	// ����摜.
	const char* SPRITE_A_BUTTON_NAME		= "buttonA";		// A�{�^���̉摜.
	const char* SPRITE_CANCEL_NAME			= "cancel";			// �L�����Z���摜.
	const char* SPRITE_BACK_NAME			= "back";			// �L�����Z���摜.

	const float DEFAULT_SPRITE_RENDER_POS_X	= 950.0f;
	const float DEFAULT_SPRITE_RENDER_POS_Y	= 680.0f;

	const float L_STICK_SPRITE_RENDER_POS_X	= -70.0f;
	const float SELECT_SPRITE_RENDER_POS_X	= -40.0f;
	const float B_SPRITE_RENDER_POS_X		= 50.0f;
	const float DET_SPRITE_RENDER_POS_X		= 80.0f;
	const float A_SPRITE_RENDER_POS_X		= 170.0f;
	const float CANCEL_SPRITE_RENDER_POS_X	= 200.0f;

	const float SPRITE_RENDER_WIDTH	= A_SPRITE_RENDER_POS_X - B_SPRITE_RENDER_POS_X;

	const float SPRITE_RENDER_POS_X;
	const float SPRITE_RENDER_POS_Y;

	const float BUTTON_SPRITE_SIZE	= 0.4f;
	const float CHAR_SPRITE_SIZE	= 0.7f;

	// �X�v���C�g�ԍ�.
	enum enSpriteNo : char
	{
		ESpriteNo_None = -1,

		ESpriteNo_LStick,			// L�X�e�B�b�N.
		ESpriteNo_Select,			// �I��.
		ESpriteNo_B,				// B�{�^��.
		ESpriteNo_Determination,	// ����.
		ESpriteNo_A,				// A�{�^��.
		ESpriteNo_Cancel,			// �L�����Z��.

		ESpriteNo_Max,

		ESpriteNo_Begin = ESpriteNo_LStick,	// �J�n.
		ESpriteNo_End = ESpriteNo_Max,	// �I��.

	} typedef ESpriteNo;

public:
	CButtonExp();
	CButtonExp( const float& weight, const float& height );
	virtual~CButtonExp();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �L�����Z���̕�����߂�ɂ��邩�ǂ���.
	inline void SetBackString(){ m_IsBackString = true; }

	// �L�����Z���E�߂�̕�����`�悷�邩�ǂ���.
	inline void SetIsCancelRender( const bool& isRender ){ m_IsCancelStringRender = isRender; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;
	std::vector<D3DXVECTOR3>				m_PositionList;
	std::vector<float>						m_SpriteSizeList;
	bool									m_IsBackString;
	bool									m_IsCancelStringRender;
};

#endif	// #ifndef BUTTON_EXPLANATION_H.
