#include "Editor.h"
#include "..\..\..\Editor\ControllerEdit\ControllerEdit.h"
#include "..\..\SceneManager\SceneManager.h"

CEditor::CEditor( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )

	, m_pControllerEdit	( std::make_unique<CControllerEdit>() )
{
}

CEditor::~CEditor()
{
}

// �Ǎ��֐�.
bool CEditor::Load()
{
	return true;
}

// �X�V�֐�.
void CEditor::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
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

	// ImGui�ŏI�`��.
	CImGuiManager::Render();
}