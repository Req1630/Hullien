#include "Alien_D.h"
#include "..\..\..\LaserBeam\LaserBeam.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\XAudio2\SoundManager.h"

CAlienD::CAlienD()
	: m_pAttackRangeSprite	( nullptr )
	, m_pLaserBeam			( nullptr )
	, m_ControlPositions	( 2 )
	, m_AttackCount			( 0.0f )
	, m_IsAttackStart		( false )
{
	m_ObjectTag = EObjectTag::Alien_D;
	m_HasAnyItem = EItemList::SPEffectTime;
	m_pLaserBeam = std::make_unique<CLaserBeam>();
}

CAlienD::~CAlienD()
{
}

// �������֐�.
bool CAlienD::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	if( GetModel( MODEL_TEMP_NAME ) == false ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( GetSprite( SPRITE_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	if( m_pLaserBeam->Init() == false ) return false; 
	return true;
}

// �X�V�֐�.
void CAlienD::Update()
{
	SetMoveVector( m_TargetPosition );
	m_pLaserBeam->Update();
	CurrentStateUpdate();	// ���݂̏�Ԃ̍X�V.
}

// �`��֐�.
void CAlienD::Render()
{
	m_pLaserBeam->Render();
	ModelRender();

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CAlienD::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	AttackCollision( pActor );
	BarrierCollision( pActor );
}

// �X�|�[��.
bool CAlienD::Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos )
{
	// ���ɃX�|�[���ς݂Ȃ�I��.
	if( m_NowState != EAlienState::None ) return true;
	// �������Ɏ��s������I��.
	if( Init() == false ) return false;
	m_Parameter = param;	// �p�����[�^��ݒ�.
	m_vPosition = spawnPos;	// �X�|�[�����W�̐ݒ�.
	m_LifePoint = m_Parameter.LifeMax;	// �̗͂̐ݒ�.
	m_vPosition.y += INIT_POSITION_ADJ_HEIGHT;
	m_NowState = EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.
	
	// ���[�U�[�̈ړ����x�̐ݒ�.
	m_pLaserBeam->SetMoveSpped( m_Parameter.LaserMoveSpeed );
	// ���[�U�[�̖�჎��Ԃ̐ݒ�.
	m_pLaserBeam->SetParalysisTime( m_Parameter.ParalysisTime );

	if (CSoundManager::GetIsPlaySE("AlienDApp", 0) == false)
	{
		CSoundManager::NoMultipleSEPlay("AlienDApp");
	}
	return true;
}

// �X�v���C�g�̕`��.
void CAlienD::SpriteRender()
{
	AttackRangeSpriteRender();
}

// ���f���̕`��.
void CAlienD::ModelRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.5f, 0.8f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );
#else
	if( m_pTempStaticMesh == nullptr ) return;
	m_pTempStaticMesh->SetPosition( m_vPosition );
	m_pTempStaticMesh->SetRotation( m_vRotation );
	m_pTempStaticMesh->SetScale( m_vSclae );
	m_pTempStaticMesh->SetColor( { 0.8f, 0.8f, 0.0f, m_ModelAlpha } );
	m_pTempStaticMesh->SetBlend( true );
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pTempStaticMesh->Render();
	m_pTempStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pTempStaticMesh->SetBlend( false );
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// �U���͈͂̃X�v���C�g�`��.
void CAlienD::AttackRangeSpriteRender()
{
	if( m_pAttackRangeSprite == nullptr ) return;
	if( m_NowMoveState != EMoveState::Attack ) return;

	D3DXVECTOR4 color;
	if( m_IsAttackStart == true ){
		// �U���̐F (��).
		color = ATTACK_RANGE_DANGER_COLOR;
		color.w = m_AttackCount;
	} else {
		// �U���̗\���̐F (��).
		color = ATTACK_RANGE_COLOR;
		color.w = m_AttackCount;
	}

	m_pAttackRangeSprite->SetPosition( { m_TargetPosition.x, m_Parameter.AttackRangeSpritePosY, m_TargetPosition.z } );
	m_pAttackRangeSprite->SetRotation( { static_cast<float>(D3DXToRadian(90)), 0.0f, 0.0f } );
	m_pAttackRangeSprite->SetScale( m_Parameter.AttackRangeSpriteScale );	
	m_pAttackRangeSprite->SetColor( color );
	m_pAttackRangeSprite->SetBlend( true );
	m_pAttackRangeSprite->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pAttackRangeSprite->Render();
	m_pAttackRangeSprite->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pAttackRangeSprite->SetBlend( false );
}

// �X�|�[��.
void CAlienD::Spawning()
{
	CAlien::Spawning();
}

