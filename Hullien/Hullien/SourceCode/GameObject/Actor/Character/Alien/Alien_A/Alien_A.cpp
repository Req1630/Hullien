#include "Alien_A.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Arm\Arm.h"

#include "..\..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"

#define IS_TEMP_MODEL_RENDER

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
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pTempStaticMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic( m_pTempStaticMesh, MODEL_TEMP_NAME );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pTempStaticMesh == nullptr ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	if( m_pArm->Init() == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienA::Update()
{
	SetMoveVector( m_TargetPosition );
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V
	m_pArm->SetPosition( m_vPosition );
	m_pArm->SetRotationY( m_vRotation.y + static_cast<float>(D3DX_PI) );
	m_pArm->Update();
}

// �`��֐�.
void CAlienA::Render()
{
	// ��ʂ̊O�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;
	
	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetAnimSpeed( 0.01 );
	AlphaBlendSetting();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );
#else
	if( m_pTempStaticMesh == nullptr ) return;
	m_pTempStaticMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pTempStaticMesh->SetRotation( rot );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->SetColor( { 0.8f, 0.0f, 0.0f, m_ModelAlpha } );
	AlphaBlendSetting();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pTempStaticMesh->Render();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pTempStaticMesh->SetBlend( false );
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
	m_pArm->Render();
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

	GirlCollision( pActor );
	BarrierCollision( pActor );
}

// �X�|�[��.
bool CAlienA::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != EAlienState::None ) return true;
	m_Parameter = param;	// �p�����[�^��ݒ�.
	// �������Ɏ��s������I��.
	if( Init() == false ) return false;
	m_vPosition = spawnPos;	// �X�|�[�����W�̐ݒ�.
	m_LifePoint = m_Parameter.LifeMax;	// �̗͂̐ݒ�.
	m_vPosition.y += INIT_POSITION_ADJ_HEIGHT;
	m_NowState = EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.

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
#ifndef IS_TEMP_MODEL_RENDER
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
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}