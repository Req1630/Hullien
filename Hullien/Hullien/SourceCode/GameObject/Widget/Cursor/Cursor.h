#ifndef CURSOR_H
#define  CURSOR_H

#include "..\Widget.h"

/********************************************
*	�J�[�\���N���X.
**/
class CCursor : public CWidget
{
private:
	const char* SPRITE_BLACK_NAME	= "selectBlack1";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_BLACK_NAME2	= "selectBlack2";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_BLUE_NAME	= "selectBlue1";	//�X�v���C�g�̃t�@�C����.
	const char* SPRITE_BLUE_NAME2	= "selectBlue2";	//�X�v���C�g�̃t�@�C����.
	const float SCALE_MAX	= 1.0f;			//�g��ő�l.
	const float SCALE_SPEED = 0.05f;		//�g�呬�x.
	const float ACC_SPEED   = 0.0011f;		//�����x.
	const float ADJ_POSITOIN_X_MAIN = 20.0f; 
	const float ADJ_POSITOIN_X_SIDE = -10.0f; 

public:
	CCursor();
	CCursor( const bool& isBlue );
	 virtual ~CCursor();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

	// �I�����鉡���̃T�C�Y�擾.
	void SetWidth( const float& width )
	{
		m_TargetWidth = width; 
		m_IsSetting = true;
	}

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �X�P�[�������֐�.
	void MoveScale();
	// �g��֐�.
	void IncreaseScale();

private:
	std::shared_ptr<CSprite>	m_pSelectSprite;		//�X�v���C�g�N���X.
	D3DXVECTOR3	m_SlectPosition;
	D3DXVECTOR3	m_vOldPosition;	//�ړ��O�̍��W.
	float		m_Acceleration;	//�����l.
	float		m_TargetWidth;
	bool		m_IsSetting;
	bool		m_IsBlueSelect;
};

#endif	//#ifndef CURSOR_H.