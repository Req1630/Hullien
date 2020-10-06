#include "GameClearEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventGirl\EventGirl.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventAlien\EventAlien_A\EventAlien_A.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/************************************
*	�N���A�C�x���g�N���X.
**/
CGameClearEvent::CGameClearEvent()
	: m_pGroundStage	( nullptr )
	, m_pSpawnUFO		( nullptr )
	, m_pPlayer			( nullptr )
	, m_pGirl			( nullptr )
	, m_pEventCamera	( nullptr )
	, m_pEventManager	( nullptr )
	, m_stPlayer		()
	, m_stGirl			()
	, m_stAlien			()
	, m_stCamera		()
	, m_vUFOPosition	( D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_vUFOScale		( D3DXVECTOR3(1.0f,1.0f,1.0f))
	, m_WaitCount		( 0 )
	, m_Gravity			( 0.0f )
	, m_Speed			( 0.0f ) 
	, m_NowScene		( 0 )
	, m_UFOScale		( 0 )
{
	m_pGroundStage	= std::make_shared<CGroundStage>();
	m_pSpawnUFO		= std::make_shared<CSpawnUFO>();
	m_pPlayer		= std::make_shared<CEventPlayer>();
	m_pGirl			= std::make_shared<CEventGirl>();
	m_pAlienA		= std::make_shared<CEventAlienA>();
	m_pEventCamera	= std::make_shared<CEventCamera>();
}

CGameClearEvent::~CGameClearEvent()
{
}

// �ǂݍ��݊֐�.
bool CGameClearEvent::Load()
{
	if (m_pGroundStage->Init() == false)	return false;	// �X�e�[�W�̏�����.
	if( SpawnUFOInit() == false )			return false;	// UFO�̏�����.
	if( PlayerInit() == false )				return false;	// �v���C���[�̏�����.
	if( GirlInit() == false )				return false;	// ���̎q�̏�����.
	if( AlienInit() == false )				return false;	// �F���l�̏�����.
	if (CameraInit() == false)				return false;	// �J�����̏�����.

	m_IsEventEnd = false;
	m_IsSkip = false;
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;

	return true;
}

// �X�V�֐�.
void CGameClearEvent::Update()
{
#if 1
	DebugOperation();
#endif

	// �V�[���̐ݒ�.
	SceneSetting();
	// �A�N�^�̍X�V.
	ActorUpdate();
	// �J�����̍X�V.
	CameraUpdate();

	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_IsEventEnd = true;
	}
}

// �`��֐�.
void CGameClearEvent::Render()
{
	m_pGroundStage->Render();	// �X�e�[�W�̕`��.
	m_pSpawnUFO->Render();		// UFO�̕`��.
	m_pPlayer->Render();		// �v���C���[�̕`��.
	m_pGirl->Render();			// ���̎q�̕`��.
	m_pAlienA->Render();		// �F���lA�̕`��.

#if 1
	DebugRender();
#endif

}

// �X�v���C�g�`��֐�.
void CGameClearEvent::SpriteRender()
{
}

// �J�����������֐�.
bool CGameClearEvent::CameraInit()
{
	m_stCamera.vPosition = { -27.0f, 7.5f, -65.5f };
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stCamera.vLookPosition.y = m_stPlayer.vPosition.y + 3.0f;
	return true;
}

// UFO�������֐�.
bool CGameClearEvent::SpawnUFOInit()
{
	if( m_pSpawnUFO->Init() == false ) return false;
	m_vUFOPosition = { 0.0f,10.0f, -100.0f };
	return true;
}

// �v���C���[�������֐�.
bool CGameClearEvent::PlayerInit()
{
	if( m_pPlayer->Init() == false ) return false;
	m_stPlayer.vPosition = m_pPlayer->GetPosition();
	m_stPlayer.vPosition.z = -30.0f;
	return true;
}

// ���̎q�������֐�
bool CGameClearEvent::GirlInit()
{
	if( m_pGirl->Init() == false )return false;
	m_stGirl.vPosition = m_pGirl->GetPosition();
	m_stGirl.vPosition.z = m_stPlayer.vPosition.z + 2.5f;
	return true;
}

