#ifndef LIMITTIME_H
#define LIMITTIME_H

#include "..\..\SceneWidget.h"
#include <vector>

/*****************************************
*	�������ԃN���X.
**/
class CLimitTime : public CSceneWidget
{
private:
	const char* SPRITE_TIME				= "timeNumber";	//���Ԃ̃X�v���C�g��.
	const D3DXVECTOR3 ONETIME_INITPOS	= D3DXVECTOR3(660.0f, 30.0f, 0.0f);	//���Ԉ�̈ʏ����ʒu.
	const D3DXVECTOR3 TENTIME_INITPOS	= D3DXVECTOR3(630.0f, 30.0f, 0.0f);	//���ԏ\�̈ʏ����ʒu.
	const int LIMITTIME_MAX				= 60;	//�������ԍő�.
	const int FRAME_DIVISION			= 100;//�t���[�����鐔.
	const int ANIM_MAX					= 10;	//�A�j���[�V�����ő吔.
	const int TEN						= 10;	//�\.

public:
	CLimitTime();
	virtual ~CLimitTime();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �I���̎擾�֐�.
	bool IsFinish() const { return m_IsFinish; }	

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �ʒu�ݒ�֐�.
	bool PositionSetting();
	// �A�j���[�V�����ԍ��ݒ�֐�.
	bool AnimSetting();
	// �J�E���g�_�E���֐�.
	void CountDown();
	// �t���[������b�ɕϊ��֐�.
	void FramesToSeconds();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;		// �X�v���C�g�N���X.
	std::vector<D3DXVECTOR3>				m_vPosition;	// �ʒu.
	std::vector<int>						m_Anim;			// �A�j���[�V�����ԍ�.
	int										m_FrameCount;	// �t���[���J�E���g.
	int										m_Seconds;		// �b.
	bool									m_IsFinish;		// �I���������ǂ���.
};

#endif	//#ifndef LIMITTIME_H.