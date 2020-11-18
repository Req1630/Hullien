#include "PlayerEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer			( nullptr )
	, m_pPlayerParam	()
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
	if( CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam ) == false ) return false;
	return true;
}

// �X�V�֐�.
void CPlayerEdit::Update()
{
	m_pPlayer->Update();
}

// �`��֐�.
void CPlayerEdit::Render() 
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"�v���C���[�̐ݒ�" );

	ImGui::PushItemWidth(200.0f);

	// �e�p�����[�^�̐ݒ�.
	ImGui::InputFloat( u8"�ړ����x",		&m_pPlayerParam.MoveSpeed );
	ImGui::InputFloat( u8"�̗�",			&m_pPlayerParam.LifeMax );
	ImGui::InputFloat( u8"�U����",			&m_pPlayerParam.AttackPower );
	ImGui::InputInt( u8"���G����",			&m_pPlayerParam.InvincibleTime );
	ImGui::InputFloat( u8"����\�͍ő�l",	&m_pPlayerParam.SpecialAbilityMax );
	ImGui::InputFloat( u8"����\�͉񕜒l",	&m_pPlayerParam.SpecialAbilityValue );
	ImGui::InputInt( u8"�U���R���{�ő吔",	&m_pPlayerParam.AttackComboMax );
	ImGui::InputInt( u8"�U���L���[�ǉ��ő吔",	&m_pPlayerParam.AttackQueueMax );
	ImGui::InputFloat( u8"����̈ړ�����",		&m_pPlayerParam.AvoidMoveDistance );
	ImGui::InputFloat( u8"���p�̈ړ����x",	&m_pPlayerParam.AvoidMoveSpeed );
	ImGui::InputFloat( u8"�J�����̈ړ����x",	&m_pPlayerParam.CameraMoveSpeed );
	ImGui::InputFloat( u8"�J�����̋���",		&m_pPlayerParam.CameraDistance );
	ImGui::InputFloat( u8"�J�����̍���",		&m_pPlayerParam.CameraHeight );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W X",	&m_pPlayerParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Y",	&m_pPlayerParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Z",	&m_pPlayerParam.SphereAdjPos.z );
	ImGui::InputFloat( u8"�X�t�B�A�̒������a",		&m_pPlayerParam.SphereAdjRadius );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_pPlayerParam );
		if( s_Success.IsSucceeded == true ){
//			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_pPlayerParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.

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