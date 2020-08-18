#ifndef ULTEMATESIGN_H
#define ULTEMATESIGN_H

#include "..\..\SceneWidget.h"

class CActor;

/*************************************************
*	�A���e�B���b�g�o���T�C���N���X.
**/
class CUltemateSing : public CSceneWidget
{
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
	void IsAppUltemate(CActor* pActor);

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	bool m_IsAppUltemate;	//�A���e�B���b�g���o�����Ă��邩.
};


#endif // #ifndef ULTEMATESIGN_H.