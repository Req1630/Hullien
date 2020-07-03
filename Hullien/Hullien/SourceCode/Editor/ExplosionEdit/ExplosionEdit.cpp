#include "ExplosionEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CExplosionEdit::CExplosionEdit()
	: m_ExplosionParam	()
{
}

CExplosionEdit::~CExplosionEdit()
{
}

// �������֐�.
bool CExplosionEdit::Init()
{
	if( FileReading() == false ) return false;
	return true;
}

// �`��֐�.
void CExplosionEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE, ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( RENDER_POSITION, ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();

	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"�����p�����[�^�[�ݒ�", &isOpen );

	ImGui::InputFloat( u8"������", &m_ExplosionParam.AttackPower );
	ImGui::InputFloat( u8"��������", &m_ExplosionParam.ExplosionTime );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : X", &m_ExplosionParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : Y", &m_ExplosionParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : Z", &m_ExplosionParam.SphereAdjPos.z );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���a", &m_ExplosionParam.SphereAdjRadius );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) s_success.IsSucceeded = FileReading();
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) s_success.IsSucceeded = FileWriting();
	ImGui::SameLine();
	s_success.Render();

	ImGui::End();
}

// �t�@�C���̓ǂݍ���.
bool CExplosionEdit::FileReading()
{
	return CFileManager::BinaryReading( FILE_PATH, m_ExplosionParam );
}

// �t�@�C���̏�������.
bool CExplosionEdit::FileWriting()
{
	return CFileManager::BinaryWriting( FILE_PATH, m_ExplosionParam );
}