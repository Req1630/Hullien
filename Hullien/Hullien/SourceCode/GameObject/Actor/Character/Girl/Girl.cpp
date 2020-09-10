#include "Girl.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"

CGirl::CGirl()
	: m_Parameter			()
	, m_pSearchCollManager	( nullptr )
	, m_pWarning					( nullptr )
	, m_OldPosition				( 0.0f, 0.0f, 0.0f )
	, m_NowState					( ENowState::None )
	, m_NowMoveState			( EMoveState::None )
	, m_IsDanger					( false )
	, m_IsOnlyFirst					( false )
{
	m_ObjectTag	= EObjectTag::Girl;
	m_NowState	= ENowState::Protected;
	m_NowMoveState = EMoveState::Wait;
	m_pSearchCollManager = std::make_shared<CCollisionManager>();
	m_pWarning = std::make_unique<CWarning>();
}

CGirl::~CGirl()
{
}

// �������֐�.
bool CGirl::Init()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#else
	if( GetModel( MODEL_TEMP_NAME ) == false ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	if ( m_pWarning->Init() == false ) return false;

	return true;
}

// �X�V�֐�.
void CGirl::Update()
{
	switch( m_NowState )
	{
	case ENowState::None:
		break;
	case ENowState::Protected:
		break;
	case ENowState::Abduct:
		if( m_OldPosition == m_vPosition ){
			m_NowState = ENowState::Move;
			m_NowMoveState = EMoveState::Rotation;
		}
		m_OldPosition = m_vPosition;
		break;
	case ENowState::Move:
		Move();
		break;
	default:
		break;
	}
	if (m_NowState == ENowState::Abduct || m_IsDanger == true)
	{
		m_pWarning->Update();
		m_pWarning->SetPosition(m_vPosition);
	}

	// �T�E���h.
	Sound();

	//=============Warning=====================//.
	WarningRotation();
}

// �`��֐�.
void CGirl::Render()
{
	float rotY = m_vRotation.y;
	m_vRotation.y += static_cast<float>(D3DX_PI);
	MeshRender();	// ���b�V���̕`��.
	m_vRotation.y = rotY;

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_pSearchCollManager == nullptr ) return;
	m_pSearchCollManager->DebugRender();

	DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CGirl::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	SearchCollision( pActor );
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;
}

// ������W�̐ݒ�֐�.
void CGirl::SetTargetPos( CActor& actor )
{
	m_vPosition = actor.GetPosition();
	m_NowState = ENowState::Abduct;
}

// �X�v���C�g�̕`��.
void CGirl::SpriteRender()
{
	// ���̎q���A�ꋎ���Ă����Ԃ܂��͊댯�ȏ�ԂȂ�Όx����`��.
	if (m_NowState == ENowState::Abduct || m_IsDanger == true)
	{
		m_pWarning->SetPosition(m_vPosition);
		m_pWarning->Update();
		m_pWarning->Render();
	}
	// ���̎q���A�ꋎ���Ă����ԏo�Ȃ��Ȃ�x�����\��.
	if(m_NowState != ENowState::Abduct)
	{
		m_IsDanger = false;
	}
}

// �ړ��֐�.
void CGirl::Move()
{
	switch( m_NowMoveState )
	{
	case EMoveState::None:
		break;
	case EMoveState::Rotation:
		TargetRotation();
		break;
	case EMoveState::Move:
		TargetMove();
		break;
	case EMoveState::Wait:
		break;
	default:
		break;
	}
}

// �ړI�̏ꏊ�Ɍ����ĉ�].
void CGirl::TargetRotation()
{
	if( m_NowMoveState != EMoveState::Rotation ) return;

	const D3DXVECTOR3 targetPosition = { 0.0f, 0.0f, 0.0f };
	// �ړI�̉�]�����擾.
	D3DXVECTOR3 targetRotation = { 0.0f, 0.0f, 0.0f };
	targetRotation.y = atan2f( 
		targetPosition.x - m_vPosition.x,
		targetPosition.z - m_vPosition.z );
	m_MoveVector.x = sinf( targetRotation.y );
	m_MoveVector.z = cosf( targetRotation.y );

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// �ړI�̃x�N�g���ƁA�����̃x�N�g���̊O�ς����߂�.
	float cross = myVector.x * m_MoveVector.z-myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x+myVector.z * m_MoveVector.z;
	dot = acosf( dot / ( myLenght * targetLenght ));

	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.

																			// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// ���ς����e�͈͂Ȃ�.
	if( -TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN ){
		m_vRotation.y = targetRotation.y;	// �^�[�Q�b�g�ւ̉�]�擾.
											// �ړ��p�x�N�g�����擾.
		m_MoveVector.x = sinf( m_vRotation.y );
		m_MoveVector.z = cosf( m_vRotation.y );
		m_NowMoveState = EMoveState::Move;
	}
}

