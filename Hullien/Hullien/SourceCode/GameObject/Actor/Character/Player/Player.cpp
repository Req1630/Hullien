#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"

CPlayer::CPlayer()	
	: m_pSkinMesh	( nullptr )
	, m_pCamera		( nullptr )
{
	m_pCamera = std::make_shared<CRotLookAtCenter>();
	m_vSclae = { 0.03f, 0.03f, 0.03f };
}

CPlayer::~CPlayer()
{
}

// �������֐�.
bool CPlayer::Init()
{
	if( m_pSkinMesh != nullptr ) return true;
	CMeshResorce::GetSkin( m_pSkinMesh, MODEL_NAME );

	if( m_pSkinMesh == nullptr ) return false;
	return true;
}

// �X�V�֐�.
void CPlayer::Update()
{
	Controller();	// ����.
	Move();			// �ړ�.

	m_pCamera->SetLength( 7.0f );	// ���S�Ƃ̋�����ݒ�.
	m_pCamera->SetHeight( 3.0 );	// �����̐ݒ�.
	// �v���C���[�𒍎����ĉ�].
	m_pCamera->RotationLookAtObject( m_vPosition );

	// �J�������}�l�[�W���[�ɐݒ�.
	CCameraManager::SetCamera( m_pCamera );
}

// �`��֐�.
void CPlayer::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->Render();
}

// ����֐�.
void CPlayer::Controller()
{
	// �R���g���[���[��L�X�e�B�b�N�̌X�����擾.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	// �J�����̉�]�ړ�.
	// ������.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalMove(  0.05f );	// �E����.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalMove( -0.05f );	// ������.
}

// �ړ��֐�.
void CPlayer::Move()
{
	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// �X�e�B�b�N�̌X���������Ɍ���.
	m_vRotation.y = atan2f( m_MoveVector.x, m_MoveVector.z );
	// �J�����̊p�x�Ƒ������킹��.
	m_vRotation.y += m_pCamera->GetRadianX();
	// ��]���ňړ�.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_MoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_MoveSpeed;
}