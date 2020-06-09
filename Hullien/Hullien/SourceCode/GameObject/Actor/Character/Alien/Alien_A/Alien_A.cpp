#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CAlienA::CAlienA()
{
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienA::~CAlienA()
{
}

// �������֐�.
bool CAlienA::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienA::Update()
{
	SetMoveVector( m_GirlPosition );
	if( GetAsyncKeyState('Q') & 0x8000 ){
		TargetRotation();
	}
} 

// �`��֐�.
void CAlienA::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += D3DX_PI;
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->Render();
}

// �ړ��֐�.
void CAlienA::Move()
{
}