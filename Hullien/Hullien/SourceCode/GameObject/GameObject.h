#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "..\Global.h"

#include "ObjectList.h"

/****************************************
*	�Q�[���I�u�W�F�N�g���N���X.
**/
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
	EObjectTag GetObjectTag() const;
	// ���W�擾�֐�.
	D3DXVECTOR3 GetPosition() const;
	// ���W�ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& vPos );

protected:
	EObjectTag	m_ObjectTag;	// �I�u�W�F�N�g�^�O.
	D3DXVECTOR3 m_vPosition;	// ���W.
	D3DXVECTOR3 m_vRotation;	// ��].
	D3DXVECTOR3 m_vSclae;		// �傫��.
};

#endif	// #ifndef GAME_OBJECT_H.