#include "GameOverEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/************************************
*	�Q�[���I�[�o�[�C�x���g�N���X.
**/
CGameOverEvent::CGameOverEvent()
{
	m_IsEventEnd = false;
}

CGameOverEvent::~CGameOverEvent()
{
}

// �Ǎ��֐�.
bool CGameOverEvent::Load()
{
	return true;
}

// �X�V�֐�.
void CGameOverEvent::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// �`��֐�.
void CGameOverEvent::Render()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameOverEvent -");
}

void CGameOverEvent::SpriteRender()
{
}