// �ړ�.
void CAlienD::Move()
{
	TargetRotation();		// ��].
	CAlienD::VectorMove( m_MoveSpeed );	// �ړ�.
	Attack();				// �U��.
	CAlien::WaitMove();		// �ҋ@.

	if( *m_pIsAlienOtherAbduct == false ) return;
	if( m_NowState == EAlienState::Abduct ) return;
	m_NowState = EAlienState::Escape;
	m_NowMoveState = EMoveState::Rotation;	// �ړ���Ԃ���]����.
}

// ����.
void CAlienD::Abduct()
{
}

// ����.
void CAlienD::Fright()
{
	CAlien::Fright();
}

// ���S.
void CAlienD::Death()
{
	CAlien::Death();
}

// ������.
void CAlienD::Escape()
{
	CAlien::Escape();
}

// �U���֐�.
void CAlienD::Attack()
{
	if( m_NowMoveState != EMoveState::Attack ) return;

	const float attackSpeed = m_IsAttackStart == false ? 
		m_Parameter.AttackRangeAddValue : m_Parameter.AttackRangeSubValue;
	m_AttackCount += attackSpeed;	// �U���J�E���g�̒ǉ�.

	// �U���J�E���g���U�����Ԃ�葽���Ȃ�΍U�����n�߂�.
	if( m_AttackCount >= ATTACK_TIME ){
		m_IsAttackStart = true;

		{
			// ����ւ̌������擾.
			float radius = atan2f(
				m_TargetPosition.x - m_vPosition.x,
				m_TargetPosition.z - m_vPosition.z );

			// ����Ƃ̋����𑪂�.
			float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

			lenght -= m_Parameter.ControlPointTwoLenght;	// �v�Z�����������������炷.

			// ������������.
			m_ControlPositions[0].x = m_vPosition.x + sinf( radius ) * m_Parameter.ControlPointOneLenght;
			m_ControlPositions[0].y = m_vPosition.y + m_Parameter.ControlPointOneLenghtY;
			m_ControlPositions[0].z = m_vPosition.z + cosf( radius ) * m_Parameter.ControlPointOneLenght;
			// ������̏����O.
			m_ControlPositions[1].x = m_vPosition.x + sinf( radius ) * lenght;
			m_ControlPositions[1].y = m_vPosition.y + m_Parameter.ControlPointTwoLenghtY;
			m_ControlPositions[1].z = m_vPosition.z + cosf( radius ) * lenght;
		}
		// ��Őݒ肵���R���g���[���|�W�V������ݒ�.
		m_pLaserBeam->SetControlPointList( m_ControlPositions );

		m_pLaserBeam->Shot( m_vPosition );	// �r�[����ł�.
		CSoundManager::NoMultipleSEPlay("AlienDAttack");
	}

	if( m_AttackCount >= 0.0f ) return;
	m_NowMoveState = EMoveState::Wait;
}

// �ړ��֐�.
void CAlienD::VectorMove( const float& moveSpeed )
{
	if( m_NowMoveState != EMoveState::Move ) return;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_TargetPosition - m_vPosition) );

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	float researchLengh = D3DXVec3Length( &D3DXVECTOR3(m_BeforeMoveingPosition - m_vPosition) );
	if( researchLengh >= m_Parameter.ResearchLenght ){
		m_NowMoveState = EMoveState::Rotation;
		m_IsAttackStart = false;
		return;
	}

	if( lenght >= m_Parameter.AttackLenght ) return;
	if( m_pLaserBeam->IsEndAttack() == false ) return;
	m_IsAttackStart = false;
	m_AttackCount = 0.0f;
	m_NowMoveState = EMoveState::Attack;
}

// ������W�̐ݒ�.
void CAlienD::SetTargetPos( CActor& actor )
{
	SetPlayerPos( actor );	// �v���C���[���W�̐ݒ�.
}

// �v���C���[���W�̐ݒ�.
void CAlienD::SetPlayerPos( CActor& actor )
{
	// �v���C���[����Ȃ���ΏI��.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_pLaserBeam->SetTargetPos( actor );	// �v���C���[�̍��W���擾.
	if( m_IsAttackStart == true ) return;
	m_TargetPosition = actor.GetPosition();	// �v���C���[�̍��W���擾.
}

// �U���̓����蔻��.
void CAlienD::AttackCollision( CActor* pActor  )
{
	if( m_pLaserBeam == nullptr ) return;
	m_pLaserBeam->Collision( pActor );	// ���[�U�[�̓����蔻��.
}

// �X�v���C�g�̎擾.
bool CAlienD::GetSprite( const char* spriteName )
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pAttackRangeSprite != nullptr ) return true;
	// �X�v���C�g�̎擾.
	m_pAttackRangeSprite = CSpriteResource::GetSprite( spriteName );
	// �X�v���C�g���ǂݍ��߂ĂȂ���� false.
	if( m_pAttackRangeSprite == nullptr ) return false;
	return true;
}

// �����蔻��̐ݒ�.
bool CAlienD::ColliderSetting()
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