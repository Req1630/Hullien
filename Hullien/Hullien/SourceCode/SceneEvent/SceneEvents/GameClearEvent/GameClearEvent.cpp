#include "GameClearEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/************************************
*	�N���A�C�x���g�N���X.
**/
CGameClearEvent::CGameClearEvent()
{
	m_IsEventEnd = false;
}

CGameClearEvent::~CGameClearEvent()
{
}

// �ǂݍ��݊֐�.
bool CGameClearEvent::Load()
{
	return true;
}

// �X�V�֐�.
void CGameClearEvent::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// �`��֐�.
void CGameClearEvent::Render()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameClearEvent -");
}

void CGameClearEvent::SpriteRender()
{
}