// �F���l�������֐�.
bool CGameClearEvent::AlienInit()
{
	if( m_pAlienA->Init() == false ) return false;
	m_stAlien.vPosition = m_vAlienOldPosition = m_vUFOPosition;
	m_stAlien.vPosition.y = m_vUFOPosition.y + 9.0f;
	m_stAlien.vRotation.y = static_cast<float>(D3DXToRadian(145));
	m_stAlien.IsDisp = false;
	return true;
}

// �A�N�^�̍X�V�֐�.
void CGameClearEvent::ActorUpdate()
{
	// �v���C���[.
	m_pPlayer->SetOptionalState( m_stPlayer );
	// ���̎q.
	m_pGirl->SetOptionalState( m_stGirl );
	// �F���l.
	m_pAlienA->SetOptionalState( m_stAlien );
	// UFO�̈ʒu�ݒ�.
	m_pSpawnUFO->SetPosition( m_vUFOPosition );
}

// �J�����̍X�V�֐�.
void CGameClearEvent::CameraUpdate()
{
	m_pEventCamera->SetState(m_stCamera);
	CCameraManager::SetCamera(m_pEventCamera);
}

// �V�[���̐ݒ�.
void CGameClearEvent::SceneSetting()
{
	switch (m_NowScene)
	{
	case 0:	// UFO�Ɍ������đ���v���C���[�Ə��̎q.
		m_stCamera.vLookPosition = m_stPlayer.vPosition;
		m_stCamera.ViewingAngle = 0.5f;
		 m_stPlayer.vPosition.z -= 0.2f;
		 m_stGirl.vPosition.z -= 0.2f;

		if (m_stPlayer.vPosition.z <= m_vUFOPosition.z)
		{
			m_NowScene++;
		}
		break;

	case 1:	// UFO�ɋz�����܂��.
		if (m_stCamera.vLookPosition.y <= 18.0f)
		{
			m_stCamera.vLookPosition.y += 0.15f;;
		}

		if (m_stPlayer.vScale.x >= 0.0f)
		{
			m_stPlayer.ScaleSpeed = 0.01f;
			m_stPlayer.vScale.x -= m_stPlayer.ScaleSpeed;
			m_stPlayer.vScale.y -= m_stPlayer.ScaleSpeed;
			m_stPlayer.vScale.z -= m_stPlayer.ScaleSpeed;
		}

		if (m_stGirl.vScale.x >= 0.0f)
		{
			m_stGirl.ScaleSpeed = 0.01f;
			m_stGirl.vScale.x -= m_stGirl.ScaleSpeed;
			m_stGirl.vScale.y -= m_stGirl.ScaleSpeed;
			m_stGirl.vScale.z -= m_stGirl.ScaleSpeed;
		}

		// �����蔻��.
		m_stPlayer.vPosition = m_pSpawnUFO->Collision(m_pPlayer.get());
		m_stGirl.vPosition = m_pSpawnUFO->Collision(m_pGirl.get());

		if (m_stPlayer.vScale.x <= 0.0f) m_NowScene++;
		break;

	case 2:	//UFO�̃A�b�v.
		if (m_stCamera.ViewingAngle >= 0.34f) m_stCamera.ViewingAngle -= 0.01f;
		else {
			m_NowScene++;
		}
		break;

	case 3:
		m_WaitCount++;

		if (m_WaitCount >= 50) m_NowScene++;
		break;

	case 4:	// UFO�̂ǂ�����΂�����呛��.
	case 5:	// �F���l�ǂ��o�����..
		switch (m_UFOScale)
		{
		case 0:
			m_vUFOScale.x += 0.1f;
			if (m_vUFOScale.y >= 0.5f)m_vUFOScale.y -= 0.05f;

			if (m_vUFOScale.x >= 3.0f) m_UFOScale++;
			break;
		case 1:
			if(m_vUFOScale.x >= 1.0f)m_vUFOScale.x -= 0.1f;
			if(m_vUFOScale.y < 2.0f)m_vUFOScale.y += 0.1f;
			else {
				m_UFOScale++;
			}
			break;
		case 2:
			 m_vUFOScale.x += 0.01f;
			 m_vUFOScale.y -= 0.05f;
			 if (m_vUFOScale.y < 0.5f)	m_UFOScale++;
			break;
		case 3:
			if (m_vUFOScale.x > 1.0f) m_vUFOScale.x -= 0.05f;
			 m_vUFOScale.y += 0.1f;
			 if (m_vUFOScale.y > 2.5f)
			 {
				 m_UFOScale++;
				 m_NowScene++;

				 m_stAlien.IsDisp = true;
			 }
			break;
		case 4:
			m_vUFOScale.y -= 0.08f;
			if (m_vUFOScale.y <= 1.0f) m_UFOScale++;
			break;

		case 5:
			m_vUFOScale = { 1.0f,1.0f,1.0f };
			break;
		default:
			break;
		}

		m_pSpawnUFO->SetScale(m_vUFOScale);

		if (m_NowScene != 5) return;
		if(m_stCamera.ViewingAngle <= m_pEventCamera->ResetViewingAngle()) 
		{
			m_stCamera.ViewingAngle += 0.01f;
		}
		else
		{
			m_NowScene++;
		}
		if (m_stAlien.vPosition.y > 6.0f) {
			m_stAlien.vPosition.x += sinf(m_stAlien.vRotation.y + static_cast<float>(D3DX_PI)) * 0.2f;
			m_stAlien.vPosition.z += cosf(m_stAlien.vRotation.y + static_cast<float>(D3DX_PI)) * 0.2f;
			m_stAlien.vPosition.y += 0.4f + m_Gravity;
			m_Gravity -= 0.005f;
		}
		break;

	case 6:	// �F���l����.
		if (m_stAlien.vPosition.y < 9.0f) {
			if (m_stCamera.vLookPosition.y >= m_vUFOPosition.y)
			{
				m_stCamera.vLookPosition.y -= 1.0f;
			}
			else
			{
				// �J�����h�炷.
				m_stCamera.vLookPosition.y -= 1.0f;
			}
		}


		if (m_stAlien.vPosition.y > 6.0f) {
			m_stAlien.vPosition.x += sinf(m_stAlien.vRotation.y + static_cast<float>(D3DX_PI)) * 0.2f;
			m_stAlien.vPosition.z += cosf(m_stAlien.vRotation.y + static_cast<float>(D3DX_PI)) * 0.2f;
			m_stAlien.vPosition.y += 0.4f + m_Gravity;
			m_Gravity -= 0.005f;
		}
		else
		{
			m_NowScene++;
		}

		if (m_stAlien.vPosition.y < m_vAlienOldPosition.y) {
			if (m_stAlien.vRotation.x <= static_cast<float>(D3DXToRadian(180))) m_stAlien.vRotation.x += static_cast<float>(D3DXToRadian(20));
		}
		m_vAlienOldPosition = m_stAlien.vPosition;

		break;
	default:
		break;
	}

}

