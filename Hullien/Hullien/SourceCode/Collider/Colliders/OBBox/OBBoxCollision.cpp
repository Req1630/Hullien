#include "OBBoxCollision.h"
#include "BoxModel.h"

#ifdef _DEBUG
bool COBBoxCollision::m_IsRender = false;
#endif	// #ifdef _DEBUG.

COBBoxCollision::COBBoxCollision()
	: m_vDirection		()
	, m_vLength			{ 0.0f, 0.0f, 0.0f }
	, m_vAdjLength		{ 0.0f, 0.0f, 0.0f }
#ifdef _DEBUG
	, m_pDebugBox		( nullptr )
#endif	// #ifdef _DEBUG.
{
#ifdef _DEBUG
	m_pDebugBox	= std::make_unique<CBoxModel>();
#endif	// #ifdef _DEBUG.
}

COBBoxCollision::~COBBoxCollision()
{
}

//----------------------------------------.
// ���f���̃{�b�N�X�̍쐬.
//----------------------------------------.
HRESULT COBBoxCollision::InitModelBox( LPD3DXMESH pMesh )
{
	D3DXVECTOR3 Max, Min;
	GetBoundingBox( pMesh, Max, Min );

	m_vLength.x = ((fabsf( Max.x - Min.x ) * (*m_pScale)) / 2.0f) + m_vAdjLength.x;
	m_vLength.y = ((fabsf( Max.y - Min.y ) * (*m_pScale)) / 2.0f) + m_vAdjLength.y;
	m_vLength.z = ((fabsf( Max.z - Min.z ) * (*m_pScale)) / 2.0f) + m_vAdjLength.z;
#ifdef _DEBUG
	m_pDebugBox->Init( m_vLength, m_vLength );
#endif	// #ifdef _DEBUG.
	return S_OK;
}

//----------------------------------------.
// ���f���̃{�b�N�X�̍쐬.
//----------------------------------------.
HRESULT COBBoxCollision::InitModelBox( const D3DXVECTOR3& length )
{
	m_vLength = ( length * (*m_pScale) / 2.0f ) + m_vAdjLength;
#ifdef _DEBUG
	m_pDebugBox->Init( m_vLength, m_vLength );
#endif	// #ifdef _DEBUG.
	return S_OK;
}


//----------------------------------------.
// �����蔻��̕\��.
//----------------------------------------.
void COBBoxCollision::DebugRender()
{
#ifdef _DEBUG
	if( GetAsyncKeyState(VK_F2) & 0x0001 )m_IsRender = !m_IsRender;
	if( m_IsRender == false ) return;
	m_pDebugBox->SetRotation( GetRotation() );
	m_pDebugBox->SetPosition( GetPosition() );
	m_pDebugBox->Render();
#endif	// #ifdef _DEBUG.
}

//----------------------------------------.
// �F��ς��邩�ǂ���.
//----------------------------------------.
void COBBoxCollision::SetChangeColor( const bool& changed )
{
#ifdef _DEBUG
	m_pDebugBox->ChangeColor( changed );
#endif	// #ifdef _DEBUG.
}

//----------------------------------------.
// �����x�N�g���擾�֐�.
//----------------------------------------.
D3DXVECTOR3 COBBoxCollision::GetDirection( const int& index )
{
	D3DXMATRIX mRot;
	D3DXVECTOR3 vRot = *m_pvRotation;
	D3DXMatrixRotationYawPitchRoll( &mRot, vRot.y, vRot.x, vRot.z );

	m_vDirection[0] = D3DXVECTOR3( mRot._11, mRot._12, mRot._13 );
	m_vDirection[1] = D3DXVECTOR3( mRot._21, mRot._22, mRot._23 );
	m_vDirection[2] = D3DXVECTOR3( mRot._31, mRot._32, mRot._33 );

	return m_vDirection[index];
}