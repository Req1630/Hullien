#include "AlienParamEdit.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFOParam.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Character\Alien\AlienList.h"

#include <filesystem>

namespace fs = std::filesystem;

CAlienParamEdit::CAlienParamEdit()
	: m_AlienParamList	()
	, m_Index			( 0 )
{
}

CAlienParamEdit::~CAlienParamEdit()
{
}

// �������֐�.
bool CAlienParamEdit::Init()
{
	if( FileAllReading() == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienParamEdit::Update()
{
}

// �`��֐�.
void CAlienParamEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::GetWindowSize();
	ImGui::Begin( u8"�F���l�p�����[�^�ݒ�" );

	if( ImGui::BeginTabBar("TabBarID") == true ){
		for( size_t i = 0; i < m_AlienNameList.size(); i++ ){
			if( ImGui::BeginTabItem( m_AlienNameList[i].c_str() ) == false ) continue;
			SpawnParamRender( i );
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

// ���f���̕`��.
void CAlienParamEdit::ModelRender()
{
}

// �S�t�@�C���̓ǂݍ���.
bool CAlienParamEdit::FileAllReading()
{
	m_AlienPathList = CFileManager::TextLoading( FILE_PATH );
	
	for( const auto& f : m_AlienPathList ){
		const fs::path fileName = f;	// �t�@�C���l�[�����p�X�ɐݒ�.
		// �F���l�̖��O�ɕϊ����ă��X�g�ɒǉ�.
		m_AlienNameList.emplace_back( fileName.stem().string() );
		CAlien::SAlienParam alienParam = {};
		// �o�C�i���ǂݍ���.
		if( FileReading( f.c_str(), alienParam ) == false ) return false; 
		m_AlienParamList.emplace_back( alienParam );	// �p�����[�^���X�g�ɒǉ�.
	}

	return true;
}

// �t�@�C���̓ǂݍ���.
bool CAlienParamEdit::FileReading( const char* fileName, CAlien::SAlienParam& param )
{
	return CFileManager::BinaryReading( fileName, param );
}

// �t�@�C���̏�������.
bool CAlienParamEdit::FileWriting( const char* fileName, CAlien::SAlienParam& param )
{
	return CFileManager::BinaryWriting( fileName, param );
}

// �X�|�[�����̕`��.
void CAlienParamEdit::SpawnParamRender( const int& index )
{
	if( m_AlienParamList.empty() == true ) return;

	auto& s = m_AlienParamList[index];
	bool hasMovedSillider = false;
	auto checkSetIndex = [&]()
	{
		if (hasMovedSillider == false) return;
		m_Index = index;
	};

	EAlienList alienType = static_cast<EAlienList>(index);

	ImGui::PushItemWidth(200.0f);

	//  �e�p�����[�^�̐ݒ�.
	ImGui::InputFloat( u8"�ړ����x", &s.MoveSpeed );
	ImGui::InputFloat( u8"�̗�", &s.LifeMax );
	ImGui::InputFloat( u8"�U����", &s.AttackPower );
	ImGui::InputInt( u8"�ҋ@����", &s.WaitTime );
	ImGui::InputInt( u8"���G����", &s.InvincibleTime );
	ImGui::InputFloat( u8"�ړ���]���x", &s.RotationalSpeed );
	ImGui::InputFloat( u8"�X�|�[�����̃��f�����߉��Z�l", &s.ModelAlphaAddValue );
	ImGui::InputFloat( u8"�X�|�[�����̃��f�����ߌ��Z�l", &s.ModelAlphaSubValue );

	switch( alienType )
	{
	case EAlienList::A:
		break;
	case EAlienList::Ada:
		break;
	case EAlienList::B:
	case EAlienList::Bda:
		ImGui::InputFloat( u8"�v���C���[��_���͈�", &s.PlayerAimLenght );
		ImGui::InputFloat( u8"�U�����鋗��", &s.AttackLenght );
		ImGui::InputFloat( u8"�U�����̏�����]��", &s.AttackRotInitPower );
		ImGui::InputFloat( u8"�U�����̉�]��", &s.AttackRotPower );
		ImGui::InputFloat( u8"�U�����̉�]���Z�l", &s.AttackRotAddValue );
		ImGui::InputFloat( u8"�U���ړ����x", &s.AttackMoveSpeed );
		ImGui::InputFloat( u8"�U���ړ��͈�", &s.AttackMoveRange );
		break;
	case EAlienList::C:
		break;
	case EAlienList::D:
		ImGui::InputFloat( u8"���[�U�[�̈ړ����x", &s.LaserMoveSpeed );
		ImGui::InputFloat( u8"��Ⴢ̎���", &s.ParalysisTime );
		ImGui::InputFloat( u8"�U�����鋗��", &s.AttackLenght );
		ImGui::InputFloat( u8"�U�����͈͕̔\���̒����p", &s.AttackRangeSpritePosY );
		ImGui::InputFloat( u8"�U�����͈͕̔\���̉摜�T�C�Y", &s.AttackRangeSpriteScale );
		ImGui::InputFloat( u8"�U�����͈͕̔\���̃A���t�@���������l", &s.AttackRangeAddValue );
		ImGui::InputFloat( u8"�U�����͈͕̔\���̃A���t�@���������l", &s.AttackRangeSubValue );
		ImGui::InputFloat( u8"�x�W�F�Ȑ��̈�ڑ�����W�̋���", &s.ControlPointOneLenght );
		ImGui::InputFloat( u8"�x�W�F�Ȑ��̈�ڑ�����W��y���W�̋���", &s.ControlPointOneLenghtY );
		ImGui::InputFloat( u8"�x�W�F�Ȑ��̓�ڑ�����W�̋���", &s.ControlPointTwoLenght );
		ImGui::InputFloat( u8"�x�W�F�Ȑ��̓�ڑ�����W��y���W�̋���", &s.ControlPointTwoLenghtY );
		break;
	default:
		break;
	}

	ImGui::InputFloat( u8"�X�t�B�A�̒������W X", &s.SphereAdjPos.x );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Y", &s.SphereAdjPos.y );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Z", &s.SphereAdjPos.z );
	ImGui::InputFloat( u8"�X�t�B�A�̒������a", &s.SphereAdjRadius );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) 
		s_success.IsSucceeded = FileReading( m_AlienPathList[index].c_str(), s );
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) 
		s_success.IsSucceeded = FileWriting( m_AlienPathList[index].c_str(), s );
	ImGui::SameLine();
	s_success.Render();


	ImGui::PopItemWidth();
}