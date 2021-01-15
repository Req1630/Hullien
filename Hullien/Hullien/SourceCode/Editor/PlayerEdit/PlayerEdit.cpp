#include "PlayerEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\Input\Input.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer				( nullptr )
	, m_pPlayerParam		()
	, m_AnimAdjDrggingParam	()
	, m_AttackAdjParam		()
{
	m_pPlayer = std::make_unique<CPlayer>();
}

CPlayerEdit::~CPlayerEdit()
{
}

// �������֐�.
bool CPlayerEdit::Init() 
{
	if( m_pPlayer->Init() == false ) return false;
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam ) == false )			return false;
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam ) == false )	return false;
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam ) == false )			return false;
	return true;
}

// �X�V�֐�.
void CPlayerEdit::Update()
{
	if( m_IsSetCamera == true ){
		m_pPlayer->Update();
	}
	if( CInput::IsMomentPress( EKeyBind::Back ) == true ){
		m_IsSetCamera = false;
		// �Q�[���p�b�h�̎g�p��������.
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	}
}

// �`��֐�.
void CPlayerEdit::Render() 
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"�v���C���[�̐ݒ�" );

	ImGui::PushItemWidth(200.0f);

	if( ImGui::TreeNode( u8"�p�����[�^�̐ݒ�" )){
		PlayerParamEdit();			ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�A�j���[�V�����̈������蒲���p�����[�^�̐ݒ�" )){
		AnimAdjDrggingParamEdit();	ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�U�������p�����[�^�̐ݒ�" )){
		AttackAdjParamEdit();		ImGui::TreePop();
	}
	
	
	m_pPlayer->SetParameter( m_pPlayerParam, true );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_pPlayerParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.
	ImGui::NewLine();
	if( ImGui::Button(u8"�v���r���[") == true ){
		m_IsSetCamera = true;
		// �Q�[���p�b�h�̎g�p����߂�.
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

// ���f���̕`��.
void CPlayerEdit::ModelRender() 
{
	m_pPlayer->Render();
}

// �G�t�F�N�g�̕`��.
void CPlayerEdit::EffectRender()
{
	m_pPlayer->EffectRender();
}

// �p�����[�^�̐ݒ�.
void CPlayerEdit::PlayerParamEdit()
{
	// �e�p�����[�^�̐ݒ�.
	CImGuiManager::DragFloat( u8"�ړ����x",						&m_pPlayerParam.MoveSpeed );
	CImGuiManager::DragFloat( u8"�̗�",							&m_pPlayerParam.LifeMax );
	CImGuiManager::DragFloat( u8"�U����",						&m_pPlayerParam.AttackPower );
	CImGuiManager::DragInt(   u8"���G����",						&m_pPlayerParam.InvincibleTime );
	CImGuiManager::DragFloat( u8"����\�͍ő�l",				&m_pPlayerParam.SpecialAbilityMax );
	CImGuiManager::DragFloat( u8"����\�͉񕜒l",				&m_pPlayerParam.SpecialAbilityValue );
	CImGuiManager::DragInt(   u8"�U���R���{�ő吔",				&m_pPlayerParam.AttackComboMax );
	CImGuiManager::DragInt(   u8"�U���L���[�ǉ��ő吔",			&m_pPlayerParam.AttackQueueMax );
	CImGuiManager::DragFloat( u8"����̈ړ�����",				&m_pPlayerParam.AvoidMoveDistance );
	CImGuiManager::DragFloat( u8"���p�̈ړ����x",				&m_pPlayerParam.AvoidMoveSpeed );
	CImGuiManager::DragFloat( u8"�J�����̈ړ����x",				&m_pPlayerParam.CameraMoveSpeed );
	CImGuiManager::DragFloat( u8"�J�����̋���",					&m_pPlayerParam.CameraDistance );
	CImGuiManager::DragFloat( u8"�J�����̍���",					&m_pPlayerParam.CameraHeight );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W X",			&m_pPlayerParam.SphereAdjPos.x );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Y",			&m_pPlayerParam.SphereAdjPos.y );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Z",			&m_pPlayerParam.SphereAdjPos.z );
	CImGuiManager::DragFloat( u8"�X�t�B�A�̒������a",			&m_pPlayerParam.SphereAdjRadius );
	CImGuiManager::DragFloat( u8"�m�b�N�o�b�N�ړ����x",			&m_pPlayerParam.HitKnocBackMoveSpeed );
	CImGuiManager::DragFloat( u8"��]�̋��e�͈�",				&m_pPlayerParam.ToleranceRadian );
	CImGuiManager::DragFloat( u8"��]���x",						&m_pPlayerParam.RotationSpeed );
	CImGuiManager::DragFloat( u8"�|�����킹��ړ��ʂ̉��Z�l",	&m_pPlayerParam.MoveSpeedMulAddValue );
	CImGuiManager::DragFloat( u8"�|�����킹��ړ��ʂ̍ő�l",	&m_pPlayerParam.MoveSpeedMulMaxValue );
	CImGuiManager::DragFloat( u8"�U���̍��G����",				&m_pPlayerParam.AttackSearcLenght );
	CImGuiManager::DragFloat( u8"��̍U���͈�",					&m_pPlayerParam.AttackFanRadian );
	CImGuiManager::DragFloat( u8"�G�̃m�b�N�o�b�N��",			&m_pPlayerParam.EnemyKnockBackPower );	
}

// �A�j���[�V�����̈������蒲���̐ݒ�.
void CPlayerEdit::AnimAdjDrggingParamEdit()
{
	if( ImGui::TreeNode( u8"�U��1" )){
		CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack1] );
		CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack1] );
		CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack1] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�U��2" )){
		CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack2] );
		CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack2] );
		CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack2] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�U��3" )){
		CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack3] );
		CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack3] );
		CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack3] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"���S" )){
		CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Dead] );
		CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Dead] );
		CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Dead] );
		ImGui::TreePop();
	}
}

// �U�������̐ݒ�.
void CPlayerEdit::AttackAdjParamEdit()
{
	if( ImGui::TreeNode( u8"�U��1" )){
		CImGuiManager::DragDouble(	u8"�L���I���t���[��",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_One-1] );
		CImGuiManager::DragDouble(	u8"�����蔻��L���I���t���[��",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_One-1] );
		CImGuiManager::DragFloat(	u8"�����蔻�蔼�a",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_One-1] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�U��2" )){
		CImGuiManager::DragDouble(	u8"�L���I���t���[��",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Two-1] );
		CImGuiManager::DragDouble(	u8"�����蔻��L���I���t���[��",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Two-1] );
		CImGuiManager::DragFloat(	u8"�����蔻�蔼�a",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Two-1] );
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�U��3" )){
		CImGuiManager::DragDouble(	u8"�L���I���t���[��",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Three-1] );
		CImGuiManager::DragDouble(	u8"�����蔻��L���I���t���[��",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Three-1] );
		CImGuiManager::DragFloat(	u8"�����蔻�蔼�a",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Three-1] );
		ImGui::TreePop();
	}

	CImGuiManager::DragFloat(	u8"�����蔻�蔼�a",				&m_AttackAdjParam.CollisionDistance );
	CImGuiManager::DragFloat(	u8"�U�����ĂȂ����̍��W : y",	&m_AttackAdjParam.CollInvalidPosition.y );

}