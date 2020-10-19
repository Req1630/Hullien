#ifndef EVENT_BASE_H
#define EVENT_BASE_H

#include <Windows.h>
#include <string>
#include <stack>
#include <memory>

#include "..\..\Global.h"

/***********************************
*	�C�x���g�V�[�����N���X.
**/
class CEventBase
{
protected:
	const int   SKIP_WAIT_COUNT = 100;

public:
	CEventBase();
	virtual ~CEventBase();

	// �Ǎ��֐�.
	virtual bool Load() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;
	// �`��֐�.
	virtual void SpriteRender() = 0;
	// �C�x���g�̏I�����擾.
	bool GetIsEventEnd() const { return m_IsEventEnd; };

protected:
	// �ړI�n�ւ̂̈ړ�.
	bool MoveDestination(D3DXVECTOR3& vMyPosition, const D3DXVECTOR3& vDestination, const float& speed);

	// ���̃V�[���ɐi�߂�.
	virtual void NextStep() = 0;
	// �X�L�b�v.
	virtual void Skip() = 0;

protected:
	bool	m_IsEventEnd;		// �C�x���g���I��������.
	bool	m_IsSkip;			// �X�L�b�v������.
	int		m_SkipWaitCount;	//�X�L�b�v����܂ł̎���.

};

#endif	//#ifndef SCENE_EVENT_BASE_H.