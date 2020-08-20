#ifndef WARNING_H
#define WARNING_H

#include "..\..\SceneWidget.h"

class CGirl;
class CCameraManager;

/*************************************
*	�x���N���X.
**/
class CWarning : public CSceneWidget
{
private:
	const char* SPRITE_NAME = "dangersignsize";	//�X�v���C�g��.
	const float ADJ_POS_Y = 6.0f;							//y���W����.

public:
	CWarning();
	virtual ~CWarning();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	//�`��֐�.
	virtual void Render() override;
	// ���̎q�̏�Ԑݒ�֐�.
	void SetGirlState(CGirl* pGirl);

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	D3DXVECTOR3 m_vTargetPos;	//�^�[�Q�b�g�ʒu.
	D3DXVECTOR3 m_vTargetRot;	//�^�[�Q�b�g��]�u.
	bool m_IsDisp;				//�\�����邩.
	bool m_IsUI;				//UI�������łȂ���.

	std::shared_ptr<CCameraManager>		m_pCamera;			// �J����.

};


#endif	//#ifndef WARNING_H.