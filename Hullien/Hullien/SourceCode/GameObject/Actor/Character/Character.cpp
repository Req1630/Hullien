#include "Character.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\XAudio2\SoundManager.h"	

CCharacter::CCharacter()
	: m_pSkinMesh				( nullptr )
#ifdef IS_TEMP_MODEL_RENDER
	, m_pTempStaticMesh			( nullptr )
#endif
	, m_MoveVector				( 0.0f, 0.0f, 0.0f )
	, m_InvincibleCount			( 0 )
	, m_HasFinishedParamSetting	( false )
	, m_pFootCollision			()
	, m_pGroundCollision		( nullptr )
	, m_vGroundPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vRightPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vLeftPosition			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{
	m_vPosition.y = INIT_POSITION_ADJ_HEIGHT;
//	m_vSclae = { 0.1f, 0.1f, 0.1f };
}

CCharacter::~CCharacter()
{
}

// ���b�V���̕\��.
void CCharacter::MeshRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( 0.01 );
	m_pSkinMesh->Render();
#else
	if( m_pTempStaticMesh == nullptr ) return;

	m_pTempStaticMesh->SetPosition( m_vPosition );
	m_pTempStaticMesh->SetRotation( m_vRotation );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->Render();
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// ���G���Ԃ��ǂ���.
bool CCharacter::IsInvincibleTime( const int& invincibleTime )
{
	return m_InvincibleCount > invincibleTime * FPS;
}

// ���f���̎擾.
bool CCharacter::GetModel( const char* modelName )
{
#ifndef IS_TEMP_MODEL_RENDER
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pSkinMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetSkin( m_pSkinMesh, modelName );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pSkinMesh == nullptr ) return false;
	return true;
#else
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pTempStaticMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic( m_pTempStaticMesh, modelName );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pTempStaticMesh == nullptr ) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

void CCharacter::FootStep(const char * rightfoot, const char * leftfoot)
{
	m_vGroundPosition = m_vPosition;
	m_pSkinMesh->GetPosFromBone(leftfoot, &m_vLeftPosition);
	m_pSkinMesh->GetPosFromBone(rightfoot, &m_vRightPosition);

	if (m_pFootCollision[0]->IsShereToShere(m_pGroundCollision.get()) == true
		|| m_pFootCollision[1]->IsShereToShere(m_pGroundCollision.get()) == true)
	{
		CSoundManager::NoMultipleSEPlay("FooStept");
	}
}

bool CCharacter::FootStepCollisionSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	// �n�ʂ̓����蔻��.
	if (m_pGroundCollision != nullptr) return true;
	if (m_pGroundCollision == nullptr)
	{
		m_pGroundCollision = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pGroundCollision->InitSphere(
		&m_vGroundPosition,
		&m_vRotation,
		&m_vSclae.x,
		{0.0f,0.0f,0.0f},
		1.0f))) return false;

	// ���̓����蔻��.
	if (m_pFootCollision.size() != 0) return true;
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	m_pFootCollision.emplace_back(std::make_shared<CCollisionManager>());
	if (FAILED(m_pFootCollision[0]->InitSphere(
		&m_vRightPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
	if (FAILED(m_pFootCollision[1]->InitSphere(
		&m_vLeftPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f,0.0f,0.0f },
		0.5f))) return false;
#endif
	return true;
}
