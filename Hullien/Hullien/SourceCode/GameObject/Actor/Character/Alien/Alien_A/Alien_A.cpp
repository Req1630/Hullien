#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Arm\Arm.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CAlienA::CAlienA()
{
	m_ObjectTag = EObjectTag::Alien_A;
	m_pArm = std::make_unique<CArm>();
}

CAlienA::~CAlienA()
{
}

// �������֐�.
bool CAlienA::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	if( m_pArm->Init()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// �X�V�֐�.
void CAlienA::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );

	SetMoveVector( m_TargetPosition );	// �ړI�̍��W�̃x�N�g�����擾.
	CurrentStateUpdate();				// ���݂̏�Ԃ̍X�V.
	// �A�[��.
	m_pArm->SetPosition( {m_vPosition.x, m_vPosition.y+5.0f, m_vPosition.z} );		// ���W��ݒ�.
	m_pArm->SetRotationY( m_vRotation.y );	// ��]����ݒ�.
	m_pArm->Update();						// �X�V.
}

// �`��֐�.
void CAlienA::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;
	
	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.8f, 0.5f, 0.5f, 1.0f } );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render( m_pAC );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );

	m_pArm->Render();	// �A�[���̕`��.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CAlienA::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );	// ���̎q�Ƃ̓����蔻��.
	BarrierCollision( pActor );	// �o���A�Ƃ̓����蔻��.
}

// �X�|�[��.
bool CAlienA::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// �p�����[�^��ݒ�.
	// �������Ɏ��s������I��.
	if( Init() == false ) return false;
	m_vPosition			= spawnPos;					// �X�|�[�����W�̐ݒ�.
	m_LifePoint			= m_Parameter.LifeMax;		// �̗͂̐ݒ�.
	m_NowState			= EAlienState::Spawn;		// ���݂̏�Ԃ��X�|�[���ɕύX.
	m_AnimSpeed			= 0.0;						// �A�j���[�V�������x���~�߂�.
	return true;
}

// �X�|�[��.
void CAlienA::Spawning()
{
	CAlien::Spawning();
}

// �ړ�.
void CAlienA::Move()
{
	CAlien::Move();
}

// ����.
void CAlienA::Abduct()
{
	CAlien::Abduct();
}

// ����.
void CAlienA::Fright()
{
	CAlien::Fright();
}

// ���S.
void CAlienA::Death()
{
	CAlien::Death();
}

// ������.
void CAlienA::Escape()
{
	CAlien::Escape();
}

// �����蔻��̐ݒ�.
bool CAlienA::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
}