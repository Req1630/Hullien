#include "STGPlayer.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\Utility\XInput\XInput.h"

STG::CPlayer::CPlayer()
	: m_Direction	( 0.0f, 0.0f, 0.0f )
{
	m_IsActive		= true;
	m_pCollManager	= std::make_shared<CCollisionManager>();
}

STG::CPlayer::~CPlayer()
{
}

// �������֐�.
bool STG::CPlayer::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME )	== false ) return false;
	if( CollisionInit()											== false ) return false;
	if( BulletInit( m_pBullets,BULLET_COUNT_MAX, BULLET_MODEL_NAME )	== false ) return false;
	for( auto& b : m_pBullets ) b->SetCollDisappear();	// ���������e�������邩�ǂ����̐ݒ�.
	return true;
}

// �X�V�֐�.
void STG::CPlayer::Update()
{
	Move();			// �ړ�.
	BulletUpdate();	// �e�̍X�V.
}

// �`��֐�.
void STG::CPlayer::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	MeshRender();	// ���b�V���̕`��.
	BulletRender();	// �e�̕`��.

#ifdef _DEBUG
	m_pCollManager->DebugRender();
#endif	// #ifdef _DEBUG.
}

// �����蔻��.
void STG::CPlayer::Collision( STG::CActor* pActor )
{
	if( m_IsActive			== false ) return;	// ���������삵�ĂȂ���ΏI��.
	if( pActor->GetActive()	== false ) return;	// ���肪���삵�ĂȂ���ΏI��.

	// �e�̐����������蔻����s��.
	for( auto& b : m_pBullets ) b->Collision( pActor );
	// �J�v�Z���̓����蔻��.
	if( m_pCollManager->IsCapsuleToCapsule( pActor->GetColl() ) == false ) return;

}

// �ړ��֐�.
void STG::CPlayer::Move()
{
	m_vPosition.x -= m_MoveVector.x * MOVE_SPEED;
	m_vPosition.z -= m_MoveVector.z * MOVE_SPEED;
}

// ����֐�.
void STG::CPlayer::Controller()
{
	if( m_IsActive == false ) return;
	// �R���g���[���[��L�X�e�B�b�N�̌X�����擾.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());
	// �R���g���[���[��R�X�e�B�b�N�̌X�����擾.
	m_Direction.x = static_cast<float>(CXInput::RThumbX_Axis());
	m_Direction.z = static_cast<float>(CXInput::RThumbY_Axis());

	if( GetAsyncKeyState('W') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('S') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState('D') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('A') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MIN;

	if( GetAsyncKeyState(VK_UP) & 0x8000 )		m_Direction.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 )	m_Direction.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )	m_Direction.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )	m_Direction.x = IDLE_THUMB_MIN;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
		m_MoveVector = { 0.0f, 0.0f, 0.0f };
	} else {
		// �m�[�}���C�Y.
		D3DXVec3Normalize( &m_MoveVector, &m_MoveVector );
	}
	// �e�l���L���͈͓��Ȃ�.
	if( m_Direction.x >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_Direction.x ||
		m_Direction.z >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_Direction.z ){
		m_vRotation.y = atan2( m_Direction.x, m_Direction.z );	// ��]�l���擾.
	}
	// �e��������.
	ShotController();
}

// �e��������֐�.
void STG::CPlayer::ShotController()
{
	// �������u�Ԃ�ShotCount�������E�e������.
	if( CXInput::R_Button() == CXInput::enPRESSED_MOMENT ){
		m_ShotCount = 0;
		BulletShot( m_vRotation.y, BULLET_MOVE_SPEED );
	}
	// �������̏ꍇ�e�����EShotCount�̉��Z.
	if( CXInput::R_Button() == CXInput::enPRESS_AND_HOLD || ( GetAsyncKeyState('R') & 0x8000 )){
		m_ShotCount++;
		if( m_ShotCount == SHOT_INTERVAL_FRAME ){
			BulletShot( m_vRotation.y, BULLET_MOVE_SPEED );
			m_ShotCount = 0;
		}
	}
}

// �����蔻��̍쐬.
bool STG::CPlayer::CollisionInit()
{
	if( FAILED( m_pCollManager->InitCapsule(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{0.0f, 0.0f, 0.0f},
		2.0f,
		2.0f ))) return false;
	return true;
}