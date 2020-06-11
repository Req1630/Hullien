#include "SpawnEdit.h"
#include "..\..\GameObject\SpawnUFO\SpawnUFOParam.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Utility\FileManager\FileManager.h"

CSpawnEdit::CSpawnEdit()
	: m_SpawnPramList		()
	, m_pStaticMesh			( nullptr )
	, m_IsSucceeded			( false )
	, m_SucceedRenderCount	( 0 )
	, m_Index				( 0 )
{
}

CSpawnEdit::~CSpawnEdit()
{
}

// �������֐�.
bool CSpawnEdit::Init()
{
	if( m_pStaticMesh != nullptr ) return true;
	if( m_pStaticMesh == nullptr ){
		m_pStaticMesh = CMeshResorce::GetStatic("SpawnPoint");
	}
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// �`��֐�.
void CSpawnEdit::Render()
{
	if( m_pStaticMesh == nullptr ) return;
	if( m_SpawnPramList.empty() == false ){
		m_pStaticMesh->SetPosition( m_SpawnPramList[m_Index].Position );
		m_pStaticMesh->SetScale( 0.5f );
		m_pStaticMesh->Render();
	}

	ImGui::SetNextWindowSize( WINDOW_SIZE, ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( RENDER_POSITION, ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();

	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"�X�|�[�����ݒ�", &isOpen );

	std::vector<std::string> tagNameList;
	tagNameList.emplace_back("Index");
	for( size_t i = 0; i < m_SpawnPramList.size(); i++ ){
		tagNameList.emplace_back("SpawnNo." + std::to_string(i) );
	}
	if( ImGui::BeginTabBar( "TabBarID" ) == true ){
		for( size_t i = 0; i < tagNameList.size(); i++ ){
			if( ImGui::BeginTabItem( tagNameList[i].c_str() ) == false ) continue;
			if( i == 0 ){
				IndexRender();
			} else {
				SpawnParamRender( i-1 );
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

// �t�@�C���̓ǂݍ���.
bool CSpawnEdit::FileReading()
{
	return CFileManager::BinaryVectorReading( FILE_PATH, m_SpawnPramList );
}

// �t�@�C���̏�������.
bool CSpawnEdit::FileWriting()
{
	return CFileManager::BinaryVectorWriting( FILE_PATH, m_SpawnPramList );
}

// �e�^�O�̕`��.
void CSpawnEdit::TagRender()
{
	// �^�O���X�g�̐ݒ�.
	std::vector<std::string> tagNameList;
	tagNameList.emplace_back("Index");
	for( size_t i = 0; i < m_SpawnPramList.size(); i++ ){
		tagNameList.emplace_back("SpawnNo." + std::to_string(i) );
	}

	// �e�^�O�̕`��.
	if( ImGui::BeginTabBar( "TabBarID" ) == false ) return;
	for( size_t i = 0; i < tagNameList.size(); i++ ){
		if( ImGui::BeginTabItem( tagNameList[i].c_str() ) == false ) continue;

		if( i == 0 ){
			// �C���f�b�N�X�̕`��.
			IndexRender();
		} else {
			// �X�|�[�����̕`��.
			SpawnParamRender( i-1 );
		}
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

// �C���f�b�N�X�̕`��.
void CSpawnEdit::IndexRender()
{
	ImGui::TextWrapped( u8"�G�͊e�ݒ肵���ő�E�ŏ��̒l���A" );
	ImGui::TextWrapped( u8"�����_���Ŏ擾���X�|�[������" );

	ImGui::Separator();

	ImGui::Text( u8"Position : �X�|�[���|�C���g�̍��W" );
	ImGui::Text( u8"SpawnTime : �X�|�[�����鎞��" );

	ImGui::Separator();

	ImGui::Text( u8"���݂̃X�|�[������" );
	ImGui::SameLine();
	if( ImGui::Button(u8"���₷") ) m_SpawnPramList.emplace_back();
	ImGui::SameLine();
	if( ImGui::Button(u8"���炷") ) m_SpawnPramList.pop_back();

	ImGui::Separator();

	if( ImGui::Button(u8"�Ǎ�") ) m_IsSucceeded = FileReading();
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") ) m_IsSucceeded = FileWriting();
	ImGui::SameLine();
	if( m_IsSucceeded == true ){
		ImGui::Text( u8"����" );
		m_SucceedRenderCount++;
		if( m_SucceedRenderCount == 60 ){
			m_SucceedRenderCount = 0;
			m_IsSucceeded = false;
		}
	}
}

// �X�|�[�����̕`��.
void CSpawnEdit::SpawnParamRender( const int& index )
{
	if( m_SpawnPramList.empty() == true ) return;

	auto& s = m_SpawnPramList[index];
	bool hasMovedSillider = false;
	auto checkSetIndex = [&]()
	{
		if( hasMovedSillider == false ) return;
		m_Index = index;
	};

	float val = 7.0f;
	ImGui::PushItemWidth(100); // ���ꂩ����UI�p�[�c�̕���70�ŌŒ肵�܂�.
	hasMovedSillider = ImGui::SliderFloat( "X", &s.Position.x, -val, val ); 
	ImGui::SameLine();
	checkSetIndex();

	hasMovedSillider = ImGui::SliderFloat( "Y", &s.Position.y, -val, val ); 
	ImGui::SameLine();
	checkSetIndex();

	hasMovedSillider = ImGui::SliderFloat( "Z", &s.Position.z, -val, val );
	checkSetIndex();

	hasMovedSillider = ImGui::SliderInt( "SpawnTime", &s.SpawnTime, 0, 100 );
	checkSetIndex();

	ImGui::PopItemWidth();
}