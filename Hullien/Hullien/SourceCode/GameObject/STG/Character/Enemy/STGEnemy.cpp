#include "STGEnemy.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Common\D3DX\D3DX11.h"
#include "..\..\..\..\Common\Font\Font.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\Bullet\BuletManager\BuletManager.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"

STG::CEnemy::CEnemy()
	: CEnemy	( STG::SEnemyParam() )
{
}

STG::CEnemy::CEnemy( const STG::SEnemyParam& param )
	: PARAMETER				( param )
	, m_pGuns				()
	, m_pFont				( nullptr )
	, m_NowState			( STG::EEnemyState::Spawn )
	, m_DeadUpParam			()
	, m_FontRotation		( FONT_ROTATION )
	, m_MoveSpeed			( 0.0f )
	, m_MoveingDistance		( 0.0f )
	, m_MoveingDistanceMax	( 0.0f )
	, m_ShakeCount			( SHAKE_COUNT_MAX )
	, m_EscapeCount			( ESCAPE_COUNT_MAX )
	, m_SpawnCount			( 0 )
	, m_IsHitShake			( false )
	, m_IsMySpawnLast		( false )
{
	m_pFont			= std::make_unique<CFont>();
	m_pCollManager	= std::make_shared<CCollisionManager>();
	m_vPosition.z	= INIT_POSITION_Z;
	m_vPosition.x	= PARAMETER.PositionX;
	m_vRotation		= { 0.0f, 0.0f, static_cast<float>(D3DXToRadian(90)) };
	m_MoveSpeed		= PARAMETER.MoveSpeed;
	m_LifePoint		= PARAMETER.LifePoint;
	m_vScale		= { PARAMETER.TextSize, PARAMETER.TextSize, PARAMETER.TextSize };

	SBulletManagerParam s = {};
	m_pGuns.emplace_back( std::make_unique<CBulletManager>( s ) );
}

STG::CEnemy::CEnemy( 
	const STG::SEnemyParam& param, 
	const std::vector<SBulletManagerParam>& bulletParams )
	: CEnemy	( param )
{
	m_pGuns.resize( bulletParams.size() );
	auto it = bulletParams.begin();
	for( auto& g : m_pGuns ){
		g = std::make_unique<CBulletManager>( *it );
		it++;
	}
}

STG::CEnemy::~CEnemy()
{
}

// �������֐�.
bool STG::CEnemy::Init()
{
	if( CollisionInit()	== false ) return false;
	for( auto& g : m_pGuns ) if( g->Init()	== false ) return false;
	if( FAILED( m_pFont->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ) ))	return false;

	return true;
}

// �X�V�֐�.
void STG::CEnemy::Update()
{
	// �e�̍X�V.
	for( auto& g : m_pGuns ) g->Update();

	switch( m_NowState )
	{
	case STG::EEnemyState::Spawn:	Spawn();	break;	// �X�|�[��.
	case STG::EEnemyState::Move:	Move();		break;	// �ړ�.
	case STG::EEnemyState::Shot:	Shot();		break;	// �e������.
	case STG::EEnemyState::Escape:	Escape();	break;	// ������.
	case STG::EEnemyState::Dead:	Dead();		break;	// ���S.
	default:									break;
	}

	HitShake();		// �q�b�g���̗h��.
}