// ���̃V�[���ɐi�߂�.
void CGameClearEvent::NextStep()
{
}

// �X�L�b�v.
void CGameClearEvent::Skip()
{
}

void CGameClearEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameClearEvent -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 2.0f ,0.0f });
	CDebugText::Render("- CameraPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 3.0f ,0.0f });
	CDebugText::Render("Pos_x", m_stCamera.vPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 4.0f ,0.0f });
	CDebugText::Render("Pos_y", m_stCamera.vPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 5.0f ,0.0f });
	CDebugText::Render("Pos_z", m_stCamera.vPosition.z);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 7.0f ,0.0f });
	CDebugText::Render("- LookPosition -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 8.0f ,0.0f });
	CDebugText::Render("Pos_x", m_stCamera.vLookPosition.x);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 9.0f ,0.0f });
	CDebugText::Render("Pos_y", m_stCamera.vLookPosition.y);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 10.0f ,0.0f });
	CDebugText::Render("Pos_z", m_stCamera.vLookPosition.z);
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 12.0f ,0.0f });
	CDebugText::Render("- ViewingAngle -");
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 13.0f ,0.0f });
	CDebugText::Render("ViewinfAngle :", m_pEventCamera->GetViewingAngle());
}

void CGameClearEvent::DebugOperation()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_stCamera.vPosition.z += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_stCamera.vPosition.z -= m_Speed;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_stCamera.vPosition.y += m_Speed;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_stCamera.vPosition.y -= m_Speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_stCamera.vPosition.x += m_Speed;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_stCamera.vPosition.x -= m_Speed;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_stCamera.vLookPosition.y += m_Speed;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_stCamera.vLookPosition.y -= m_Speed;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_stCamera.vLookPosition.z -= m_Speed;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_stCamera.vLookPosition.z += m_Speed;
	}
}
