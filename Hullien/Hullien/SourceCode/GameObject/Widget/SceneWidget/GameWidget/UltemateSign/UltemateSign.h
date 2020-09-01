#ifndef ULTEMATESIGN_H
#define ULTEMATESIGN_H

#include "..\..\SceneWidget.h"

class CGameActorManager;

/*************************************************
*	�A���e�B���b�g�o���T�C���N���X.
**/
class CUltemateSing : public CSceneWidget
{
private:
	const char* SPRITE_NAME = "ultemate signsize";	//�X�v���C�g�̖��O.
	const float DISPTIME_MAX = 250.0f;						//�ő�\������.
	const float ALPHA_SPEED = 0.03f;							//���ߑ��x.

public:
	CUltemateSing();
	virtual ~CUltemateSing();

	// �������֐�.
	virtual bool Init() override;

	// �X�V�֐�.
	virtual void Update() override;

	// �`��֐�.
	virtual void Render() override;

	// �A���e�B���b�g���o�����Ă��邩.
	void IsAppUltemate(CGameActorManager* pActor);

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	// �T�C���\���֐�.
	void DispSign();

private:
	bool m_IsAppUltemate;			// �A���e�B���b�g���o�����Ă��邩.
	unsigned int m_ObjCount;		// �I�u�W�F�N�g�̃J�E���g.
	float m_DispTime;					// �\������.
};


#endif // #ifndef ULTEMATESIGN_H.