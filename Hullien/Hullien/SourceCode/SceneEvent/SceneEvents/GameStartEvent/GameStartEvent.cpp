#include "GameStartEvent.h"
#include "..\..\..\Utility\XInput\XInput.h"
#include ".\..\..\..\GameObject\GroundStage\GroundStage.h"
#include "..\..\..\GameObject\SpawnUFO\SpawnUFO.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventPlayer\EventPlayer.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventGirl\EventGirl.h"
#include "..\..\..\GameObject\Actor\EventCharacter\EventAlien\EventAlien_A\EventAlien_A.h"
#include "..\..\..\GameObject\Actor\Barrier\Barrier.h"
#include "..\..\..\GameObject\MotherShipUFO\MotherShipUFO.h"
#include "..\..\..\GameObject\SkyDome\SkyDome.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\EventManager\EventManager.h"

#include "..\..\..\Common\DebugText\DebugText.h"

/***********************************
*	�X�^�[�g�C�x���g�N���X.
**/
CGameStartEvent::CGameStartEvent()
	: m_pGroundStage		( nullptr )
	, m_pSpawnUFO			( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGirl				( nullptr )
	, m_pAlienA				( nullptr )
	, m_pBarrier			( nullptr )
	, m_pMotherShipUFO		( nullptr )
	, m_pEventCamera		( nullptr )
	, m_pEventManager		( nullptr )
	, m_vUFOPosition		( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ))
	, m_EventStep			( EEventStep::EventStart )
	, m_NowStep				( 0 )
	, m_Speed				( 0.0f )
	, m_DecelerationZ		( 0.0f )
	, m_Count				( 0.0f )
	, m_IsDisp				( false )
	, m_stPlayer			()
	, m_stGirl				()
	, m_stAlien				()
	, m_stCamera			()
{
	m_pGroundStage		= std::make_shared<CGroundStage>();
	m_pSpawnUFO			= std::make_shared<CSpawnUFO>();
	m_pPlayer			= std::make_shared<CEventPlayer>();
	m_pGirl				= std::make_shared<CEventGirl>();
	m_pAlienA			= std::make_shared<CEventAlienA>();
	m_pBarrier			= std::make_shared<CBarrier>();
	m_pMotherShipUFO	= std::make_shared<CMotherShipUFO>();
	m_pEventCamera		= std::make_shared<CEventCamera>();
}

CGameStartEvent::~CGameStartEvent()
{
}

// �Ǎ��֐�.
bool CGameStartEvent::Load()
{
	if( m_pGroundStage->Init() == false )	return false;	// �n�ʂ̏�����.
	if( SpawnUFOInit() == false )			return false;	// UFO�̏�����.
	if( PlayerInit() == false )				return false;	// �v���C���[�̏�����.
	if( GirlInit() == false )				return false;	// ���̎q�̏�����.
	if( AlienInit() == false )				return false;	// �F���lA�̏�����.
	if( MotherShipUFOInit() == false )		return false;	// �}�U�[�V�b�v�̏�����.
	if( CameraInit() == false )				return false;	// �J�����̏�����.

	m_IsEventEnd = false;
	m_Speed = static_cast<float>(D3DX_PI) * 0.05f;

	return true;
}

