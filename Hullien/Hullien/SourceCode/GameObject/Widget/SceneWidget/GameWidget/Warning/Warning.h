#ifndef WARNING_H
#define WARNING_H

#include "..\GameWidget.h"
class CActor;
class CGirl;

/*************************************
*	�x���N���X.
**/
class CWarning : public CGameWidget
{
private:
	const char* SPRITE_NAME = "dangersignsize";

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
	// �v���C���[�ʒu�擾.
	void SetPlayerPos(CActor* pActor);

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	D3DXVECTOR3 m_vPlayePos;	//�v���C���[�ʒu.
	D3DXVECTOR3 m_vPlayeRot;	//�v���C���[�ʒu.
	D3DXVECTOR3 m_vTargetPos;	//�^�[�Q�b�g�ʒu.
	D3DXVECTOR3 m_vTargetRot;	//�^�[�Q�b�g��]�u.
	bool m_IsDisp;							//�\�����邩.
	bool m_IsUI;								//UI�������łȂ���.
};


#endif	//#ifndef WARNING_H.