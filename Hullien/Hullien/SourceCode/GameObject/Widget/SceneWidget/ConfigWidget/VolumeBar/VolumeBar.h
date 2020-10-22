#ifndef VOLUME_BAR_H
#define VOLUME_BAR_H

#include "..\..\..\Widget.h"
#include <vector>

class CVolumeBar : public CWidget
{
	const char* SPRITE_NAME = "VolumeBar";
public:
	CVolumeBar();
	virtual ~CVolumeBar();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �{�����[���̐ݒ�.
	void SetVolume( const float& volume ){ m_Volume = volume; }

	// ���݂̃J�[�\���̍��W�̎擾.
	float GetNowCursorPosition(){ return m_CursorPositionX; }
	// Y���W�̐ݒ�.
	void SetPositionY( const float& posY ){ m_vPosition.y = posY; }

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::shared_ptr<CSprite>	m_pSprite;	// �X�v���C�g�N���X.
	float	m_CursorPositionX;
	float	m_ImageWidthSize;
	float	m_Volume;
};

#endif	// #ifndef VOLUME_BAR_H.