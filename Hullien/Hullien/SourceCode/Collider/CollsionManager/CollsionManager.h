#ifndef COLLSION_MANAGER_H
#define COLLSION_MANAGER_H

#include "..\..\Global.h"

class CSphereCollision;
class COBBoxCollision;
class CCapsuleCollision;

//----------------------------------.
//	�����蔻��}�l�[�W���[�N���X.
//	 �e�I�u�W�F�N�g�͂��̃N���X���g�p����.
//----------------------------------.
class CCollisionManager
{
public: //------ �֐� -------.
	CCollisionManager();
	~CCollisionManager();

	// ������.
	HRESULT Init( LPD3DXMESH pMesh, D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, float* pScale );
	// �X�t�B�A�̏�����.
	HRESULT InitSphere( 
		LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
		D3DXVECTOR3* pRot, float* pScale,
		const D3DXVECTOR3& adjPos, const float& adjRadius );
	// �{�b�N�X�̏�����.
	HRESULT InitBox( 
		LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
		D3DXVECTOR3* pRot, float* pScale,
		const D3DXVECTOR3& adjPos, const D3DXVECTOR3& adjlength );
	// �J�v�Z���̏�����.
	HRESULT InitCapsule( 
		LPD3DXMESH pMesh, D3DXVECTOR3* pPos, 
		D3DXVECTOR3* pRot, float* pScale,
		const float& adjRadius, const float& adjHeight );

	// �f�o�b�O�p�`��.
	void DebugRender();

	/*
	//- �����蔻��֐� -.
	*/

	// �X�t�B�A���m�̓����蔻��.
	bool IsShereToShere( CCollisionManager* pManager );
	// OBB���m�̓����蔻��.
	bool IsOBBToOBB( CCollisionManager* pManager );
	// �J�v�Z�����m�̓����蔻��.
	bool IsCapsuleToCapsule( CCollisionManager* pManager );

	/*
	//- �擾�֐� -.
	*/

	// �X�t�B�A�擾�֐�.
	CSphereCollision* GetSphere() const { return m_pSphere; }
	// �{�b�N�X�擾�֐�.
	COBBoxCollision* GetOBB() const { return m_pBox; }
	// �J�v�Z���擾�֐�.
	CCapsuleCollision* GetCapsule() const { return m_pCapsule; }

	/*
	//- �����p�֐� -.
	*/

	// �X�t�B�A�̒����p���W�ݒ�.
	void SetSphereAdjPosition( const D3DXVECTOR3& pos );
	// �X�t�B�A�̒����p���a�ݒ�.
	void SetSphereAdjRadius( const float& radius );
	// �{�b�N�X�̒����p���W�ݒ�.
	void SetBoxAdjPosition( const D3DXVECTOR3& pos );
	// �{�b�N�X�̒����p�����ݒ�.
	void SetBoxAdjLength( const D3DXVECTOR3& length );

private:
	// �X�t�B�A�̕`��.
	void SphereRender();
	// �{�b�N�X�̕`��.
	void BoxRender();
	// �J�v�Z���̕`��.
	void CapsuleRender();

private: //------ �ϐ� -------.
	CSphereCollision*	m_pSphere;	// �X�t�B�A.
	COBBoxCollision*	m_pBox;		// �{�b�N�X.
	CCapsuleCollision*	m_pCapsule;	// �J�v�Z��.
};

#endif	// #ifndef COLLSION_MANAGER_H.