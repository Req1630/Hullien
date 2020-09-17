#include "GameStartEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include ".\..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\Character\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\Character\Girl\Girl.h"
#include "..\..\..\GameObject\Actor\Character\Alien\Alien_A\Alien_A.h"
#include "..\..\..\Camera\EvevtCamera\EvevtCamera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/***********************************
*	�X�^�[�g�C�x���g�N���X.
**/
CGameStartEvent::CGameStartEvent()
	: m_pGroundStage	( nullptr )
	, m_pSpawnUFO		( nullptr )
	, m_pPlayer			( nullptr )
	, m_pGirl			( nullptr )
	, m_pAlienA			( nullptr )
	, m_pEventCamera	( nullptr )
{
	m_pGroundStage	= std::make_shared<CGroundStage>();
	m_pSpawnUFO		= std::make_shared<CSpawnUFO>();
	m_pPlayer		= std::make_shared<CEventPlayer>();
	m_pGirl			= std::make_shared<CGirl>();
	m_pAlienA		= std::make_shared<CAlienA>();
	m_pEventCamera	= std::make_shared<CEventCamera>();
}

CGameStartEvent::~CGameStartEvent()
{
}

// �Ǎ��֐�.
bool CGameStartEvent::Load()
{
	if( m_pGroundStage->Init() == false ) return false;	// �n�ʂ̏�����.
	if( m_pSpawnUFO->Init() == false ) return false;	// UFO�̏�����.
	if( m_pPlayer->Init() == false ) return false;		// �v���C���[�̏�����.
	if( m_pGirl->Init() == false ) return false;		// ���̎q�̏�����.
	if( m_pAlienA->Init() == false ) return false;		// �F���lA�̏�����.
	m_IsEventEnd = false;
	return true;
}

// �X�V�֐�.
void CGameStartEvent::Update()
{
	CAlien::SAlienParam m_Param;
	m_Param.ModelAlphaAddValue = 0.1f;
	m_pAlienA->Spawn(m_Param, D3DXVECTOR3(10.0f, 0.0f, 0.0f));

	static D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 35.0f, 45.0f);
	D3DXVECTOR3 vLookPos = D3DXVECTOR3(vPos.x, vPos.y - 1.0f, vPos.z + 5.0f);
	float Speed = 0.05f;
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		vPos.z += static_cast<float>(D3DX_PI) * Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		vPos.z -= static_cast<float>(D3DX_PI) * Speed;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		vPos.y += static_cast<float>(D3DX_PI) * Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		vPos.y -= static_cast<float>(D3DX_PI) * Speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		vPos.x += static_cast<float>(D3DX_PI) * Speed;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		vPos.x -= static_cast<float>(D3DX_PI) * Speed;
	}
	m_pEventCamera->SetPosition( vPos );
	m_pEventCamera->SetLookPosition(vLookPos);
	CCameraManager::SetCamera( m_pEventCamera );

	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// �`��֐�.
void CGameStartEvent::Render()
{
	m_pGroundStage->Render();	// �X�e�[�W�̕`��.
	m_pSpawnUFO->Render();		// UFO�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pGirl->Render();			// ���̎q�̕`��.
	m_pAlienA->Render();		// �F��A�̕`��.
	DebugRender();
}

void CGameStartEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameStartEvent -");
}

