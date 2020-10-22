#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Arm\Arm.h"

CAlien::CAlien()
	: m_pArm					( nullptr )
	, m_TargetPosition			( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation			( 0.0f, 0.0f, 0.0f )
	, m_KnockBackVector			( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter				()
	, m_NowState				( EAlienState::None )
	, m_NowMoveState			( EMoveState::None )
	, m_HasAnyItem				( EItemList::None )
	, m_LifePoint				( 0.0f )
	, m_DeathScale				( 1.0f )
	, m_DeathCount				( 0.0f )
	, m_KnockBackCount			( 0 )
	, m_WaitCount				( 0 )
	, m_pIsAlienOtherAbduct		( nullptr )
	, m_IsExplosion				( false )
	, m_IsDelete				( false )
{
	m_vScale = { 0.0f, 0.0f, 0.0f };
	m_AnimFrameList.resize( EAnimNo_Max );
}

CAlien::~CAlien()
{
}

// ������W�̐ݒ�.
void CAlien::SetTargetPos( CActor& actor )
{
	SetGirlPos( actor );
}
// �x�N�g���̎擾.
void CAlien::SetVector( const D3DXVECTOR3& vec )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	m_KnockBackVector = vec;
	m_KnockBackCount = 0;
}

// ���C�t�v�Z�֐�.
void CAlien::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( m_NowState == EAlienState::Spawn ) return;
	if( m_NowState == EAlienState::Death ) return;
	if( m_NowState == EAlienState::Fright ) return;

	bool isAttack = false;
	proc( m_LifePoint, isAttack );
	m_NowState = EAlienState::Fright;	// ���ݏ�Ԃ֑J��.
	SetAnimation( EAnimNo_Damage, m_pAC );
	m_AnimSpeed = 0.01;

	// �A�[����Еt���Ă��Ȃ���ΕЕt����.
	if( m_pArm->IsCleanUp() == false ){
		m_pArm->SetCleanUp();
	}

	if( m_LifePoint > 0.0f ) return;
	// �̗͂� 0.0�ȉ��Ȃ玀�S��Ԃ֑J��.
	m_NowState = EAlienState::Death;
	SetAnimation( EAnimNo_Dead, m_pAC );
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
	case EAlienState::KnockBack:
		break;
	case EAlienState::Fright:
		this->KnockBack();
		this->Fright();
		break;
	case EAlienState::Death:
		this->Death();
		break;
	case EAlienState::Escape:
		this->Escape();
		break;
	case EAlienState::RisingMotherShip:
		this->RisingMotherShip();
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
		m_vPosition.x - m_TargetPosition.x,
		m_vPosition.z - m_TargetPosition.z );

	// ���̎q�ƈ��̋����u�������W��ݒ�.
	m_TargetPosition.x += sinf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
	m_TargetPosition.z += cosf( m_TargetRotation.y ) * CArm::GRAB_DISTANCE;
}

// ���W�ݒ�֐�.
void CAlien::SetPosition( const D3DXVECTOR3& vPos )
{
	if( *m_pIsAlienOtherAbduct == false ) return;
	m_NowState = EAlienState::RisingMotherShip;
	m_vPosition	= vPos;
}

// �ړ��x�N�g���ݒ�֐�.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	if( m_NowState == EAlienState::Fright ) return;

	// �ړI�̉�]�����擾.
	m_TargetRotation.y = atan2f( 
		m_vPosition.x - targetPos.x,
		m_vPosition.z - targetPos.z );

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

	// �ړI�̍��W�֌����ĉ�].
	if( CCharacter::TargetRotation( m_MoveVector, ROTATIONAL_SPEED, TOLERANCE_RADIAN ) == false ) return;
	m_vRotation.y			= m_TargetRotation.y;	// �^�[�Q�b�g�ւ̉�]�擾.
	m_BeforeMoveingPosition = m_vPosition;			// ���݂̍��W���L��.
	m_NowMoveState			= EMoveState::Move;		// �ړ���Ԃ֑J��.
}

