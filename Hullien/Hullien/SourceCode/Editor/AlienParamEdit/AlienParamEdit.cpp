#include "AlienParamEdit.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFOParam.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Character\Alien\AlienList.h"
#include "..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"

#include "..\..\GameObject\Actor\Character\Alien\EditAlien\Alien_A\EditAlien_A.h"

#include <filesystem>

namespace fs = std::filesystem;

CAlienParamEdit::CAlienParamEdit()
	: m_AlienParamList		()
	, m_pSkinMeshs			()
	, m_AlienPathList		()
	, m_AlienNameList		()
	, m_AlienNameColorList	( 4 )
	, m_pEditAlien			( nullptr )
	, m_Index				( 0 )

{
	m_pEditAlien = std::make_unique<CEditAlienA>();
	m_AlienNameColorList =
	{
		D3DXVECTOR4( 0.2f, 0.8f, 0.2f, 1.0f ),
		D3DXVECTOR4( 0.8f, 0.2f, 0.2f, 1.0f ),
		D3DXVECTOR4( 0.2f, 0.2f, 0.8f, 1.0f ),
		D3DXVECTOR4( 0.8f, 0.8f, 0.2f, 1.0f ),
	};
}

CAlienParamEdit::~CAlienParamEdit()
{
}

// �������֐�.
bool CAlienParamEdit::Init()
{
	if( FileAllReading()		== false ) return false;
	if( GetModel()				== false ) return false;
	if( m_pEditAlien->Init()	== false ) return false;
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
	if( m_pSkinMeshs.at(m_Index) == nullptr ) return;

	m_pSkinMeshs[m_Index]->SetPosition( { 0.0f, 0.0f, 0.0f } );
	m_pSkinMeshs[m_Index]->SetRotation( { 0.0f, 0.0f, 0.0f } );
	m_pSkinMeshs[m_Index]->SetScale( { 1.0f, 1.0f, 1.0f } );
	m_pSkinMeshs[m_Index]->SetColor( m_AlienNameColorList[m_Index] );
	m_pSkinMeshs[m_Index]->SetAnimSpeed( 0.01 );
	m_pSkinMeshs[m_Index]->SetRasterizerState( ERS_STATE::Back );
	m_pSkinMeshs[m_Index]->Render();
	m_pSkinMeshs[m_Index]->SetRasterizerState( ERS_STATE::None );
}

// �S�t�@�C���̓ǂݍ���.
bool CAlienParamEdit::FileAllReading()
{
	m_AlienPathList = CFileManager::TextLoading( FILE_PATH );
	
	for( const auto& f : m_AlienPathList ){
		const fs::path fileName = f;	// �t�@�C���l�[�����p�X�ɐݒ�.
		// �F���l�̖��O�ɕϊ����ă��X�g�ɒǉ�.
		m_AlienNameList.emplace_back( fileName.stem().string() );
		SAlienParam alienParam = {};
		// �o�C�i���ǂݍ���.
		if( FileReading( f.c_str(), alienParam ) == false ) return false; 
		m_AlienParamList.emplace_back( alienParam );	// �p�����[�^���X�g�ɒǉ�.
	}

	return true;
}

// �t�@�C���̓ǂݍ���.
bool CAlienParamEdit::FileReading( const char* fileName, SAlienParam& param )
{
	return CFileManager::BinaryReading( fileName, param );
}

// �t�@�C���̏�������.
bool CAlienParamEdit::FileWriting( const char* fileName, SAlienParam& param )
{
	return CFileManager::BinaryWriting( fileName, param );
}

// �X�|�[�����̕`��.
void CAlienParamEdit::SpawnParamRender( const int& index )
{
	if( m_AlienParamList.empty() == true ) return;

	auto& s = m_AlienParamList[index];

	EAlienList alienType = static_cast<EAlienList>(index);

	ImGui::PushItemWidth(200.0f);

	//  �e�p�����[�^�̐ݒ�.
	ImGui::InputFloat( u8"�ړ����x", &s.MoveSpeed );
	ImGui::InputFloat( u8"�̗�", &s.LifeMax );
	ImGui::InputFloat( u8"�U����", &s.AttackPower );
	ImGui::InputInt( u8"���G����", &s.InvincibleTime );

	ImGui::InputFloat( u8"�ړ���]���x", &s.RotationalSpeed );
	ImGui::InputFloat( u8"�X�|�[�����̃X�P�[���̉��Z�l", &s.SpawnScaleAddValue );
	ImGui::InputFloat( u8"�X�|�[�����̍~�����x", &s.SpawnDownSpeed );
	ImGui::InputFloat( u8"�}�U�[�V�b�v�ɏ㏸���鎞�̃X�P�[���̌��Z�l", &s.MotherShipUpScaleSubValue );
	ImGui::InputInt( u8"�ҋ@����", &s.WaitTime );
	ImGui::InputInt( u8"�m�b�N�o�b�N�̎���", &s.KnockBackTime );

	ImGui::InputFloat( u8"���S�J�E���g�̉��Z�l", &s.DeadCountAddValue );
	ImGui::InputFloat( u8"���S���̃X�P�[���̌��Z�l", &s.DeadScaleSubValue );

	switch( alienType )
	{
	case EAlienList::Ada:
	case EAlienList::A:
		m_Index = index / 2;
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
		m_Index = index / 2;
		break;
	case EAlienList::C:
		m_Index = index / 2;
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
		m_Index = index / 2+1;
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

// ���f���̏�����.
bool CAlienParamEdit::GetModel()
{
	//�ǂݍ��ރ��f�����ݒ�.
	const char* modelNames[] =
	{
		A_MODEL_NAME,
		B_MODEL_NAME,
		C_MODEL_NAME,
		D_MODEL_NAME,
	};
	const int modelMax = sizeof(modelNames) / sizeof(modelNames[0]);

	// �e���̐ݒ�.
	for( int no = 0; no < modelMax; no++ )
	{
		m_pSkinMeshs.emplace_back();
		if( CMeshResorce::GetSkin( m_pSkinMeshs[no], modelNames[no] ) == false ) return false;
		m_pSkinMeshs.back()->ChangeAnimSet_StartPos( 0, 0.0 );
	}

	return true;
}