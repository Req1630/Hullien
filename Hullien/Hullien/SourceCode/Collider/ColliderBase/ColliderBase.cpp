#include "ColliderBase.h"

CColliderBase::CColliderBase()
	: m_pvPosition		( nullptr )
	, m_vAdjPosition	{ 0.0f, 0.0f, 0.0f }
	, m_pvRotation		( nullptr )
	, m_pScale			( nullptr )
{
}

CColliderBase::~CColliderBase()
{
}

// ���b�V���̒��_�̍ő�E�ŏ����擾����.
HRESULT CColliderBase::GetBoundingBox( LPD3DXMESH pInMesh, D3DXVECTOR3& vOutMax, D3DXVECTOR3& vOutMin )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	VOID* pVertices = nullptr;
	D3DXVECTOR3	Max, Min;

	if( FAILED( pInMesh->GetVertexBuffer( &pVB ) ) ){
		_ASSERT_EXPR( false, L"���_�o�b�t�@�擾���s" );
		return E_FAIL;
	}
	// ���b�V���̒��_�o�b�t�@�����b�N����.
	if( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		_ASSERT_EXPR( false, L"���_�o�b�t�@�̃��b�N���s" );
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}
	// ���b�V�����̒��_�ʒu�̍ő�ƍŏ�����������.
	D3DXComputeBoundingBox(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pInMesh->GetNumVertices(),
		D3DXGetFVFVertexSize( pInMesh->GetFVF() ),
		&Min, &Max );	// (out)�ŏ�,�ő咸�_.

	vOutMax = Max;
	vOutMin = Min;

	pVB->Unlock();		// �A�����b�N.
	SAFE_RELEASE( pVB );	// �g�p�ςݒ��_�o�b�t�@�̉��.

	return S_OK;
}

// ���b�V���̒��_�̍ő�E�ŏ����擾����.
HRESULT CColliderBase::GetBoundingSphere( LPD3DXMESH pInMesh, float& fOutRadius )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr; // ���_�o�b�t�@.
	void* pVertices = nullptr;	// ���_.
	D3DXVECTOR3 vCenter;		// ���S.

								// ���_�o�b�t�@���擾.
	if( FAILED( pInMesh->GetVertexBuffer( &pVB ) ) ){
		ERROR_MESSAGE("Failed to get mesh vertex buffer");
		return E_FAIL;
	}
	// ���b�V���̒��_�o�b�t�@�����b�N����.
	if( FAILED( pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		SAFE_RELEASE( pVB );
		ERROR_MESSAGE("Lock failure of mesh vertex buffer");
		return E_FAIL;
	}
	// ���b�V���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pInMesh->GetNumVertices(),				// ���_�̐�.
		D3DXGetFVFVertexSize( pInMesh->GetFVF() ),// ���_�̏��.
		&vCenter,								// (out)���S���W.
		&fOutRadius );							// (out)���a.

	// �A�����b�N.
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	return S_OK;
}