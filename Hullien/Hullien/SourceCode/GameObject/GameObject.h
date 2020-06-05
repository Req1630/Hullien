#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "..\Global.h"

#include "ObjectList.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	// �������֐�.
	virtual bool Init() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;

	// �I�u�W�F�N�g�^�O�擾�֐�.
	EObjectTag GetObjectTag();
	// ���W�擾�֐�.
	D3DXVECTOR3 GetPosition();

protected:
	EObjectTag	m_ObjectTag;	// �I�u�W�F�N�g�^�O.
	D3DXVECTOR3 m_vPosition;	// ���W.
	D3DXVECTOR3 m_vRotation;	// ��].
	D3DXVECTOR3 m_vSclae;		// �傫��.
};

#endif	// #ifndef GAME_OBJECT_H.