#include "Alien_C.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

CAlienC::CAlienC()
{
	m_ObjectTag = EObjectTag::Alien_C;
	m_pArm = std::make_unique<CArm>();
}

CAlienC::~CAlienC()
{
}

// �������֐�.
bool CAlienC::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( ColliderSetting()			== false ) return false;
	if( m_pArm->Init()				== false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienC::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
	SetMoveVector( m_TargetPosition );	// �ړI�̃x�N�g�����擾.
	CurrentStateUpdate();				// ���݂̏�Ԃ̍X�V.
	// �A�[��.
	m_pArm->SetPosition( {m_vPosition.x, m_vPosition.y+5.5f, m_vPosition.z} );		// ���W��ݒ�.
	m_pArm->SetRotationY( m_vRotation.y );	// ��]�l��ݒ�.
	m_pArm->Update();						// �X�V.

	// �A���t�@�l��0��葽����ΏI��.
	if( m_IsDelete == false ) return;
	if( m_NowState == EAlienState::RisingMotherShip ) return;	// �}�U�[�V�b�v�ɏ����Ă����ԂȂ�I��.
	m_IsExplosion = true;	// ����������.
	CSoundManager::PlaySE("Bomb");
}

// �`��֐�.
void CAlienC::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, 1.0f } );
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
void CAlienC::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	GirlCollision( pActor );	// ���̎q�Ƃ̓����蔻��.
	BarrierCollision( pActor );	// �o���A�Ƃ̓����蔻��.
}

// �X�|�[��.
bool CAlienC::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// �p�����[�^��ݒ�.
	// �������Ɏ��s������I��.
	if( Init() == false ) return false;
	m_vPosition		= spawnPos;				// �X�|�[�����W�̐ݒ�.
	m_LifePoint		= m_Parameter.LifeMax;	// �̗͂̐ݒ�.
	m_NowState		= EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.
	m_AnimSpeed		= 0.0;					// �A�j���[�V�������x���~�߂�.
	return true;
}

// �X�|�[��.
void CAlienC::Spawning()
{
	CAlien::Spawning();
}

// �ړ�.
void CAlienC::Move()
{
	CAlien::Move();
}

// ����.
void CAlienC::Abduct()
{
	CAlien::Abduct();
}

// ����.
void CAlienC::Fright()
{
	CAlien::Fright();
}

// ���S.
void CAlienC::Death()
{
	CAlien::Death();
}

// ������.
void CAlienC::Escape()
{
	CAlien::Escape();
}

// �����蔻��̐ݒ�.
bool CAlienC::ColliderSetting()
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