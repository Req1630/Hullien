#include "MotherShipUFOEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CMotherShipUFOEdit::CMotherShipUFOEdit()
	: m_pMotherShipUFO		( nullptr )
	, m_MotherShipUFOParam	()
{
	m_pMotherShipUFO = std::make_unique<CMotherShipUFO>();
}

CMotherShipUFOEdit::~CMotherShipUFOEdit()
{
}

// �������֐�.
bool CMotherShipUFOEdit::Init()
{
	if( m_pMotherShipUFO->Init() == false ) return false;
	if( CFileManager::BinaryReading( MOTHER_SHIP_UFO_PARAM_FILE_PATH, m_MotherShipUFOParam ) == false ) return false;
	return true;
}

// �X�V�֐�.
void CMotherShipUFOEdit::Update()
{
	m_pMotherShipUFO->Update();
}

// �`��֐�.
void CMotherShipUFOEdit::Render()
{

	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"�}�U�[�V�b�vUFO�ݒ�" );
	ImGui::PushItemWidth(200.0f);

	ImGui::InputFloat( u8"�������W : X",			&m_MotherShipUFOParam.Position.x );
	ImGui::InputFloat( u8"�������W : Y",			&m_MotherShipUFOParam.Position.y );
	ImGui::InputFloat( u8"�������W : Z",			&m_MotherShipUFOParam.Position.z );
	ImGui::InputFloat( u8"�����グ���",			&m_MotherShipUFOParam.AddPosYPower );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : X",&m_MotherShipUFOParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : Y",&m_MotherShipUFOParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : Z",&m_MotherShipUFOParam.SphereAdjPos.z );
	ImGui::InputFloat( u8"�X�t�B�A�̔��a",			&m_MotherShipUFOParam.CollisionRadius );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ){
		s_success.IsSucceeded = CFileManager::BinaryReading( MOTHER_SHIP_UFO_PARAM_FILE_PATH, m_MotherShipUFOParam );
		m_pMotherShipUFO->SetParameter( m_MotherShipUFOParam );
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ){
		s_success.IsSucceeded = CFileManager::BinaryWriting( MOTHER_SHIP_UFO_PARAM_FILE_PATH, m_MotherShipUFOParam );
		m_pMotherShipUFO->SetParameter( m_MotherShipUFOParam );
	}
	ImGui::SameLine();
	s_success.Render();

	ImGui::PopItemWidth();
	ImGui::End();
}

// ���f���̕`��.
void CMotherShipUFOEdit::ModelRender()
{
	m_pMotherShipUFO->Render();
}