// �X�V�֐�.
void CGameStartEvent::Update()
{
	DebugOperation();

	// �V�[���̐ݒ�.
	SceneSetting();
	// �A�N�^�̍X�V.
	ActorUpdate();
	// �J�����̍X�V.
	CameraUpdate();

	if (GetAsyncKeyState(VK_RETURN) & 0x0001
		|| CXInput::B_Button() == CXInput::enPRESS_AND_HOLD)
	{
		m_EventStep = EEventStep::GameStart;
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
	m_pBarrier->Render();
	m_pMotherShipUFO->Render();

	DebugRender();
}

// �J�����������֐�.
bool CGameStartEvent::CameraInit()
{
	m_stCamera.vPosition = CAMERA_INITPOSITION;
	m_stCamera.vLookPosition = m_pPlayer->GetPosition();
	m_stCamera.vLookPosition.y = m_pPlayer->GetPosition().y + CAMERA_CORRECTION_PLAYERPOS_Y;
	return true;
}

// UFO�������֐�.
bool CGameStartEvent::SpawnUFOInit()
{
	if (m_pSpawnUFO->Init() == false) return false;	
	m_vUFOPosition = UFO_INITPOSITION;

	return true;
}

// �v���C���[�������֐�.
bool CGameStartEvent::PlayerInit()
{
	if (m_pPlayer->Init() == false) return false;	
	m_stPlayer.vPosition.z = PLAYER_INITPOSITION_Z;
	return true;
}

// ���̎q�������֐�.
bool CGameStartEvent::GirlInit()
{
	if (m_pGirl->Init() == false) return false;
	m_stGirl.vPosition.z = GIRL_INITPOSITION_Z;
	return true;
}

// �F���l�������֐�.
bool CGameStartEvent::AlienInit()
{
	if (m_pAlienA->Init() == false) return false;
	m_stAlien.vRotation.y = ALIEN_INITROTATION_Y;
	m_stAlien.IsDisp = false;
	return true;
}

// �}�U�[�V�b�v�������֐�.
bool CGameStartEvent::MotherShipUFOInit()
{
	if (m_pMotherShipUFO->Init() == false) return false;
	m_pMotherShipUFO->SetPosition(MOTHERSHIP_INITPOSITION);
	m_pMotherShipUFO->SetDisp(false);
	return true;
}

// �A�N�^�̍X�V�֐�.
void CGameStartEvent::ActorUpdate()
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
void CGameStartEvent::CameraUpdate()
{
	m_pEventCamera->SetState( m_stCamera );
	CCameraManager::SetCamera( m_pEventCamera );
}

// �V�[���̐ݒ�.
void CGameStartEvent::SceneSetting()
{
	switch (m_EventStep)
	{
	case EEventStep::Escape_PlayerAndGirl:	//������v���C���[�Ə��̎q.
		EscapePlayerAndGirl();
		break;
	case EEventStep::Viewpoint_UFO:	//2�l��ǂ�UFO�̎��_.
		ViewpointUFO();
		break;
	case EEventStep::Move_UFO:	//UFO��ʒu�ɂ�.
		MoveUFO();
		break;
	case EEventStep::Stop_UFO: // UFO��~.
		StopUFO();
		break;
	case EEventStep::Appearance_Alien: //�F���l�o��.
		AppearanceAlien();
		break;
	case EEventStep::Move_Alien: // �F���l�_�b�V��.
		MoveAlien();
		break;
	case EEventStep::GetCaught_Girl: // ���̎q�߂܂�.
		GetCaughtGirl();
		break;
	case EEventStep::InvocatingOrder_Barrier:	//�o���A�����w��.
		InvocatingOrderBarrier();
		break;
	case EEventStep::Player_Up:	//�o���A����.
		PlayerUp();
		break;
	case EEventStep::Invocating_Barrier:	//�o���A����.
		InvocatingBarrier();
		break;
	case EEventStep::Return_Girl:	// ���̎q�A��.
		ReturnGirl();
		break;
	case EEventStep::GameStart:	//�Q�[���V�[����.
		GameStart();
		break;
	default:
		break;
	}
}

// ���̃V�[���ɐi�߂�.
void CGameStartEvent::NextStep()
{
	m_NowStep++;
	m_EventStep = static_cast<EEventStep>(m_NowStep);
}

// ������v���C���[�Ə��̎q.
void CGameStartEvent::EscapePlayerAndGirl()
{
	m_stCamera.vPosition.z = m_stPlayer.vPosition.z;
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

	// ���̃X�e�b�v��.
	if (m_stPlayer.vPosition.z <= CAMERASWITCHING_POS_Z) NextStep();
}

// UFO���_.
void CGameStartEvent::ViewpointUFO()
{
	// �J�����̐ݒ�.
	m_stCamera.vPosition = m_vUFOPosition;
	m_stCamera.vPosition.y = m_vUFOPosition.y + CAMERA_CORRECTION_UFOPOS_Y;
	m_stCamera.vPosition.z = m_vUFOPosition.z - CAMERA_CORRECTION_UFOPOS_Z;
	m_stCamera.vLookPosition.z = m_pPlayer->GetPosition().z - CAMERA_CORRECTION_PLAYERLOOK_Z;
	// �v���C���[�Ə��̎q�̈ړ�.
	if (m_stPlayer.vPosition.z > 0.0f)
	{
		m_stPlayer.vPosition.z -= m_stPlayer.MoveSpeed;
		m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;
	}

	// UFO�ړ�.
	m_vUFOPosition.z -= UFO_MOVE_SPEED;

	// ���̃X�e�b�v��.
	if (m_pSpawnUFO->GetPosition().z <= CAMERASWITCHING_POS_Z) NextStep();
}

// UFO�ړ�.
void CGameStartEvent::MoveUFO()
{
	// UFO�ړ�.
	if (m_pSpawnUFO->GetPosition().z < UFO_MOVEING_LIMIT_Z)
	{
		m_vUFOPosition.z -= (UFO_MOVE_SPEED + m_DecelerationZ);
		m_DecelerationZ -= UFO_MOVE_DECELERATION_Z;
	}
	else
	{
		m_vUFOPosition.z -= UFO_MOVE_SPEED;
	}

	// �J�����ʒu.
	m_stCamera.vPosition = CAMERA_POSITION_MOVEUFO;
	// �J�����̎��_�ړ�.
	m_stCamera.vLookPosition = m_pSpawnUFO->GetPosition();
	// �v���C���[�̈ʒu�ݒ�.
	m_stPlayer.vPosition.z = 0.0f;
	// ���̎q�̈ʒu�ݒ�.
	m_stGirl.vPosition.z = GIRL_CONSTANT_POSITION_Z;

	// ���̃X�e�b�v.
	if (m_pSpawnUFO->GetPosition().z < UFO_MOVEING_LIMIT_Z)
	{
		m_DecelerationZ = 0.0f;
		NextStep();
	}
}

// UFO��~.
void CGameStartEvent::StopUFO()
{
	m_Count++;
	if (m_Count < UFO_STOP_COUNT)
	{
		m_vUFOPosition.y = m_vUFOPosition.y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_UFO_STOP * m_Count) * AMPLITUDE_UFO_STOP);
	}
	else
	{
		if (m_vUFOPosition.y >= UFO_MOVEING_LIMIT_Y) m_vUFOPosition.y -= UFO_STOP_SPEED;
	}

	if (m_vUFOPosition.y < UFO_MOVEING_LIMIT_Y && m_Count >= WAIT_COUNT)
	{
		NextStep();
		m_stAlien.vPosition = m_pSpawnUFO->GetPosition();
		m_Count = 0;
	}
}

