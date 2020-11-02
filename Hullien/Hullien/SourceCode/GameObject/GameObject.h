#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "..\Global.h"
#include "..\Common\Shader\ShadowMap\ShadowMap.h"
#include "ObjectList.h"

/****************************************
*	�Q�[���I�u�W�F�N�g���N���X.
**/
class CGameObject
{
	// �E�B���h�E�O�̒����l.
	inline static const float WND_OUT_ADJ_SIZE = 300.0f;
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
	// X���̉�]�l�̐ݒ�.
	void SetRotationX( const float& rotX );
	// Y���̉�]�l�̐ݒ�.
	void SetRotationY( const float& rotY );
	// Z���̉�]�l�̐ݒ�.
	void SetRotationZ( const float& rotZ );

	// ��ʂ̊O�ɏo�Ă��邩.
	bool IsDisplayOut( const float& adjSize = WND_OUT_ADJ_SIZE );

protected:
	EObjectTag	m_ObjectTag;	// �I�u�W�F�N�g�^�O.
	D3DXVECTOR3 m_vPosition;	// ���W.
	D3DXVECTOR3 m_vRotation;	// ��].
	D3DXVECTOR3 m_vScale;		// �傫��.
};

#endif	// #ifndef GAME_OBJECT_H.