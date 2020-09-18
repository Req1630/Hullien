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
public:
	CEventBase();
	virtual ~CEventBase();

	// �Ǎ��֐�.
	virtual bool Load() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;
	// �C�x���g�̏I�����擾.
	bool GetIsEventEnd() const { return m_IsEventEnd; };

protected:
	bool	m_IsEventEnd;	// �C�x���g���I��������.

private:
};

#endif	//#ifndef SCENE_EVENT_BASE_H.