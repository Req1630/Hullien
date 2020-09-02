#include "ItemEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\GameObject\Actor\Item\ItemList.h"

CItemEdit::CItemEdit()
	: m_Prameter		()
	, m_EachItemEffect	()
{
}

CItemEdit::~CItemEdit()
{
}

// �������֐�.
bool CItemEdit::Init()
{
	if( FileReading() == false ) return false;
	return true;
}

// �`��֐�.
void CItemEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE, ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( RENDER_POSITION, ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();

	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"�A�C�e���ݒ�", &isOpen );

	if( ImGui::BeginTabBar( "TabBarID" ) == true ){
		for( int i = 0; i < (int)TAG_LIST.size(); i++ ){
			if( ImGui::BeginTabItem( TAG_LIST[i].c_str() ) == false ) continue;
			TagRender( i );
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

// �e�^�O�̕`��.
void CItemEdit::TagRender( const int& index )
{
	EItemList itemNo = static_cast<EItemList>(index);
	switch( itemNo )
	{
	case EItemList::None:
		ParamRender();
		return;
	case EItemList::LifeRecovery:
		ImGui::InputFloat( u8"�񕜗�", &m_EachItemEffect.LifeRecoveryValue );
		break;
	case EItemList::SPEffectTime:
		ImGui::InputFloat( u8"����\�͉񕜗�", &m_EachItemEffect.SPRecoveryValue );
		ImGui::InputFloat( u8"����\�͉񕜎���", &m_EachItemEffect.SPRecoveryTime );
		break;
	case EItemList::AttackUp:
		ImGui::InputFloat( u8"�U����", &m_EachItemEffect.AttackPower );
		ImGui::InputFloat( u8"�U����UP����", &m_EachItemEffect.AttackUpTime );
		break;
	case EItemList::MoveSpeedUp:
		ImGui::InputFloat( u8"�ړ����x", &m_EachItemEffect.MovePower );
		ImGui::InputFloat( u8"�ړ����xUP����", &m_EachItemEffect.MoveUpTime );
		break;
	default:
		break;
	}
	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect );
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryWriting( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect );
	ImGui::SameLine();
	s_success.Render();
}

// �p�����[�^�̕`��.
void CItemEdit::ParamRender()
{
	ImGui::InputFloat( u8"���������l",					&m_Prameter.InitAccelerationValue );
	ImGui::InputFloat( u8"�����d��",					&m_Prameter.InitGravity );
	ImGui::InputInt(   u8"�o�E���h�ő吔",				&m_Prameter.BoundCountMax );
	ImGui::InputFloat( u8"���f���T�C�Y�ő�",			&m_Prameter.ModelScaleMax );
	ImGui::InputFloat( u8"���f���T�C�Y���Z�l",			&m_Prameter.ModelScaleAddValue );
	ImGui::InputFloat( u8"�ŏI�I�ȕ`��̍���",			&m_Prameter.FinalRenderHeight );
	ImGui::InputFloat( u8"�ŏI�I�Ȉړ����x",			&m_Prameter.FinalMoveSpeed );
	ImGui::InputFloat( u8"��]���x",					&m_Prameter.RotationSpeed );
	ImGui::InputFloat( u8"�A�N�e�B�u����",				&m_Prameter.ActiveTime );
	ImGui::InputFloat( u8"�����鎞��",					&m_Prameter.DisappearTime );
	ImGui::InputFloat( u8"�_�ŉ��Z�l",					&m_Prameter.FlashingAddValue );
	ImGui::InputFloat( u8"���f�����ߒl�ő�",			&m_Prameter.ModelAlphaMax );
	ImGui::InputFloat( u8"�q�b�g���̃G�t�F�N�g����",	&m_Prameter.HitEffectTime );

	static CImGuiManager::SSuccess s_success = {};
	if( ImGui::Button(u8"�Ǎ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) 
		s_success.IsSucceeded = CFileManager::BinaryWriting( ITEM_PARAM_FILE_PATH, m_Prameter );
	ImGui::SameLine();
	s_success.Render();
}

// �t�@�C���̓ǂݍ���.
bool CItemEdit::FileReading()
{
	if( CFileManager::BinaryReading( EACH_ITEM_EFFECT_FILE_PATH, m_EachItemEffect ) == false ) return false;
	if( CFileManager::BinaryReading( ITEM_PARAM_FILE_PATH, m_Prameter ) == false ) return false;
	return true;
}