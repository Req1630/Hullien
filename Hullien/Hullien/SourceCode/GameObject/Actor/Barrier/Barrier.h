/**
* @file Barrier.h.
* @brief ���̎q�ɒ���o���A�N���X.
* @author ���c���.
*/
#ifndef BARRIER_H
#define BARRIER_H

#include "..\Actor.h"

class CEffectManager;	// �G�t�F�N�g�N���X.
class CEventActor;		// �C�x���g�A�N�^�[�N���X.

/********************************
*	�o���A�N���X.
**/
class CBarrier : public CActor
{
public:
	CBarrier();
	virtual ~CBarrier();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �G�t�F�N�g�`��֐�.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& pActor ) override;
	// �����Ԃ�.
	inline bool IsActive() const { return m_IsActive; }

private:
	// �����蔻��̐ݒ�.
	bool ColliderSetting();

private:
	std::shared_ptr<CEffectManager>	m_pEffect;		// �G�t�F�N�g.
	bool m_IsEffectPlay;		// �G�t�F�N�g���Đ����ꂽ��.
	bool m_IsActive;			// �����Ԃ�.
	float m_ActiveCount;		// ����J�E���g.
	float m_CollSphereRadius;	// �����蔻��̔��a.
#if _DEBUG
	float m_ResizeCollTime;		// �����蔻��̔��a�̃��T�C�Y���鎞��(�f�o�b�O�p).
#endif	// #if _DEBUG.
};

#endif	// #ifndef BARRIER_H.