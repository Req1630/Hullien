#include "Alien_B.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"

CAlienB::CAlienB()
	: m_vPlayerPos		( 0.0f, 0.0f, 0.0f )
	, m_HasAimPlayer	( false )
	, m_RotAcc				( 0.0f )
{
	m_ObjectTag = EObjectTag::Alien_B;
	m_vSclae = { 0.05f, 0.05f, 0.05f };
}

CAlienB::~CAlienB()
{
}

// �������֐�.
bool CAlienB::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienB::Update()
{
	SetMoveVector( m_TargetPosition );
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V.
}

// �`��֐�.
void CAlienB::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.8f, 0.5f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CAlienB::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
	PlayerCollison( pActor );		// �v���C���[�Ƃ̓����蔻��.
	GirlCollision( pActor );			// ���̎q�Ƃ̓����蔻��.
}

// �X�|�[��.
bool CAlienB::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != EAlienState::None ) return true;
	// �������Ɏ��s������I��.
	if( Init() == false ) return false;
	m_Parameter = param;	// �p�����[�^��ݒ�.
	m_vPosition = spawnPos;	// �X�|�[�����W�̐ݒ�.
	m_NowState = EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.
	return true;
}

// ������W�̐ݒ�.
void CAlienB::SetTargetPos( CActor& actor )
{
	// ���̎q�̍��W���擾.
	CAlien::SetTargetPos( actor );

	// �v���C���[����Ȃ���ΏI��.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_vPlayerPos = actor.GetPosition();	// �v���C���[�̍��W���擾.
}

// �X�|�[��.
void CAlienB::Spawning()
{
	CAlien::Spawning();
}

// �ړ�.
void CAlienB::Move()
{
	AimPlayerDecision();	// �v���C���[��_��������.
	TargetRotation();			// ��].
	if( m_HasAimPlayer == true ){
		CAlienB::VectorMove( m_Parameter.MoveSpeed );		// �ړ�.
		Attack();						// �U��.
		CAlien::WaitMove();		// �ҋ@.
	} else {
		CAlien::VectorMove( m_Parameter.MoveSpeed );		// �ړ�.
		CAlien::WaitMove();		// �ҋ@.
	}

	if (*m_pIsAlienOtherAbduct == false) return;
	if (m_NowState == EAlienState::Abduct) return;
	m_NowState = EAlienState::Escape;
	m_NowMoveState = EMoveState::Rotation;	// �ړ���Ԃ���]����.
}

// ����.
void CAlienB::Abduct()
{
	CAlien::Abduct();
}

// ����.
void CAlienB::Fright()
{
	CAlien::Fright();
}

// ���S.
void CAlienB::Death()
{
	CAlien::Death();
}

// ������.
void CAlienB::Escape()
{
	CAlien::Escape();
}

// �ړ��֐�.
void CAlienB::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	if( lenght >= 1.0f ) return;

	m_NowMoveState = EMoveState::Attack;
	m_RotAcc = 0.0f;
}

// �U���֐�.
void CAlienB::Attack()
{
	if( m_NowMoveState != EMoveState::Attack ) return;

	const float power = 1.0f;
	static float pow = 0.99f;
	m_vRotation.y += (power - pow);
	pow -= 0.01f;
//	m_RotAcc += power - pow;

	if( pow > -power ) return;
	m_NowMoveState = EMoveState::Wait;
}

// �v���C���[�Ƃ̓����蔻��.
void CAlienB::PlayerCollison( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O���v���C���[����Ȃ���ΏI��.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowMoveState != EMoveState::Attack ) return;	// �U����Ԃ���Ȃ���ΏI��.
	if( m_NowState == EAlienState::Death ) return;	// ���S���Ă�����I��.
	if( m_NowState == EAlienState::Fright ) return;	// ���ݏ�ԂȂ�I��.

	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
//	pActor->LifeCalculation();
}

// �v���C���[��_��������.
void CAlienB::AimPlayerDecision()
{
	if( m_NowMoveState == EMoveState::Attack ) return;

	// �v���C���[�Ƃ̋������v�Z.
	float playerLenght = D3DXVec3Length( &D3DXVECTOR3(m_vPlayerPos - m_vPosition) );
	// ���̎q�Ƃ̋������v�Z.
	float girlLenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	if( playerLenght <= girlLenght ){
		// �v���C���[�̂ق����߂��̂Ńv���C���[��_��.
		m_HasAimPlayer = true;
		SetMoveVector( m_vPlayerPos );
	} else {
		// ���̎q�̂ق���_��.
		m_HasAimPlayer = false;
		SetMoveVector( m_TargetPosition );
	}
}

// �����蔻��̐ݒ�.
bool CAlienB::ColliderSetting()
{
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
}