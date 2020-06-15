#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

#include "..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"

CPlayer::CPlayer()
	: m_pCamera						( nullptr )
	, m_OldPosition					( 0.0f, 0.0f, 0.0f )
	, m_NowAnimNo					( CPlayer::enAnimNo::Walk )
	, m_OldAnimNo					( CPlayer::enAnimNo::None )
	, m_AttackComboCount			( 0 )
	, m_AttackEnabledFrameList		()
	, m_AttackDataQueue				()
	, m_IsDuringAvoid				( false )
	, m_AvoidMoveSpeed				( 0.0f )
	, m_Parameter					()
{
	m_ObjectTag = EObjectTag::Player;
	m_pCamera = std::make_shared<CRotLookAtCenter>();
	m_vSclae = { 0.03f, 0.03f, 0.03f };
}

CPlayer::~CPlayer()
{
}

// �������֐�.
bool CPlayer::Init()
{
	if( ParameterSetting( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;

	SetAttackFrameList();
	return true;
}

// �X�V�֐�.
void CPlayer::Update()
{
	Controller();			// ����.
	AttackController();	// �U������.
	AvoidController();		// ��𑀍�.
	AttackAnimation();	// �U���A�j���[�V����.
	Move();					// �ړ�.
	AvoidMove();			// ��𓮍�.

	m_pCamera->SetLength( m_Parameter.CameraDistance );	// ���S�Ƃ̋�����ݒ�.
	m_pCamera->SetHeight( m_Parameter.CameraHeight );	// �����̐ݒ�.
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
	m_pSkinMesh->SetAnimSpeed( 0.01 );
	m_pSkinMesh->Render();

#if _DEBUG
	m_pCollManager->DebugRender();
	// �G�f�B�b�g�p�̕`��֐����G�f�B�b�g�����_���[�ɒǉ�.
	CEditRenderer::PushRenderProc( [&](){ EditRender(); } );
	// �f�o�b�O�`��.
	DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CPlayer::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// ���̂̓����蔻��.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	
	// �U���֐�.
	auto attackProc = [&]( float& life ){ life -= m_Parameter.AttackPower; };
	if( GetAsyncKeyState('C') & 0x8000 )
		pActor->LifeCalculation( attackProc );
}

// ������W�̐ݒ�֐�.
void CPlayer::SetTargetPos( CActor& actor )
{
//	m_vPosition = actor.GetPosition();
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
		m_pCamera->DegreeHorizontalMove(  m_Parameter.CameraMoveSpeed );	// �E����.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalMove( -m_Parameter.CameraMoveSpeed );	// ������.
}

// �U������֐�.
void CPlayer::AttackController()
{
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;
	// X�{�^�����������u�Ԃ���Ȃ���ΏI��.
	if( CXInput::X_Button() != CXInput::enPRESSED_MOMENT ) return;
	// �U���J�E���g���ő�ȏ�Ȃ�I��.
	if( m_AttackComboCount >= m_Parameter.AttackComboMax ) return;
	m_AttackComboCount++;	// �U���J�E���g�����Z.
	// �U���f�[�^���L���[�ɒǉ����ꂽ��I��.
	if( IsPushAttack() == true ) return;
	m_AttackComboCount--;	// �U���J�E���g�����Z.
}

// ��𑀍�֐�.
void CPlayer::AvoidController()
{
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// A�{�^���������ꂽ�u�Ԃ���Ȃ���ΏI��.
	if( CXInput::A_Button() != CXInput::enPRESSED_MOMENT ) return;
	m_IsDuringAvoid = true;
	m_AvoidVector = m_MoveVector;	// �ړ��x�N�g����ݒ�.
	m_OldPosition = m_vPosition;	// ���݂̍��W��ݒ�.

	// ����A�j���[�V������ݒ肷��Ȃ� ����.
	//

}

// �ړ��֐�.
void CPlayer::Move()
{
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// �X�e�B�b�N�̌X���������Ɍ���.
	m_vRotation.y = atan2f( m_MoveVector.x, m_MoveVector.z );
	// �J�����̊p�x�Ƒ������킹��.
	m_vRotation.y += m_pCamera->GetRadianX();

	// ��]���ňړ�.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.MoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.MoveSpeed;
	m_OldPosition = m_vPosition;
}

// ��𓮍�֐�.
void CPlayer::AvoidMove()
{
	// ��𒆂���Ȃ���ΏI��.
	if( m_IsDuringAvoid == false ) return;

	// �X�e�B�b�N�̌X���������Ɍ���.
	m_vRotation.y = atan2f( m_AvoidVector.x, m_AvoidVector.z );
	// �J�����̊p�x�Ƒ������킹��.
	m_vRotation.y += m_pCamera->GetRadianX();

	// ��]���ňړ�.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;

	// ��]�@�A�j���[�V�����ݒ�����.
	m_vRotation.z -= 0.1f;

	D3DXVECTOR3 length = m_OldPosition - m_vPosition;
	// �ړ��������v�Z.
	float moveDistance = D3DXVec3Length( &length );
	// �ړ����������ȉ��Ȃ�I��.
	if( moveDistance <= m_Parameter.AvoidMoveDistance ) return;
	m_IsDuringAvoid = false;	// ��𒆂���Ȃ�����.
	m_vRotation.z = 0.0f;	// ��]�@�A�j���[�V�����ݒ�����.
}

// �U���A�j���[�V����.
void CPlayer::AttackAnimation()
{
	// �L���[����Ȃ�I��.
	if( m_AttackDataQueue.empty() == true ) return;
	if( m_AttackDataQueue.front().Frame >= m_AttackDataQueue.front().EndFrame ){
		m_AttackDataQueue.pop();	// �L���[�̐擪�����o��.
		// �L���[����Ȃ�I��.
		if( m_AttackDataQueue.empty() == true ){
			// ���ꂪ�Ō�̍U���Ȃ̂ŁA�U���J�E���g��0�ɂ���.
			m_AttackComboCount = 0;	
			// �A�j���[�V������ҋ@�ɐݒ�.
			SetAnimation( enAnimNo::Dead );
			return;
		}
		// �V�����A�j���[�V�������Z�b�g����.
		SetAnimation( m_AttackDataQueue.front().AnimNo );
	}
	m_AttackDataQueue.front().Frame += 0.01;	// �t���[���̍X�V.
}

// �A�j���[�V�����ݒ�.
void CPlayer::SetAnimation( const enAnimNo& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == m_OldAnimNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimSet( static_cast<int>(animNo) );
}

// �U���A�j���[�V�����t���[���̐ݒ�.
void CPlayer::SetAttackFrameList()
{
	if( m_pSkinMesh == nullptr ) return;
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(3) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(4) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(1) );
}

// �U���̒ǉ����ł�����.
bool CPlayer::IsPushAttack()
{
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackComboMax ) return false;
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackQueueMax ) return false;
	if( m_AttackDataQueue.empty() == false ){
		// �L���[�Ƀf�[�^�������Ă�����.
		// �o�߃t���[�����U���L���t���[�����傫����ΏI��.
		if( m_AttackDataQueue.front().Frame >= 
			m_AttackDataQueue.front().EnabledEndFrame ) return false;
	}

	SAttackData tmpAttackData;	// ���f�[�^��p��.
	switch( m_AttackComboCount )
	{
	case 1:	// �U��1.
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Wait1;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[0];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(0);
		// �ŏ��̓A�j���[�V������ݒ肷��.
		SetAnimation( tmpAttackData.AnimNo );	
		break;
	case 2:	// �U��2.
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Happy;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[1];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(4);
		break;
	case 3:	// �U��3.
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Wait;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[2];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(1);
		break;
	default:
		break;
	}
	// �L���[�Ƀf�[�^��}��.
	m_AttackDataQueue.push( tmpAttackData );
	return true;
}