// �F���l�o��.
void CGameStartEvent::AppearanceAlien()
{
	m_stCamera.vPosition = CAMERA_POSITION_APP_ALIEN;
	m_stCamera.vLookPosition.y = m_pAlienA->GetPosition().y + CAMERA_CORRECTIONALIENPOS_Y;

	m_stAlien.IsDisp = true;

	if (m_stAlien.vPosition.y >= ALIEN_MOVEING_LIMIT_Y)
	{
		m_stAlien.vPosition.y -= ALIEN_FALL_SPEED;
	}
	else
	{
		NextStep();
	}
}

// �F���l�ړ�.
void CGameStartEvent::MoveAlien()
{
	m_Count++;
	m_stAlien.vPosition.y = m_pAlienA->GetPosition().y + static_cast<float>(sin(D3DX_PI * TWO / FREQUENCY_ALIEN_UPDOWN * m_Count) * AMPLITUDE_ALIEN_UPDOWN);
	if (m_Count > WAIT_COUNT)
	{
		m_stAlien.vPosition.z += ALIEN_RUN_SPEED;
	}

	if (m_stAlien.vPosition.z >= 0.0f)
	{
		m_Count = 0;
		NextStep();
	}
}

// ���̎q�߂܂�.
void CGameStartEvent::GetCaughtGirl()
{
	m_stCamera.vPosition = CAMERA_POSITION_CAUGHT_GIRL;
	m_stCamera.vLookPosition = m_pPlayer->GetPosition();

	m_stAlien.vPosition.z += ALIEN_MOVE_SPEED;

	m_pSpawnUFO->SetDisp(false);

	if (m_stAlien.vPosition.z > ALIEN_MOVEING_LIMIT_Z)
	{
		NextStep();
	}

	if (m_pGirl->GetIsDanger() == true)
	{
		m_stPlayer.vRotation.y = m_pPlayer->RotationMoveRight(PLAYER_ROTATION_Y, m_stPlayer.RotationalSpeed);
	}
	m_pGirl->Collision(m_pAlienA.get());
	m_pAlienA->Collision( m_pGirl.get() );
	m_stGirl.vPosition = m_pGirl->GetPosition();
}

