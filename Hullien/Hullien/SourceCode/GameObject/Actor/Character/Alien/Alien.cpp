#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Arm\Arm.h"

#define IS_TEMP_MODEL_RENDER

CAlien::CAlien()
	: m_pArm					( nullptr )
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter				()
	, m_NowState				( EAlienState::None )
	, m_NowMoveState			( EMoveState::None )
	, m_HasAnyItem				( EItemList::None )
	, m_LifePoint				( 0.0f )
	, m_ModelAlpha				( 0.0f )
	, m_WaitCount				( 0 )
	, m_pIsAlienOtherAbduct		( nullptr )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
	, m_IsRisingMotherShip		( false )
{
}

CAlien::~CAlien()
{
}

// ������W�̐ݒ�.
void CAlien::SetTargetPos( CActor& actor )
{
	SetGirlPos( actor );
}

// ���C�t�v�Z�֐�.
void CAlien::LifeCalculation( const std::function<void(float&)>& proc )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	if( m_NowState == EAlienState::Fright ) return;

	proc( m_LifePoint );
	m_NowState = EAlienState::Fright;
	if( m_LifePoint > 0.0f ) return;
	m_NowState = EAlienState::Death;
}

// ���݂̏�Ԃ̍X�V�֐�.
void CAlien::CurrentStateUpdate()
{
	switch( m_NowState )
	{
	case EAlienState::Spawn:
		this->Spawning();
		break;
	case EAlienState::Move:
		this->Move();
		break;
	case EAlienState::Abduct:
		this->Abduct();
		break;
	case EAlienState::Fright:
		this->Fright();
		break;
	case EAlienState::Death:
		this->Death();
		break;
	case EAlienState::Escape:
		this->Escape();
		break;
	default:
		break;
	}
}

// ���̎q�̍��W��ݒ�.
void CAlien::SetGirlPos( CActor& actor )
{
	if( m_NowMoveState == EMoveState::Move ) return;
	if( *m_pIsAlienOtherAbduct == true ) return;

	// ���̎q����Ȃ���ΏI��.
	if( actor.GetObjectTag() != EObjectTag::Girl ) return;
	m_TargetPosition = actor.GetPosition();	// ���̎q�̍��W���擾.
	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f( 
		m_TargetPosition.x - m_vPosition.x,
		m_TargetPosition.z - m_vPosition.z );

	// �ړ��p�x�N�g�����擾.
	m_TargetPosition.x -= sinf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
	m_TargetPosition.z -= cosf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
}

// ���W�ݒ�֐�.
void CAlien::SetPosition( const D3DXVECTOR3& vPos )
{
	if( *m_pIsAlienOtherAbduct == false ) return;
	m_vPosition = vPos;
	m_IsRisingMotherShip = true;
}

// �ړ��x�N�g���ݒ�֐�.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f( 
		targetPos.x - m_vPosition.x,
		targetPos.z - m_vPosition.z );

	// �ړ��p�x�N�g�����擾.
	m_MoveVector.x = sinf( m_TargetRotation.y );
	m_MoveVector.z = cosf( m_TargetRotation.y );
}

// �ړI�̍��W�։�].
void CAlien::TargetRotation()
{
	if( m_NowMoveState != EMoveState::Rotation ) return;

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// �ړI�̃x�N�g���ƁA�����̃x�N�g���̊O�ς����߂�.
	float cross = myVector.x * m_MoveVector.z-myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x+myVector.z * m_MoveVector.z;
	dot = acosf( dot /( myLenght * targetLenght ));

	// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// ���ς����e�͈͂Ȃ�.
	if( -TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN ){
		m_vRotation.y = m_TargetRotation.y;	// �^�[�Q�b�g�ւ̉�]�擾.
		// �ړ��p�x�N�g�����擾.
		m_MoveVector.x = sinf( m_vRotation.y );
		m_MoveVector.z = cosf( m_vRotation.y );
		m_BeforeMoveingPosition = m_vPosition;
		m_NowMoveState = EMoveState::Move;
	}
}

// �ړ��֐�.
void CAlien::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	float researchLengh = D3DXVec3Length( &D3DXVECTOR3(m_BeforeMoveingPosition - m_vPosition) );
	if( researchLengh >= m_Parameter.ResearchLenght ) m_NowMoveState = EMoveState::Rotation;

	if( lenght >= 1.0f ) return;

	m_NowMoveState = EMoveState::Wait;
}

// �ҋ@�֐�.
void CAlien::WaitMove()
{
	if( m_NowMoveState != EMoveState::Wait ) return;
	m_WaitCount++;	// �ҋ@�J�E���g���Z.
	if( m_WaitCount < m_Parameter.WaitTime*FPS ) return;
	m_NowMoveState = EMoveState::Rotation;	// �ړ���Ԃ���]����.
	m_WaitCount = 0;	// �ҋ@�J�E���g�̏�����.
}