// �ړ��֐�.
void CAlien::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;
	
	// �x�N�g�����g�p���Ĉړ�.
	m_vPosition.x -= m_MoveVector.x * moveSpeed;
	m_vPosition.z -= m_MoveVector.z * moveSpeed;

	// �ēx�ړI�̍��W��T������r.
	// ��]���ɋL���������W�ƌ��݂̍��W�̋��������ȏ�Ȃ�.
	if( D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition-m_vPosition)) >= m_Parameter.ResearchLenght ) 
		m_NowMoveState = EMoveState::Rotation;	// ��]��Ԃֈړ�.

	if( D3DXVec3Length(&D3DXVECTOR3(m_TargetPosition-m_vPosition)) >= 1.0f ) return;
	// ���݂̍��W�ƖړI�̍��W�̋��������ȏ�Ȃ�.
	m_NowMoveState = EMoveState::Wait;	// �ҋ@��Ԃ֑J��.
}

// �ҋ@�֐�.
void CAlien::WaitMove()
{
	if( m_NowMoveState != EMoveState::Wait ) return;
	m_WaitCount++;	// �ҋ@�J�E���g���Z.
	if( m_WaitCount < m_Parameter.WaitTime*FPS ) return;
	m_NowMoveState	= EMoveState::Rotation;	// �ړ���Ԃ���]����.
	m_WaitCount		= 0;	// �ҋ@�J�E���g�̏�����.
}

// �X�|�[����.
void CAlien::Spawning()
{
	// �X�P�[���̉��Z.
	m_vScale += { ADD_SCALE_VALUE, ADD_SCALE_VALUE, ADD_SCALE_VALUE };
	// �傫�������l�ȏ�Ȃ�.
	if( m_vScale.x >= SCALE_MAX ){
		m_vScale = { SCALE_MAX, SCALE_MAX, SCALE_MAX };
		m_vPosition.y -= DOWN_SPEED;	// ������������.
	}

	// ���������l���傫����ΏI��.
	if( m_vPosition.y > 0.0f ) return;

	CSoundManager::NoMultipleSEPlay("AlienApp");
	m_AnimSpeed	= 0.01;
	SetAnimation( EAnimNo_Move, m_pAC );
	m_NowState	= EAlienState::Move;
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

	SetMoveVector( *m_pAbductUFOPosition );		// �}�U�[�V�b�v�̍��W�Ƃ̃x�N�g�����擾.
	m_TargetPosition = *m_pAbductUFOPosition;	// �}�U�[�V�b�v�̍��W���L��.

	TargetRotation();	// �}�U�[�V�b�v�̕��։�].
	CAlien::VectorMove( m_MoveSpeed );	// �ړ�.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// ���̎q��A�ꋎ���Ă��Ȃ����.
	m_NowState		= EAlienState::Move;		// �ړ���Ԃ֑J��.
	m_NowMoveState	= EMoveState::Rotation;		// �ړ��̉�]��Ԃ֑J��.
}

// �m�b�N�o�b�N.
void CAlien::KnockBack()
{
	m_KnockBackCount++;	// ���G�J�E���g�����Z.
	if( m_KnockBackCount <= KNOCK_BACK_TIME ){
		m_vRotation.y = atan2( m_KnockBackVector.x, m_KnockBackVector.z ) + static_cast<float>(D3DX_PI);
		m_vPosition.x -= m_KnockBackVector.x;
		m_vPosition.z -= m_KnockBackVector.z;
	}
}

// ����.
void CAlien::Fright()
{
	if( m_AnimFrameList[EAnimNo_Damage].IsNowFrameOver() == false ) return;

	m_KnockBackCount	= 0;	// ���G�J�E���g�̏�����.
	SetAnimation( EAnimNo_Move, m_pAC );
	m_NowState			= EAlienState::Move;	// �ړ���Ԃ֑J��.
	m_NowMoveState		= EMoveState::Rotation;	// �ړ��̉�]��Ԃ֑J��.
}