// �o���A��������.
void CGameStartEvent::InvocatingOrderBarrier()
{
	m_stCamera.vPosition = CAMERA_POSITION_ORDER_BARRIER;
	m_stCamera.vLookPosition = m_pGirl->GetPosition();
	// �}�U�[�V�b�v�̕`��.
	m_pMotherShipUFO->SetDisp(true);
	// �v���C���[�̍X�V.
	m_pPlayer->Update();

	if (m_pPlayer->IsSpecialAbility() == true)
	{
		m_pBarrier->Init();	// �o���A�̏�����.
		NextStep();

		// �J�����̐ݒ�.
		m_stCamera.vLookPosition = m_pPlayer->GetPosition();
		m_stCamera.vLookPosition.y = m_pPlayer->GetPosition().y + CAMERA_CORRECTION_PLAYERPOS_Y;
		m_stCamera.vPosition = CAMERA_POSITION_PLAYER_UP;
		m_stCamera.ViewingAngle = VIEWING_ANGLE_PLAYER_UP;
	}
}

// �v���C���[�̃A�b�v.
void CGameStartEvent::PlayerUp()
{
	// ����p���ړ����E�l�����傫���ꍇ.
	if (m_stCamera.ViewingAngle > VIEWING_ANGLE_MOVING_LIMIT)
	{
		if (m_stCamera.ViewingAngle > VIEWING_ANGLE_DEC_START_POS)
		{
			m_stCamera.ViewingAngle -= VIEWING_ANGLE_MOVE_SPEED;
		}
		else
		{
			m_stCamera.ViewingAngle -= VIEWING_ANGLE_MOVE_SPEED + m_DecelerationZ;
			m_DecelerationZ -= VIEWING_ANGLE_DECELERATION_SPEED;
		}
	}
	else
	{
		m_DecelerationZ = 0.0f;
		m_stCamera.ViewingAngle = VIEWING_ANGLE_MOVING_LIMIT;
		// �J�������v���C���[�̌�둤�Ɉړ�.
		if (m_stCamera.vRotation.x > CAMERA_ROTAION_MOVING_LIMIT_X)
		{
			m_stCamera.vRotation.x -= static_cast<float>(D3DX_PI) * m_stCamera.MoveSpeed;
			// �J�����̉����E�����̐ݒ�.
			if (m_stCamera.vRotation.x > CAMERA_ROTAION_DEC_START)
			{
				if (m_stCamera.MoveSpeed <= CAMERA_ROTATION_SPEED) m_stCamera.MoveSpeed += CAMERA_DECELERATION_SPEED;
			}
			else
			{
				if (m_stCamera.MoveSpeed >= CAMERA_ROTATION_SPEED) m_stCamera.MoveSpeed -= CAMERA_DECELERATION_SPEED;
			}
		}
		else
		{
			NextStep();
			// �J�����̗h��p�J�E���g.
			m_Count = AMPLITUDE_COUNT;
		}

		// �J�����ƃv���C���[�̋���.
		if (m_stCamera.vLenght.z < 16.0f) m_stCamera.vLenght.z += CAMERA_MOVE_SPEED;
		if (m_stCamera.vLookPosition.z < 9.5f) m_stCamera.vLookPosition.z += CAMERA_MOVE_SPEED;

		// �J�����ʒu���Z�o.
		m_stCamera.vPosition.x = m_stCamera.vLookPosition.x + (sinf(m_stCamera.vRotation.x) * m_stCamera.vLenght.x);
		m_stCamera.vPosition.z = m_stCamera.vLookPosition.z + (cosf(m_stCamera.vRotation.x) * m_stCamera.vLenght.z);
	}
}