// �X�|�[����.
void CAlien::Spawning()
{
	// ���f���̃A���t�@�l�𑫂��Ă���.
	m_ModelAlpha += m_Parameter.ModelAlphaAddValue;
	if( m_ModelAlpha < MODEL_ALPHA_MAX ) return;
	CSoundManager::NoMultipleSEPlay("AlienAppSE");
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// �ړ�.
void CAlien::Move()
{
	TargetRotation();			// ��].
	CAlien::VectorMove( m_MoveSpeed );	// �ړ�.
	CAlien::WaitMove();			// �ҋ@.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Escape;
	m_NowMoveState	= EMoveState::Rotation;	// �ړ���Ԃ���]����.
}

// ����.
void CAlien::Abduct()
{
	if( m_IsBarrierHit == true ) return;
	if( m_pArm == nullptr ) return;

	SetMoveVector( *m_pAbductUFOPosition );
	m_TargetPosition = *m_pAbductUFOPosition;

	TargetRotation();
	CAlien::VectorMove( m_MoveSpeed );

	if( *m_pIsAlienOtherAbduct == true ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// ����.
void CAlien::Fright()
{
	m_InvincibleCount++;
	if( IsInvincibleTime( m_Parameter.InvincibleTime+5 ) == false ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// ���S.
void CAlien::Death()
{
	m_ModelAlpha -= m_Parameter.ModelAlphaSubValue;
	if( m_ModelAlpha > 0.0f ) return;
	CSoundManager::PlaySE("AlienHit");
	m_IsDelete = true;
}

// ������.
void CAlien::Escape()
{
	if( m_IsBarrierHit == true ) return;

	SetMoveVector( *m_pAbductUFOPosition );
	m_TargetPosition = *m_pAbductUFOPosition;
	TargetRotation();
	CAlien::VectorMove( m_MoveSpeed );
	if( *m_pIsAlienOtherAbduct == true ) return;
	m_NowState = EAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// �A���t�@�u�����h�̐ݒ�.
void CAlien::AlphaBlendSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ){
		if( m_ModelAlpha < 1.0f ){
			m_pSkinMesh->SetBlend( false );
		}
	}
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::GBuffer ){
		m_pSkinMesh->SetBlend( true );
		if( m_ModelAlpha >= 1.0f ){
			m_pSkinMesh->SetBlend( false );
		}
	}
#else
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ){
		if( m_ModelAlpha < 1.0f ){
			m_pTempStaticMesh->SetBlend( false );
		}
	}
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::GBuffer ){
		m_pTempStaticMesh->SetBlend( true );
		if( m_ModelAlpha >= 1.0f ){
			m_pTempStaticMesh->SetBlend( false );
		}
	}
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
}

// ���̎q�Ƃ̓����蔻��.
void CAlien::GirlCollision( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if( pActor->GetObjectTag() != EObjectTag::Girl ) return;
	if( m_IsBarrierHit == true ) return;
	if( m_NowMoveState == EMoveState::Attack )	return;	// �U����Ԃ͏I��.
	if( m_NowState == EAlienState::Spawn )		return;	// �X�|�[����ԂȂ�I��.
	if( m_NowState == EAlienState::Death )		return;	// ���S���Ă�����I��.
	if( m_NowState == EAlienState::Fright )		return;	// ���ݏ�ԂȂ�I��.

	bool isAbduct = false;
	if( m_NowState == EAlienState::Abduct ){
		isAbduct = true;
		pActor->SetPosition( m_pArm->GetGrabPosition() );
		return;
	} else {
		if( *m_pIsAlienOtherAbduct == true ){
			if( m_pArm->IsCleanUp() == false ){
				m_pArm->SetCleanUp();
			}
			return;
		}
		isAbduct = true;
	}

	if( isAbduct == false ) return;
	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	
	if( m_pArm->IsGrab() == false ){
		m_pArm->SetAppearance();
		return;
	}
	pActor->SetPosition( m_pArm->GetGrabPosition() );

	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState = EAlienState::Abduct;
	m_NowMoveState = EMoveState::Rotation;
}

// �o���A�Ƃ̓����蔻��.
void CAlien::BarrierCollision( CActor* pActor )
{
	// �I�u�W�F�N�g�̃^�O�����̎q����Ȃ���ΏI��.
	if( pActor->GetObjectTag() != EObjectTag::Bariier ) return;
	const float moveSpeed = -2.0f;
	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ){
		m_MoveSpeed = m_Parameter.MoveSpeed;
		m_IsBarrierHit = false;
		
	} else {
		CSoundManager::PlaySE("BarrierHitSE");
		m_MoveSpeed = moveSpeed;
		m_IsBarrierHit = true;
		*m_pIsAlienOtherAbduct = false;
		m_NowState = EAlienState::Move;
		m_NowMoveState = EMoveState::Move;
	}
}