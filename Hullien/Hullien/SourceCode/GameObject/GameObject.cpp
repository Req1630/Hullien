#include "GameObject.h"

/****************************************
*	�Q�[���I�u�W�F�N�g���N���X.
**/
CGameObject::CGameObject()
	: m_ObjectTag	( EObjectTag::None )
	, m_vPosition	( 0.0f, 0.0f, 0.0f )
	, m_vRotation	( 0.0f, 0.0f, 0.0f )
	, m_vSclae		( 1.0f, 1.0f, 1.0f )
{
}

CGameObject::~CGameObject()
{
}

//-----------------------------.
// �I�u�W�F�N�g�^�O�擾�֐�.
//-----------------------------.
EObjectTag CGameObject::GetObjectTag() const
{
	return m_ObjectTag;
}

//-----------------------------.
// ���W�擾�֐�.
//-----------------------------.
D3DXVECTOR3 CGameObject::GetPosition() const
{
	return m_vPosition;
}