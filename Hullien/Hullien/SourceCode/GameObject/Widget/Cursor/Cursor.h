#ifndef CURSOR_H
#define  CURSOR_H

#include "..\Widget.h"

/********************************************
*	�J�[�\���N���X.
**/
class CCursor : public CWidget
{
private:
	const char* SPRITE_NAME = "selectHoversize";	//�X�v���C�g�̃t�@�C����.
	const float SCALE_MAX	= 1.0f;						//�g��ő�l.
	const float SCALE_SPEED = 0.05f;						//�g�呬�x.
	const float ACC_SPEED   = 0.0011f;					//�����x.

public:
	CCursor();
	 virtual ~CCursor();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �X�P�[�������֐�.
	void MoveScale();
	// �g��֐�.
	void IncreaseScale();

private:
	D3DXVECTOR3	m_vOldPosition;	//�ړ��O�̍��W.
	float		m_Acceleration;	//�����l.
};

#endif	//#ifndef CURSOR_H.