// ���C�t�v�Z�֐�.
void CPlayer::LifeCalculation( const std::function<void(float&)>& proc )
{	
	proc( m_Parameter.Life );
}

// �����蔻��̐ݒ�.
bool CPlayer::ColliderSetting()
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

// �G�f�B�b�g�p�̕`��֐�.
void CPlayer::EditRender()
{
#if _DEBUG
	ImGui::SetNextWindowSize( ImVec2(440.0f,470.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( ImVec2(WND_W-440,0.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();
	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"�v���C���[�̐ݒ�", &isOpen );

	// �e�p�����[�^�̐ݒ�.
	ImGui::InputFloat( u8"�ړ����x", &m_Parameter.MoveSpeed );
	ImGui::InputFloat( u8"�̗�", &m_Parameter.Life );
	ImGui::InputFloat( u8"�U����", &m_Parameter.AttackPower );
	ImGui::InputInt( u8"���G����", &m_Parameter.InvincibleTime );
	ImGui::InputInt( u8"�U���R���{�ő吔", &m_Parameter.AttackComboMax );
	ImGui::InputInt( u8"�U���L���[�ǉ��ő吔", &m_Parameter.AttackQueueMax );
	ImGui::InputFloat( u8"����̈ړ�����", &m_Parameter.AvoidMoveDistance );
	ImGui::InputFloat( u8"���p�̈ړ����x", &m_Parameter.AvoidMoveSpeed );
	ImGui::InputFloat( u8"�J�����̈ړ����x", &m_Parameter.CameraMoveSpeed );
	ImGui::InputFloat( u8"�J�����̋���", &m_Parameter.CameraDistance );
	ImGui::InputFloat( u8"�J�����̍���", &m_Parameter.CameraHeight );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W X", &m_Parameter.SphereAdjPos.x );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Y", &m_Parameter.SphereAdjPos.y );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Z", &m_Parameter.SphereAdjPos.z );
	ImGui::InputFloat( u8"�X�t�B�A�̒������a", &m_Parameter.SphereAdjRadius );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Parameter );
		if( s_Success.IsSucceeded == true ){
			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_Parameter );
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.

	ImGui::End();
#endif	// #if _DEBUG.
}

// �f�o�b�O�p�̕`��.
void CPlayer::DebugRender()
{
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*-1, 0.0f } );
	CDebugText::Render( "- Player Parameter -" );

	// ���W�̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*0 ,0.0f } );
	CDebugText::Render( "----- Position -----" );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*1, 0.0f } );
	CDebugText::Render( "Pos_x : ", m_vPosition.x );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*2, 0.0f } );
	CDebugText::Render( "Pos_y : ", m_vPosition.y );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*3, 0.0f } );
	CDebugText::Render( "Pos_z : ", m_vPosition.z );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*5, 0.0f } );
	CDebugText::Render( "----- Rotation -----" );

	// ��]�̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*6, 0.0f } );
	CDebugText::Render( "Rot_x : ", m_vRotation.x );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*7, 0.0f } );
	CDebugText::Render( "Rot_y : ", m_vRotation.y );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*8, 0.0f } );
	CDebugText::Render( "Rot_z : ", m_vRotation.z );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*10, 0.0f } );
	CDebugText::Render( "----- Parameter ----" );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*11, 0.0f } );
	CDebugText::Render( "LifePoint : ", m_Parameter.Life );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*12, 0.0f } );
	CDebugText::Render( "AttackPower : ", m_Parameter.AttackPower );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*13, 0.0f } );
	CDebugText::Render( "InvincibleTime : ", m_Parameter.InvincibleTime );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*15, 0.0f } );
	CDebugText::Render( "----- Animation ----" );
	
	// �A�j���[�V�����ԍ��̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*16, 0.0f } );
	CDebugText::Render( "Now_AnimationNo : ", (int)m_NowAnimNo );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*17, 0.0f } );
	CDebugText::Render( "Old_AnimationNo : ", (int)m_OldAnimNo );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*19, 0.0f } );
	CDebugText::Render( "------ Other -------" );

	// �U���J�E���g�̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*20, 0.0f } );
	CDebugText::Render( "AttackComboCount : ", m_AttackComboCount );

	// ��𒆂�.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*21, 0.0f } );
	CDebugText::Render( "IsDuringAvoid : ", m_IsDuringAvoid==true?"true":"false" );
}