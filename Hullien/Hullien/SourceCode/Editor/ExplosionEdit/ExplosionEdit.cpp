#include "ExplosionEdit.h"
#include "..\..\Utility\FileManager\FileManager.h"

CExplosionEdit::CExplosionEdit()
	: m_pExplosion		( nullptr )
	, m_ExplosionParam	()
{
	m_pExplosion = std::make_unique<CExplosion>();
}

CExplosionEdit::~CExplosionEdit()
{
}

// �������֐�.
bool CExplosionEdit::Init()
{
	if( FileReading() == false ) return false;
	if( m_pExplosion->Init() == false ) return false;
	m_pExplosion->SetExplosionParam( m_ExplosionParam );
	return true;
}

// �X�V�֐�.
void CExplosionEdit::Update()
{
	m_pExplosion->Update();
}

// �`��֐�.
void CExplosionEdit::Render()
{
	ImGui::SetNextWindowSize( WINDOW_SIZE );
	ImGui::SetNextWindowPos( RENDER_POSITION );
	ImGui::Begin( u8"�����p�����[�^�[�ݒ�" );

	ImGui::PushItemWidth(200.0f);

	if( ImGui::Button( u8"�Đ�" ) == true ){
		if( m_pExplosion->IsStop() == true ){
			m_pExplosion->Init();
			m_pExplosion->SetPosition( D3DXVECTOR3( 0.0f, 5.0f, 0.0f ) );
		}
	}

	ImGui::InputFloat( u8"������", &m_ExplosionParam.AttackPower );
	if( ImGui::InputFloat( u8"�������x", &m_ExplosionParam.ExplosionSpeed ) ){
		m_pExplosion->SetExplosionParam( m_ExplosionParam );
	}
	if( ImGui::InputFloat( u8"�X�t�B�A�̍ő唼�a", &m_ExplosionParam.SphereMaxRadius ) ){
		m_pExplosion->SetExplosionParam( m_ExplosionParam );
	}
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

// ���f���̕`��.
void CExplosionEdit::ModelRender()
{
	m_pExplosion->Render();
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