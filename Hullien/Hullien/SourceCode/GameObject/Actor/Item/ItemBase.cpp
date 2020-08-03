#include "ItemBase.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

CItemBase::CItemBase()
	: m_pStaticMesh			( nullptr )
	, m_NowState			( ENowState::None )
	, m_Scale				( 0.0f )
	, m_ModelAlpha			( INIT_MODEL_ALPHA )
	, m_AccelerationValue	( DEFAULT_ACCELERATION_VALUE )
	, m_Gravity				( DEFAULT_GRAVITY )
	, m_BoundCount			( 0 )
	, m_ActiveCount			( 0 )
	, m_FlashingCount		( 0.0f )
	, m_DisappearCount		( 0.0f )
	, m_FlashingAccValue	( 0.0f )
	, pPRAMETER				( nullptr )
{
	m_vSclae = { 1.0f, 1.0f, 1.0f };
}

CItemBase::CItemBase( const SParameter* pParam )
	: m_pStaticMesh			( nullptr )
	, m_NowState			( ENowState::None )
	, m_Scale				( 0.0f )
	, m_ModelAlpha			( INIT_MODEL_ALPHA )
	, m_AccelerationValue	( pParam->InitAccelerationValue )
	, m_Gravity				( pParam->InitGravity )
	, m_BoundCount			( 0 )
	, m_ActiveCount			( 0 )
	, m_FlashingCount		( 0.0f )
	, m_DisappearCount		( 0.0f )
	, m_FlashingAccValue	( 0.0f )
	, pPRAMETER				( pParam )
{
	m_vSclae = { pPRAMETER->ModelScaleMax, pPRAMETER->ModelScaleMax, pPRAMETER->ModelScaleMax };
}

CItemBase::~CItemBase()
{
}

// �X�V�֐�.
void CItemBase::Update()
{
	switch( m_NowState )
	{
	case ENowState::Drop:
		Drop();
		break;
	case ENowState::Active:
		Active();
		break;
	case ENowState::HitDisappear:
		HitDisappear();
		break;
	case ENowState::TimeDisappear:
		TimeDisappear();
		break;
	case ENowState::Delete:
		break;
	default:
		break;
	}
}

// �`��֐�.
void CItemBase::Render()
{
	if( m_NowState == ENowState::Delete ) return;
	if( m_NowState == ENowState::None ) return;
	if( m_NowState == ENowState::Max ) return;
	if( m_ModelAlpha < 1.0f ) return;

	if( m_pStaticMesh == nullptr ) return;
	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetRotation( m_vRotation );
	m_pStaticMesh->SetScale( m_vSclae );
	m_pStaticMesh->SetAlpha( m_ModelAlpha );
	m_pStaticMesh->SetBlend( true );
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pStaticMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CItemBase::Collision( CActor* pActor )
{
	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if( pActor->GetObjectTag() != EObjectTag::Player ) return;
	if( m_NowState == ENowState::Delete ) return;
	if( m_NowState == ENowState::None ) return;
	if( m_NowState == ENowState::HitDisappear ) return;
	if( m_NowState == ENowState::Drop ) return;

	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// ���������ۂ̌��ʂ�^����.
	this->GiveEffect( pActor );

	m_NowState = ENowState::HitDisappear;	// ���݂̏�Ԃ𓖂��������ɏ�����ԂɕύX.
}

// �o������.
void CItemBase::Drop( const D3DXVECTOR3& vPos )
{
	this->Init();
	m_NowState = ENowState::Drop;
	this->m_vPosition = vPos;
}

// �o��.
void CItemBase::Drop()
{
	// ���f���T�C�Y��傫������.
	if( m_Scale <= pPRAMETER->ModelScaleMax ){
		m_Scale += pPRAMETER->ModelScaleAddValue;
		m_vSclae = { m_Scale, m_Scale, m_Scale };
	}

	m_AccelerationValue += m_Gravity;		// �d�͂̉��Z.
	m_vPosition.y -= m_AccelerationValue;	// ���W�����Z�l�ň���.

	// ������0.0f���Ⴍ�Ȃ����ꍇ.
	if( m_vPosition.y <= 0.0f ){
		m_AccelerationValue = -m_AccelerationValue;	// ���Z�l�𔽓]����.
		m_Gravity += m_Gravity;	// �d�͂����Z����.
		m_BoundCount++;		// �o�E���h�̉񐔂����Z����.

		if( m_BoundCount <= pPRAMETER->BoundCountMax ) return;
		// �o�E���h�����o�E���h�ő吔�ȏ�Ȃ�.
		m_NowState = ENowState::Active;	// ���݂̏�Ԃ��A�N�e�B�u�ɕύX.
	}
}

// �A�N�e�B�u.
void CItemBase::Active()
{
	// ��]����.
	Rotate();

	m_ActiveCount++;	// �A�N�e�B�u�J�E���g�̉��Z.
	if( m_ActiveCount < pPRAMETER->ActiveTime*FPS ) return;
	m_NowState = ENowState::TimeDisappear;	// ���݂̏�Ԃ����Ԍo�߂ŏ�����ԂɕύX.
}

// ���������ۂɏ����鏈��.
void CItemBase::HitDisappear()
{
	m_NowState = ENowState::Delete;	// ���݂̏�Ԃ�������ԂɕύX.
}

// ���Ԃ��o�������ɏ�����.
void CItemBase::TimeDisappear()
{
	// ��]����.
	Rotate();

	m_FlashingCount += m_FlashingAccValue;		// �_�ŃJ�E���g�����Z.
	m_FlashingAccValue += pPRAMETER->FlashingAddValue;	// �_�ŉ��Z�l�����Z.
	m_ModelAlpha = sinf( m_FlashingCount ) + pPRAMETER->ModelAlphaMax;

	m_DisappearCount++;
	if( m_DisappearCount < pPRAMETER->DisappearTime*FPS ) return;
	m_ModelAlpha = 0.0f;
	m_NowState = ENowState::Delete;	// ���݂̏�Ԃ�������ԂɕύX.
}

// ��]�֐�.
void CItemBase::Rotate()
{
	// ��]����.
	m_vRotation.y += pPRAMETER->RotationSpeed;
	// �I�[�o�[�t���[�΍�.
	if( m_vRotation.y >= D3DX_PI*2.0 ) m_vRotation.y = 0.0f;
}

// ���f���̎擾.
bool CItemBase::GetModel( const char* modelName )
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pStaticMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetStatic( m_pStaticMesh, modelName );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pStaticMesh == nullptr ) return false;

	return true;
}

// �����蔻��̐ݒ�.
bool CItemBase::ColliderSetting()
{
	if( m_pStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f, 0.0f, 0.0f },
		0.0f ) )) return false;
	return true;
}