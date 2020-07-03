#include "Editor.h"
#include "..\..\SceneManager\SceneManager.h"
#include "..\..\..\Editor\ControllerEdit\ControllerEdit.h"
#include "..\..\..\Editor\SpawnEdit\SpawnEdit.h"
#include "..\..\..\Editor\AlienParamEdit\AlienParamEdit.h"
#include "..\..\..\Editor\ExplosionEdit\ExplosionEdit.h"

CEditor::CEditor( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )

	, m_pControllerEdit	( nullptr )
	, m_pSpawnEdit		( nullptr )
	, m_pAlienParamEdit	( nullptr )
	, m_pExplosionEdit	( nullptr ) 
{
	m_pControllerEdit = std::make_unique<CControllerEdit>();
	m_pSpawnEdit = std::make_unique<CSpawnEdit>();
	m_pAlienParamEdit = std::make_unique<CAlienParamEdit>();
	m_pExplosionEdit = std::make_unique<CExplosionEdit>();
}

CEditor::~CEditor()
{
}

// �Ǎ��֐�.
bool CEditor::Load()
{
	if( m_pSpawnEdit->Init() == false ) return false;
	if( m_pAlienParamEdit->Init() == false ) return false;
	if( m_pExplosionEdit->Init() == false ) return false;
	return true;
}

// �X�V�֐�.
void CEditor::Update()
{
	if( ( GetAsyncKeyState('E') & 0x8000 ) &&
		( GetAsyncKeyState('D') & 0x8000 ) ){
		if( !(GetAsyncKeyState('W') & 0x8000 ) ) return;
		m_pSceneManager->NextSceneMove();
	}
}

// �`��֐�.
void CEditor::Render()
{
	// ImGui�̃t���[��������.
	CImGuiManager::SetingNewFrame();

	// �R���g���[���[�G�f�B�b�g�̕`��.
	m_pControllerEdit->Render();
	m_pSpawnEdit->Render();
	m_pAlienParamEdit->Render();
	m_pExplosionEdit->Render();

	// ImGui�ŏI�`��.
	CImGuiManager::Render();
}