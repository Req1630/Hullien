#ifndef VOLUME_CONFIG_WIDGET_H
#define VOLUME_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include <vector>

class CSlinder;	// �X���C�_�[.
class CCursor;	// �J�[�\��.

/********************************************
*	���ʐݒ�UI�N���X.
**/
class CVolumeConfigWidget : public CWidget
{
	const char* SPRITE_MASTER_NAME	= "MasterVolume";
	const char* SPRITE_BGM_NAME		= "BGMVolume";
	const char* SPRITE_SE_NAME		= "SEVolume";

	const float BAR_POSITION_X = 200.0f;

	// �{�����[���̎��.
	enum enVolumeType
	{
		EVolumeType_None = -1,

		EVolumeType_Master,	// �}�X�^�[.
		EVolumeType_BGM,	// BGM.
		EVolumeType_SE,		// SE.

		EVolumeType_Max,

		EVolumeType_Begin	= EVolumeType_Master,	// �n�܂�.
		EVolumeType_End		= EVolumeType_Max,		// �I��.
	} typedef EVolumeType;

public:
	CVolumeConfigWidget();
	virtual ~CVolumeConfigWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// ���ʎ�ނ̑I��.
	void SelectVolumeType();
	// ���ʂ̐ݒ�.
	void VolumeSeting();
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::unique_ptr<CCursor>				m_pCursor;			// �J�[�\���N���X.
	std::vector<std::shared_ptr<CSlinder>>	m_pVolumeSlinders;	// ���ʃX���C�_�[.
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;			// �摜.
	std::vector<D3DXVECTOR3>				m_SlinderPositions;	// �X���C�_�[�̍��W.
	D3DXVECTOR3	m_CursorPosition;	// �J�[�\���̍��W.
	int			m_NowSelectVolume;	// ���݂̑I�����Ă��鉹�ʎ��.
};

#endif	// #ifndef VOLUME_CONFIG_WIDGET_H.