// �o���A����.
void CGameStartEvent::InvocatingBarrier()
{
	m_stCamera.vLookPosition.x = m_stCamera.vLookPosition.x + static_cast<float>(sin(D3DX_PI * 2.0f / 10.0f * m_Count) * (m_Count * 0.01f));
	m_stCamera.vLookPosition.y = m_stCamera.vLookPosition.y + static_cast<float>(sin(D3DX_PI * 2.0f / 20.0f * m_Count) * (m_Count * 0.01f));
	if (m_Count != 0) {
		m_Count--;
	}

	m_pBarrier->SetTargetPos(*m_pGirl.get());
	m_pBarrier->Update();
	m_pAlienA->Collision(m_pBarrier.get());

	m_stAlien.vPosition = m_pAlienA->GetPosition();
	if (m_pAlienA->IsBarrierHit() == true) { m_stAlien.vPosition.x -= 1.0f; }
	m_pAlienA->SetPosition(m_stAlien.vPosition);

	if (m_pBarrier->IsActive() == false)
	{
		NextStep();
	}
}

// ���̎q�A��.
void CGameStartEvent::ReturnGirl()
{
	m_pSpawnUFO->SetDisp( true );
	m_vUFOPosition.y = 10.0f;
	m_vUFOPosition.z = -100.0f;

	m_stGirl.vPosition.z -= m_stGirl.MoveSpeed;

	if (m_stGirl.vPosition.z >= 20.0f) return;
	m_stCamera.vLookPosition = m_stPlayer.vPosition;
	m_stCamera.ViewingAngle = m_pEventCamera->ResetViewingAngle();
	m_stPlayer.vRotation.y = m_pPlayer->RotationMoveRight(static_cast<float>(D3DXToRadian(0)), m_stPlayer.RotationalSpeed);

	if (m_stCamera.vPosition.x < 0.0f) { m_stCamera.vPosition.x += 0.1f; }
	else { m_stCamera.vPosition.x = 0.0f; }

	if (m_stCamera.vPosition.y < 12.0f) { m_stCamera.vPosition.y += 0.2f; }
	else { m_stCamera.vPosition.y = 12.0f; }

	if (m_stCamera.vPosition.z < 30.0f) { m_stCamera.vPosition.z += 1.0f; }
	else { m_stCamera.vPosition.z = 30.0f; }

	if (m_stGirl.vPosition.z <= 0.0f)  NextStep();
}

// �Q�[���J�n.
void CGameStartEvent::GameStart()
{
	m_IsEventEnd = true;
}

void CGameStartEvent::DebugRender()
{
	CDebugText::SetPosition({ 0.0f, 80.0f + CDebugText::GetScale() * 0 ,0.0f });
	CDebugText::Render("- GameStartEvent -");
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

void CGameStartEvent::DebugOperation()
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