// �ړI�̏ꏊ�Ɍ����Ĉړ�.
void CGirl::TargetMove()
{
	if( m_NowMoveState != EMoveState::Move ) return;

	// �ړI�̏ꏊ.
	const D3DXVECTOR3 targetPosition = { 0.0f, 5.0f, 0.0f };
	const float moveSpeed = 0.05f;
	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

	float lenght = D3DXVec3Length( &D3DXVECTOR3(targetPosition - m_vPosition) );

	if( lenght >= 1.0f ) return;

	m_NowMoveState = EMoveState::Wait;
}

// ���G�̓����蔻��.
void CGirl::SearchCollision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pSearchCollManager == nullptr ) return;
	if( m_pSearchCollManager->GetSphere() == nullptr ) return;

	// ���ɘA�ꋎ���Ă�����I��.
	if( m_NowState == ENowState::Abduct ) return;

	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// ���̂̓����蔻��.
	if( m_pSearchCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	m_IsDanger = true;

}

// �����蔻��̍쐬.
bool  CGirl::ColliderSetting()
{
#ifndef IS_TEMP_MODEL_RENDER
	if( m_pSkinMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	return true;
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	// ���̎q�̓����蔻��.
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	// ���G�̓����蔻��.
	if( FAILED( m_pSearchCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SearchCollRadius ) )) return false;

	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

// �T�E���h.
void CGirl::Sound()
{
	// ���̎q���댯�ȏ�ԂȂ��SE��炷.
	if (m_IsDanger == true && m_IsOnlyFirst == false)
	{
		CSoundManager::NoMultipleSEPlay("Warning");
		m_IsOnlyFirst = true;
	}
	// �댯�ȏ�Ԃ�E������SE���ēx�点���Ԃɂ���.
	if (m_IsDanger == false)
	{
		m_IsOnlyFirst = false;
	}
}

// �댯���̉�].
void CGirl::WarningRotation()
{
	const D3DXVECTOR3 targetPosition = { 0.0f, 0.0f, 0.0f };
	// �J�������珗�̎q�ւ̉�]�����擾.
	D3DXVECTOR3 targetRotation = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 MoveVector = { 0.0f, 0.0f, 0.0f };
	targetRotation.y = atan2f(
		m_vPosition.x - CCameraManager::GetPosition().x,
		m_vPosition.z - CCameraManager::GetPosition().z);
	MoveVector.x = sinf(targetRotation.y);
	MoveVector.z = cosf(targetRotation.y);

	// �J�����̑O�x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf(m_CameraRadianX);
	myVector.z = cosf(m_CameraRadianX);

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(MoveVector.x*MoveVector.x + MoveVector.z*MoveVector.z);

	// �J�������珗�̎q�ւ̃x�N�g���ƁA�J�����̑O�x�N�g���̊O�ς����߂�.
	float cross = myVector.x * MoveVector.z - myVector.z * MoveVector.x;
	float dot = myVector.x * MoveVector.x + myVector.z * MoveVector.z;
	dot = acosf(dot / (myLenght * targetLenght));

	if (dot > 2.4f)
	{
		// �J������.
		m_pWarning->SetIsGirlOffScreen(false);
	}
	else
	{
		// �J�����O.
		m_pWarning->SetIsGirlOffScreen(true);
	}

	const float pos_y = 40.0f;
	const float pos_x = 880.0f;

	if (m_IsWarning == true)
	{
		CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 23, 0.0f });
		if (cross > 0.0f) {
			CDebugText::Render("right");
		}
		else
		{
			CDebugText::Render("left");
		}
	}

	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 21, 0.0f });
	CDebugText::Render("cross : ", cross);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 22, 0.0f });
	CDebugText::Render("dot : ", dot);
}

// �f�o�b�O�`��֐�.
void CGirl::DebugRender()
{
	const float pos_y = 40.0f;
	const float pos_x = 880.0f;
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale()*10, 0.0f });
	CDebugText::Render("- warning -");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 11, 0.0f });
	CDebugText::Render("-- CameraRadianX --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 12, 0.0f });
	CDebugText::Render("CameraRadianX : ", m_CameraRadianX);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 14, 0.0f });
	CDebugText::Render("-- CameraPosition --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 15, 0.0f });
	CDebugText::Render("Pos_X : ", CCameraManager::GetPosition().x);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 16, 0.0f });
	CDebugText::Render("Pos_Y : ", CCameraManager::GetPosition().y);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 17, 0.0f });
	CDebugText::Render("Pos_Z : ", CCameraManager::GetPosition().z);
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 19, 0.0f });
	CDebugText::Render("-- WarningPosition --");
	CDebugText::SetPosition({ pos_x, pos_y + CDebugText::GetScale() * 20, 0.0f });
	if (m_IsWarning == true)
	{
		CDebugText::Render("GirlPosition");
	}
	else
	{
		CDebugText::Render("!GirlPosition");
	}

}
