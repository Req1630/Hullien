#ifndef	CONFIG_CURSOR_H
#define	CONFIG_CURSOR_H

#include "..\Widget.h"

/********************************************
*	�J�[�\���N���X.
**/
class CConfigCursor : public CWidget
{
private:
	const char* SPRITE_NAME = "ConfigSelect";	//�X�v���C�g�̃t�@�C����.
	const float SCALE_MAX	= 1.0f;				//�g��ő�l.
	const float SCALE_SPEED = 0.05f;			//�g�呬�x.
	const float ACC_SPEED   = 0.0011f;			//�����x.
	const float ADJ_POSITOIN_X_MAIN = 70.0f;

public:
	CConfigCursor();
	virtual ~CConfigCursor();

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
	D3DXVECTOR3	m_vOldPosition;	//�ړ��O�̍��W.
	float		m_Acceleration;	//�����l.
	float		m_TargetWidth;
	float		m_ScaleCount;
	bool		m_IsSetting;
};

#endif	//#ifndef CONFIG_CURSOR_H.