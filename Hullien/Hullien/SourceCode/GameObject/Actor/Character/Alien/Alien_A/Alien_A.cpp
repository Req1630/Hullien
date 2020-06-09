#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CAlienA::CAlienA()
{
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienA::~CAlienA()
{
}

// 初期化関数.
bool CAlienA::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	return true;
}

// 更新関数.
void CAlienA::Update()
{
	SetMoveVector( m_GirlPosition );
	if( GetAsyncKeyState('Q') & 0x8000 ){
		TargetRotation();
	}
} 

// 描画関数.
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

// 移動関数.
void CAlienA::Move()
{
}