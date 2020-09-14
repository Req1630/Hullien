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
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::Begin( u8"�����p�����[�^�[�ݒ�" );

	ImGui::PushItemWidth(200.0f);

	ImGui::InputFloat( u8"������", &m_ExplosionParam.AttackPower );
	ImGui::InputFloat( u8"�������x", &m_ExplosionParam.ExplosionSpeed );
	ImGui::InputFloat( u8"�X�t�B�A�̍ő唼�a", &m_ExplosionParam.SphereMaxRadius );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : X", &m_ExplosionParam.SphereAdjPos.x );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : Y", &m_ExplosionParam.SphereAdjPos.y );
	ImGui::InputFloat( u8"�X�t�B�A�̒����p���W : Z", &m_ExplosionParam.SphereAdjPos.z );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) s_success.IsSucceeded = FileReading();
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) s_success.IsSucceeded = FileWriting();
	ImGui::SameLine();
	s_success.Render();

	ImGui::PopItemWidth();

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