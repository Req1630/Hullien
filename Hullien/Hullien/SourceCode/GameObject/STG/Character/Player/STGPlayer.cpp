#include "STGPlayer.h"
#include "..\..\Bullet\STGBullet.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Utility\XInput\XInput.h"

STG::CPlayer::CPlayer()
	: m_Direction	( 0.0f, 0.0f, 0.0f )
{
	m_pBullets.resize( BULLET_COUNT_MAX );
}

STG::CPlayer::~CPlayer()
{
}

// �������֐�.
bool STG::CPlayer::Init()
{
	if( CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME ) == false ) return false;
	if( BulletInit( BULLET_MODEL_NAME ) == false ) return false;
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
	m_pStaticMesh->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	MeshRender();
	BulletRender();
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
	// �R���g���[���[��L�X�e�B�b�N�̌X�����擾.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());
	// �R���g���[���[��R�X�e�B�b�N�̌X�����擾.
	m_Direction.x = static_cast<float>(CXInput::RThumbX_Axis());
	m_Direction.z = static_cast<float>(CXInput::RThumbY_Axis());
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
	if( CXInput::R_Button() == CXInput::enPRESS_AND_HOLD ){
		m_ShotCount++;
		if( m_ShotCount == SHOT_INTERVAL_FRAME ){
			BulletShot( m_vRotation.y, BULLET_MOVE_SPEED );
			m_ShotCount = 0;
		}
	}
}