// ���S.
void CAlien::Death()
{
	if( m_AnimFrameList[EAnimNo_Dead].IsNowFrameOver() == false ) return;

	m_AnimSpeed = 0.0;
	m_DeathCount += DEATH_COUNT_ADD_VALUE;
	m_DeathScale -= DEATH_SCALE_SUB_VALUE;
	// ���f���̃T�C�Y�̌v�Z.
	const float scale = m_DeathScale*exp(-m_DeathCount)*sinf(DEATH_SCALE_PI*m_DeathCount);
	m_vScale = { scale, scale, scale };

	// �傫�������l�ȏ�Ȃ�.
	if( m_vScale.x > 0.0f ) return;
	m_vScale = { 0.0f, 0.0f, 0.0f };
	CSoundManager::PlaySE("AlienDead");
	m_IsDelete = true;	// ���S�t���O�𗧂Ă�.
}

// ������.
void CAlien::Escape()
{
	if( m_IsBarrierHit == true ) return;

	SetMoveVector( *m_pAbductUFOPosition );		// �}�U�[�V�b�v�̍��W�Ƃ̃x�N�g�����擾.
	m_TargetPosition = *m_pAbductUFOPosition;	// �}�U�[�V�b�v�̍��W���L��.

	TargetRotation();	// �}�U�[�V�b�v�̕��։�].
	CAlien::VectorMove( m_MoveSpeed );	// �ړ�.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// ���̎q��A�ꋎ���Ă��Ȃ����.
	m_NowState		= EAlienState::Move;		// �ړ���Ԃ֑J��.
	m_NowMoveState	= EMoveState::Rotation;		// �ړ��̉�]��Ԃ֑J��.
}

// �}�U�[�V�b�v�ɏ����Ă���.
void CAlien::RisingMotherShip()
{
	m_vScale.x -= RISING_MOTHER_SHIP_SCALE_SUB_VALUE;
	m_vScale.y -= RISING_MOTHER_SHIP_SCALE_SUB_VALUE;
	m_vScale.z -= RISING_MOTHER_SHIP_SCALE_SUB_VALUE;
	if( m_vScale.x > 0.0f ) return;
	m_IsDelete = true;	// ���S�t���O�𗧂Ă�.
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

	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ){
		// �A�[����Еt���Ă��Ȃ���ΕЕt����.
		if( m_pArm->IsCleanUp() == false ){
			m_AnimSpeed = 0.01;
			SetAnimation( EAnimNo_Move, m_pAC );
			m_pArm->SetCleanUpPreparation();
		}
		m_NowState		= EAlienState::Move;		// �ړ���Ԃ֑J��.
		m_NowMoveState	= EMoveState::Rotation;		// �ړ��̉�]��Ԃ֑J��.
		return;
	}

	if( m_NowState == EAlienState::Abduct ){
		// �A�ꋎ���Ă����ԂȂ̂ŃA�[���̍��W��ݒ肷��.
		pActor->SetPosition( {m_pArm->GetGrabPosition().x, m_pArm->GetGrabPosition().y-5.5f, m_pArm->GetGrabPosition().z} );
		pActor->SetRotationY( m_vRotation.y );
		return;
	} else {
		// ���ɘA�ꋎ���Ă��邩.
		if( *m_pIsAlienOtherAbduct == true ){
			// �A�[����Еt���Ă��Ȃ���ΕЕt����.
			if( m_pArm->IsCleanUp() == false ){
				m_AnimSpeed = 0.01;
				SetAnimation( EAnimNo_Move, m_pAC );
				m_pArm->SetCleanUpPreparation();
			}
			return;
		}
	}
	
	// �͂�ł��Ȃ����(�A�[�������o���ĂȂ����).
	if( m_pArm->IsGrab() == false ){
		m_pArm->SetAppearancePreparation();	// �A�[�������o��.
		SetAnimation( EAnimNo_Arm, m_pAC );
		if( m_AnimFrameList[EAnimNo_Arm].IsNowFrameOver() == true ){
			m_AnimSpeed = 0.0;
		}
		return;
	} else {
		//const float lenght = D3DXVec3Length( &D3DXVECTOR3(pActor->GetPosition()-m_vPosition) );
		//// �ړI�̍��W�Ƃ̋��������l��菭�Ȃ�����r.
		//if( lenght > m_pArm->GRAB_DISTANCE ){
		//	// �A�[����Еt���Ă��Ȃ���ΕЕt����.
		//	if( m_pArm->IsCleanUp() == false ){
		//		m_AnimSpeed = 0.01;
		//		SetAnimation( EAnimNo_Move, m_pAC );
		//		m_pArm->SetCleanUpPreparation();
		//	}
		//	m_NowState		= EAlienState::Move;		// �ړ���Ԃ֑J��.
		//	m_NowMoveState	= EMoveState::Rotation;		// �ړ��̉�]��Ԃ֑J��.
		//}
		// �A�[���̍��W��ݒ肷��.
//		pActor->SetPosition( {m_pArm->GetGrabPosition().x, m_pArm->GetGrabPosition().y-5.5f, m_pArm->GetGrabPosition().z} );
	}

	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState		= EAlienState::Abduct;	// �A�ꋎ���Ԃ֑J��.
	m_NowMoveState	= EMoveState::Rotation;	// �ړ�����]�֑J��.
}