// �`��֐�.
void STG::CEnemy::Render()
{
	// �e�̕`��.
	for( auto& g : m_pGuns ) g->Render();

	if( m_NowState == STG::EEnemyState::None ) return;

	m_pFont->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
	m_pFont->SetPosition( m_vPosition );
	m_pFont->SetRotation( m_FontRotation );
	m_pFont->SetScale( m_vScale );
	m_pFont->SetCoverage( true );
	m_pFont->Render( PARAMETER.Text );
	m_pFont->SetCoverage( false );

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// �����蔻��.
void STG::CEnemy::Collision( STG::CActor* pActor )
{
	if( pActor->GetActive()	== false ) return;	// ���肪���삵�ĂȂ���ΏI��.
	// �e�̓����蔻��.
	for( auto& g : m_pGuns ) g->Collision( pActor );

	if( m_IsActive			== false ) return;	// ���������삵�ĂȂ���ΏI��.
	// �J�v�Z���̓����蔻��.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;
}

// �X�|�[��.
void STG::CEnemy::Spawn()
{
	m_SpawnCount++;	// �X�|�[���J�E���g�����Z.
	if( m_SpawnCount < PARAMETER.SpawnTime ) return;
	// �X�|�[���J�E���g�����l�ɒB�����.
	// �X�|�[������.
	m_NowState = STG::EEnemyState::Move;
	m_IsActive = true;
}


// �ړ��֐�.
void STG::CEnemy::Move()
{
	m_vPosition.z += m_MoveSpeed;
	if( m_vPosition.z >= MOVE_SUB_POSITION_Z ) m_MoveSpeed -= MOVE_SUB_VALUE;

	if( m_MoveSpeed > 0.0f ) return;

	m_NowState = STG::EEnemyState::Shot;

	// �����p�x��ݒ�.
	for( auto& g : m_pGuns ) g->SetInitAngle( m_vPosition, m_TargetPositon );
}

// �e������.
void STG::CEnemy::Shot()
{
	for( auto& g : m_pGuns ) g->SetPosition( m_vPosition );
	for( auto& g : m_pGuns ) g->Shot();
	bool isEnd = true;
	for( auto& g : m_pGuns ){
		if( g->IsShotEnd() == false ) isEnd = false;
	}
	if( isEnd == false ) return;
	m_NowState = STG::EEnemyState::Escape;
}

// ������.
void STG::CEnemy::Escape()
{
	// �ړ����x����葬�x�ɂȂ�܂ŉ��Z.
	if( m_MoveSpeed < PARAMETER.MoveSpeed ) m_MoveSpeed += MOVE_SUB_VALUE;

	// �x�N�g�����g�p���Ĉړ�.
	m_vPosition.x += m_MoveVector.x * m_MoveSpeed;
	m_vPosition.z += m_MoveVector.z * m_MoveSpeed;

	m_EscapeCount--;	// ������J�E���g�̌��Z.
	// ������J�E���g�� 0.0 ���傫�����.
	if( m_EscapeCount > 0.0f ){
		m_MoveingDistance += m_MoveSpeed;	// ���������Z.
	}

	// �ړ���������苗���𒴓�����.
	if( m_MoveingDistance >= m_MoveingDistanceMax ){
		SearchRandomMoveVector();	// �ړ��x�N�g������������.
	}

	// ��ʊO�ɏo����.
	if( IsDisplayOut( E_WND_OUT_ADJ_SIZE ) == true ){
		m_NowState = STG::EEnemyState::Dead;	// ���S.
	}
}

// ���S.
void STG::CEnemy::Dead()
{
	if( m_IsMySpawnLast == false ){
		m_IsActive	= false;		// ����I��.
		m_vScale.x -= DEAD_SCALE_SUB_VALUE;	// �X�P�[�������Z.
		m_vScale.y -= DEAD_SCALE_SUB_VALUE;	// �X�P�[�������Z.
		m_vScale.z -= DEAD_SCALE_SUB_VALUE;	// �X�P�[�������Z.
		m_FontRotation.z += DEAD_ROTATION_SPEED;	// ��]������.

		if( m_vScale.x > 0.0f ) return;
		// �X�P�[���� 0.0 �ȉ��ɂȂ��.
		m_NowState	= STG::EEnemyState::None;	// �����Ȃ���Ԃ֑J��.
		// ���W����ʊO��.
		m_vPosition	= { INIT_POSITION_Z, 0.0f, INIT_POSITION_Z };
	} else {
		// �X�|�[�����X�g�̍ۂ̎��S����
		SpawnLastDead();
	}
}

// �X�|�[�����X�g�̍ۂ̎��S����.
void STG::CEnemy::SpawnLastDead()
{
	D3DXVECTOR3 cameraPos = CCameraManager::GetPosition();	// �J�����̍��W.
	D3DXVECTOR3 vec = cameraPos - m_vPosition;	// �ړI�̍��W�ւ̃x�N�g��.
	vec.y = 0.0f;
	float length = 0.0f;
	// �ړI�̍��W�Ƃ̋��������ȏ�Ȃ�.
	//	�ړI�̍��W�ɋ߂Â���.
	length = D3DXVec3Length( &vec );
	if( length >= DEAD_TARGET_POSITION_LENGTH && m_DeadUpParam.IsMoveEnd == false ){
		D3DXVec3Normalize( &vec, &vec );
		const float moveSpeed = length * DEAD_TARGET_MOVE_SPEED;
		m_vPosition.x += vec.x * moveSpeed;
		m_vPosition.z += vec.z * moveSpeed;
	}
	m_FontRotation.z += DEAD_ROTATION_SPEED;	// ��]������.

	// �ړ������l�̉��Z.
	m_DeadUpParam.MoveAccValue += DEAD_MOVE_ACC_ADD_VALUE;
	if( m_DeadUpParam.MoveAccValue >= DEAD_MOVE_ACC_VALUE_MAX ) 
		m_DeadUpParam.MoveAccValue = DEAD_MOVE_ACC_VALUE_MAX;

	// ���W����ɂ�����.
	if( m_vPosition.y < cameraPos.y-DEAD_POSITION_Y_ADJ_VALUE ){
		m_vPosition.y += m_DeadUpParam.MoveSpeed + m_DeadUpParam.MoveAccValue;
	} else {
		m_vPosition.y = cameraPos.y-DEAD_POSITION_Y_ADJ_VALUE;
		m_FontRotation.z -= DEAD_ROTATION_SPEED;

		m_DeadUpParam.IsMoveEnd = true;
		if( m_DeadUpParam.IsPlaySE == false ){
			CSoundManager::PlaySE("camaeraHit");
			m_DeadUpParam.IsPlaySE = true;
		}
	}

	// �J�����̗h��.
	if( m_DeadUpParam.IsMoveEnd == true ){
		D3DXVECTOR3 cameraLookPos = CCameraManager::GetLookPosition();
		m_DeadUpParam.CameraShakeCount--;
		if( m_DeadUpParam.CameraShakeCount <= 0.0f ) m_DeadUpParam.CameraShakeCount = 0.0f;

		// �h��l�̎擾.
		const float SHAKE_VALUE =  m_DeadUpParam.GetShakeValue();
		cameraPos.x		+= SHAKE_VALUE;
		cameraPos.z		+= SHAKE_VALUE;
		cameraLookPos.x	+= SHAKE_VALUE * DEAD_CAMERA_SHAKE_ADJ_VALUE;
		cameraLookPos.z	+= SHAKE_VALUE * DEAD_CAMERA_SHAKE_ADJ_VALUE;

		CCameraManager::SetPosition( cameraPos );
		CCameraManager::SetLookPosition( cameraLookPos );
	}
}

// �����������̗h��.
void STG::CEnemy::HitShake()
{
	if( m_IsHitShake == false ) return;

	m_ShakeCount -= SHAKE_SUB_VALUE;	// �h��J�E���g�����Z.
	m_vPosition.x += sinf( static_cast<float>(D3DX_PI)		* m_ShakeCount ) * SHAKE_SPEED;
	m_vPosition.z += sinf( static_cast<float>(D3DX_PI)*0.5f * m_ShakeCount ) * SHAKE_SPEED;

	if( m_ShakeCount > 0.0f ) return;
	// �h��J�E���g�� 0.0 �ȉ��ɂȂ��.
	m_ShakeCount = SHAKE_COUNT_MAX;	// �h��J�E���g��������.
	m_IsHitShake = false;			// �q�b�g�t���O������.
}

// ���C�t�v�Z�֐�.
void STG::CEnemy::LifeCalculation( const std::function<void(float&)>& proc )
{
	// �������Ԃ̂ݍU�����󂯂�.
	if( m_NowState != STG::EEnemyState::Escape ){
		m_IsHitShake = true;	// �q�b�g�t���O�𗧂Ă�.
		return;
	}

	proc( m_LifePoint );
	m_IsHitShake = true;	// �q�b�g�t���O�𗧂Ă�.

	if( m_LifePoint > 0.0f ) return;
	// ���C�t��0�ȉ��ɂȂ��
	m_NowState = STG::EEnemyState::Dead;	// ���S��Ԃ֑J��.
	CSoundManager::PlaySE(DEAD_SE_NAME);
}

// �����_���ňړ��x�N�g��������.
void STG::CEnemy::SearchRandomMoveVector()
{
	// �ړ��x�N�g���������_������擾.
	m_MoveVector.x = sinf( static_cast<float>(rand()) );
	m_MoveVector.z = cosf( static_cast<float>(rand()) );

	// �ő�ړ������������_������擾.
	m_MoveingDistanceMax = fabsf(cosf( static_cast<float>(rand()) )) * ESCAPE_MOVE_DISTANCE;
	m_MoveingDistance = 0.0f;
	m_pGuns[0]->RandomShot();
}

// �����蔻��̍쐬.
bool STG::CEnemy::CollisionInit()
{
	float charByte = 2.0f;	// �����̃o�C�g��.
	// �e�L�X�g�̈ꕶ���ڂ̃o�C�g�����擾.
	if( IsDBCSLeadByte( PARAMETER.Text[0] ) == TRUE ) charByte = 1.0f;

	// �e�L�X�g�̑傫�����擾.
	const float textScale = 
		( PARAMETER.TextSize * charByte * static_cast<float>(PARAMETER.Text.length()) );

	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		PARAMETER.TextSize,
		textScale ))) return false;
	return true;
}