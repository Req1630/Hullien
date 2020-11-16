#include "EditAlien.h"
#include "..\..\..\..\..\Common\Effect\EffectManager.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"

CEditAlien::CEditAlien()
	: m_Paramter	()
{}

CEditAlien::~CEditAlien()
{}

// �X�|�[��.
bool CEditAlien::Spawn( const D3DXVECTOR3& spawnPos )
{
	m_vPosition	= spawnPos;						// �X�|�[�����W�̐ݒ�.
	m_LifePoint	= m_Paramter.LifeMax;			// �̗͂̐ݒ�.
	m_NowState	= alien::EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.
	m_AnimSpeed	= 0.0;							// �A�j���[�V�������x���~�߂�.
	m_pEffects[alien::EEffectNo_Spawn]->Play( m_vPosition );
	return true;
}

// �U���̍Đ�.
void CEditAlien::PlayAttack()
{
}

// ���݂̍Đ�.
void CEditAlien::PlayFright()
{
}

// ���S�̍Đ�.
void CEditAlien::PlayDeath()
{
	m_NowState = alien::EAlienState::Death;
	m_pEffects[alien::EEffectNo_Dead]->Play( m_vPosition );
	SetAnimation( alien::EAnimNo_Dead, m_pAC );
}

// �}�U�[�V�b�v�ɏ�铮��̍Đ�.
void CEditAlien::PlayRisingMotherShip( const D3DXVECTOR3& vPos )
{
	m_NowState = alien::EAlienState::RisingMotherShip;
	m_vPosition	= vPos;
}

// �ҋ@�֐�.
void CEditAlien::WaitMove()
{
	if( m_NowMoveState != alien::EMoveState::Wait ) return;

	// �ړ��p�x�N�g�����擾.
	m_MoveVector.x = sinf( m_vRotation.y );
	m_MoveVector.z = cosf( m_vRotation.y );

	// �x�N�g�����g�p���Ĉړ�.
	m_vPosition.x -= m_MoveVector.x * m_Paramter.MoveSpeed;
	m_vPosition.z -= m_MoveVector.z * m_Paramter.MoveSpeed;

	// ���W���͈͓��Ȃ�I��.
	if( MOVE_DISTANCE >= m_vPosition.x && m_vPosition.x >= -MOVE_DISTANCE &&
		MOVE_DISTANCE >= m_vPosition.z && m_vPosition.z >= -MOVE_DISTANCE ) return;
	m_vPosition.x = 0.0f;
	m_vPosition.z = 0.0f;
}

// �X�|�[����.
void CEditAlien::Spawning()
{
	// �X�P�[���̉��Z.
	m_vScale += 
	{
		m_Paramter.SpawnScaleAddValue, 
		m_Paramter.SpawnScaleAddValue,
		m_Paramter.SpawnScaleAddValue
	};

	// �傫�������l�ȏ�Ȃ�.
	if( m_vScale.x >= SCALE_MAX ){
		m_vScale = { SCALE_MAX, SCALE_MAX, SCALE_MAX };
	}

	m_vPosition.y -= m_Paramter.SpawnDownSpeed;	// ������������.
	if( m_vPosition.y <= 0.0f ){
		m_vPosition.y = 0.0f;
	}

	// ���������l���傫����ΏI��.
	if( m_vScale.x < SCALE_MAX || m_vPosition.y > 0.0f ) return;

	CSoundManager::NoMultipleSEPlay("AlienApp");
	m_AnimSpeed	= 0.01;
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// �ړ�.
void CEditAlien::Move()
{
	TargetRotation();					// ��].
	CAlien::VectorMove( m_MoveSpeed );	// �ړ�.
	CAlien::WaitMove();					// �ҋ@.
}

// ����.
void CEditAlien::Abduct()
{
}

// �m�b�N�o�b�N.
void CEditAlien::KnockBack()
{
	m_KnockBackCount++;	// ���G�J�E���g�����Z.
	if( m_KnockBackCount <= m_Paramter.KnockBackTime ){
		m_vRotation.y = atan2( m_KnockBackVector.x, m_KnockBackVector.z ) + static_cast<float>(D3DX_PI);
		m_vPosition.x -= m_KnockBackVector.x;
		m_vPosition.z -= m_KnockBackVector.z;
	}
}

// ����.
void CEditAlien::Fright()
{
	if( m_AnimFrameList[alien::EAnimNo_Damage].IsNowFrameOver() == false ) return;

	m_KnockBackCount	= 0;	// ���G�J�E���g�̏�����.
	SetAnimation( alien::EAnimNo_Move, m_pAC );
	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// ���S.
void CEditAlien::Death()
{
	if( m_AnimFrameList[alien::EAnimNo_Dead].IsNowFrameOver() == false ) return;

	m_AnimSpeed = 0.0;
	m_DeathCount += m_Paramter.DeadCountAddValue;
	m_DeathScale -= m_Paramter.DeadScaleSubValue;
	// ���f���̃T�C�Y�̌v�Z.
	const float scale = m_DeathScale*exp(-m_DeathCount)*sinf(DEATH_SCALE_PI*m_DeathCount);
	m_vScale = { scale, scale, scale };

	// �傫�������l�ȏ�Ȃ�.
	if( m_vScale.x > 0.0f ) return;
	m_vScale = { 1.0f, 1.0f, 1.0f };
	CSoundManager::PlaySE("AlienDead");

	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// ������.
void CEditAlien::Escape()
{
	if( m_IsBarrierHit == true ) return;

	SetMoveVector( *m_pAbductUFOPosition );		// �}�U�[�V�b�v�̍��W�Ƃ̃x�N�g�����擾.
	m_TargetPosition = *m_pAbductUFOPosition;	// �}�U�[�V�b�v�̍��W���L��.

	TargetRotation();	// �}�U�[�V�b�v�̕��։�].
	CAlien::VectorMove( m_MoveSpeed );	// �ړ�.

	if( *m_pIsAlienOtherAbduct == true ) return;
	// ���̎q��A�ꋎ���Ă��Ȃ����.
	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}

// �}�U�[�V�b�v�ɏ����Ă���.
void CEditAlien::RisingMotherShip()
{
	m_vScale.x -= m_Paramter.MotherShipUpScaleSubValue;
	m_vScale.y -= m_Paramter.MotherShipUpScaleSubValue;
	m_vScale.z -= m_Paramter.MotherShipUpScaleSubValue;
	if( m_vScale.x > 0.0f ) return;

	m_NowState	= alien::EAlienState::Move;
	m_NowMoveState = alien::EMoveState::Wait;
}
