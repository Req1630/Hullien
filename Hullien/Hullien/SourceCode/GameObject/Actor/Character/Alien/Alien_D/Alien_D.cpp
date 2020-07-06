#include "Alien_D.h"
#include "..\..\..\LaserBeam\LaserBeam.h"
#include "..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

CAlienD::CAlienD()
	: m_pAttackRangeSprite	( nullptr )
	, m_pLaserBeam			( nullptr )
	, m_AttackCount			( 0.0f )
	, m_IsAttackStart		( false )
{
	m_ObjectTag = EObjectTag::Alien_D;
	m_vSclae = { 0.05f, 0.05f, 0.05f };
	m_pLaserBeam = std::make_unique<CLaserBeam>();
}

CAlienD::~CAlienD()
{
}

// �������֐�.
bool CAlienD::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
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
	AttackRangeSpriteRender();

#if _DEBUG
	m_pCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CAlienD::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	AttackCollision( pActor );
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
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
	m_NowState = EAlienState::Spawn;	// ���݂̏�Ԃ��X�|�[���ɕύX.
	return true;
}

// ���f���̕`��.
void CAlienD::ModelRender()
{
	if( m_pSkinMesh == nullptr ) return;
	
	m_pSkinMesh->SetPosition( m_vPosition );
	D3DXVECTOR3 rot = m_vRotation;
	rot.y += static_cast<float>(D3DX_PI);
	m_pSkinMesh->SetRotation( rot );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetColor( { 0.8f, 0.8f, 0.5f, m_ModelAlpha } );
	m_pSkinMesh->SetBlend( true );
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pSkinMesh->Render();
	m_pSkinMesh->SetRasterizerState( CCommon::enRS_STATE::None );
	m_pSkinMesh->SetBlend( false );
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

	m_pAttackRangeSprite->SetPosition( { m_TargetPosition.x, m_TargetPosition.y+0.002f, m_TargetPosition.z } );
	m_pAttackRangeSprite->SetRotation( { static_cast<float>(D3DXToRadian(90)), 0.0f, 0.0f } );
	m_pAttackRangeSprite->SetScale( 10.0f );
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
	CAlienD::VectorMove( m_Parameter.MoveSpeed );	// �ړ�.
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

	const float attackSpeed = m_IsAttackStart == false ? 0.005f : -0.01f;
	m_AttackCount += attackSpeed;	// �U���J�E���g�̒ǉ�.

	// �U���J�E���g���U�����Ԃ�葽���Ȃ�΍U�����n�߂�.
	if( m_AttackCount >= ATTACK_TIME ){
		m_IsAttackStart = true;
		m_pLaserBeam->Shot();	// �r�[����ł�.
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

	if( lenght >= m_Parameter.AttackLenght ) return;
	m_pLaserBeam->ResetParam();
	m_IsAttackStart = false;
	m_AttackCount = 0.0f;
	m_NowMoveState = EMoveState::Attack;
}

// ������W�̐ݒ�.
void CAlienD::SetTargetPos( CActor& actor )
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