// �o���A�Ƃ̓����蔻��.
void CAlien::BarrierCollision( CActor* pActor )
{
	if( m_NowState == EAlienState::RisingMotherShip ) return;	// �}�U�[�V�b�v�ɏ����Ă����ԂȂ�I��.
	// �I�u�W�F�N�g�̃^�O���o���A����Ȃ���ΏI��.
	if( pActor->GetObjectTag() != EObjectTag::Bariier ) return;
	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == true ){
		CSoundManager::PlaySE("BarrierHit");
		m_MoveSpeed		= BARRIER_HIT_MOVE_SPEED;	// �o���A�Փˎ��̈ړ����x�ɕύX����.
		m_IsBarrierHit	= true;						// �o���A�Փ˃t���O�𗧂Ă�.
		m_NowState		= EAlienState::Move;		// �ړ���Ԃ֑J��.
		m_NowMoveState	= EMoveState::Move;			// �ړ��̈ړ���Ԃ֑J��.
		*m_pIsAlienOtherAbduct	= false;			// ���̎q��A�ꋎ��t���O������.
		SetAnimation( EAnimNo_Move, m_pAC );
		m_AnimSpeed = 0.01;
	} else {
		m_MoveSpeed		= m_Parameter.MoveSpeed;	// �ʏ�̈ړ����x�ɖ߂�.
		m_IsBarrierHit	= false;	// �o���A�Փ˃t���O������.
	}
}

// �A�j���[�V�����R���g���[���[�̎擾.
bool CAlien::GetAnimationController()
{
	if( m_pSkinMesh == nullptr ) return false;
	// �A�j���[�V�����R���g���[���[�̃N���[���쐬.
	if( FAILED( m_pSkinMesh->GetAnimController()->CloneAnimationController(
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationOutputs(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumAnimationSets(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumTracks(),
		m_pSkinMesh->GetAnimationController()->GetMaxNumEvents(),
		&m_pAC ) )) return false;
	return true;
}

// �A�j���[�V�����t���[���̐ݒ�.
bool CAlien::SetAnimFrameList()
{
	if( m_pSkinMesh == nullptr ) return false;
	// �����p�A�j���[�V�����t���[���̃��X�g��p��.
	const double animAdjFrames[] =
	{
		0.0f,
		0.5f,
		0.0f,
		0.0f,
	};
	if( m_pSkinMesh == nullptr ) return false;
	for( int i = EAnimNo_Begin; i < EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-animAdjFrames[i] };
	}
	return true;
}