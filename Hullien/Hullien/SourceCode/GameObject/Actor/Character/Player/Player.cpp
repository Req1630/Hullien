#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\ChatacterWidget.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\LifeGauge\LifeGauge.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\SpecialAbilityGauge\SpecialAbilityGauge.h"
#include "..\..\..\..\Common\Effect\EffectManager.h"
#include "..\..\Item\EffectTimer\EffectTimer.h"

#include "..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"
#include "..\...\..\..\..\..\XAudio2\SoundManager.h"

CPlayer::CPlayer()
	: m_pCamera						( nullptr )
	, m_pWidget						()
	, m_pAttackCollManager			( nullptr )
	, m_OldPosition					( 0.0f, 0.0f, 0.0f )
	, m_NowAnimNo					( CPlayer::enAnimNo::Walk )
	, m_OldAnimNo					( CPlayer::enAnimNo::None )
	, m_AttackComboCount			( EAttackNo_None )
	, m_AttackEnabledFrameList		()
	, m_AttackDataQueue				()
	, m_AttackPosition				( 0.0f, 0.0f, 0.0f )
	, m_pEffects					()
	, m_IsDuringAvoid				( false )
	, m_AvoidVector					( 0.0f, 0.0f, 0.0f )
	, m_Parameter					()
	, m_LifePoint					( 0.0f )
	, m_SpecialAbility				( 0.0f )
	, m_HasUsableSP					( false )
	, m_SpecialAbilityValue			( 0.0f )
	, m_ItemSpecialAbilityValue		( 0.0f )
	, m_AttackPower					( 0.0f )
	, m_MoveSpeed					( 0.0f )
	, m_CameraDefaultHeight			( 0.0f )
	, m_CameraHeight				( 0.0f )
	, m_IsAttackHitCamera			( false )
	, m_CameraShakeCount			( 0.0f )
	, m_CameraShakeTieme			( 10.0f )
	, m_CameraShakeCountAdd			( 1.0f )
	, m_pEffectTimers				( EEffectTimerNo::EEffectTimerNo_Max )
	, m_IsAttackSE					( false )
{
	m_ObjectTag = EObjectTag::Player;
	m_pCamera = std::make_shared<CRotLookAtCenter>();
	for( auto& e : m_pEffectTimers ) e = std::make_shared<CEffectTimer>();
}

CPlayer::~CPlayer()
{
}

// �������֐�.
bool CPlayer::Init()
{
	if( ParameterSetting( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
#ifndef IS_TEMP_MODEL_RENDER
	if( GetModel( MODEL_NAME ) == false ) return false;
#if 0
	// �A�j���[�V�����Đ�.
	SetAttackFrameList();
#endif	//#if 0
#else
	if( GetModel( MODEL_TEMP_NAME ) == false ) return false;
#endif	// #ifndef IS_TEMP_MODEL_RENDER.
	if( ColliderSetting() == false ) return false;
	if( WidgetSetting() == false ) return false;
	if( EffectSetting() == false ) return false;
	m_MoveSpeed		= m_Parameter.MoveSpeed;	// �ړ����x�̐ݒ�.
	m_AttackPower	= m_Parameter.AttackPower;	// �U���͂̐ݒ�.
	m_LifePoint		= m_Parameter.LifeMax;		// �̗͂̐ݒ�.
	m_SpecialAbilityValue = m_Parameter.SpecialAbilityValue;	// ����\�͉񕜒l�̐ݒ�.
	m_CameraHeight = m_CameraDefaultHeight = m_Parameter.CameraHeight;


	return true;
}

// �X�V�֐�.
void CPlayer::Update()
{
	// ��Ⴣ^�C�}�[�����삵�ĂȂ����.
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Paralysis]->IsUpdate() == false ){
		Controller();			// ����.
		AttackController();		// �U������.
		SPController();			// ����\�͑���.
		AvoidController();		// ��𑀍�.
		AttackAnimation();		// �U���A�j���[�V����.
		Move();					// �ړ�.
		AvoidMove();			// ��𓮍�.
	} else {
		ParalysisUpdate();	// ��჎��̍X�V.
	}
	AttackHitCameraUpdate();	// �U���q�b�g���̃J��������.
	SpecialAbilityUpdate();		// ����\�͉񕜍X�V.
	AttackUpUpdate();			// �U����UP�X�V.
	MoveSpeedUpUpdate();		// �ړ����xUP�X�V.

	CameraController();	// �J��������.
	// �v���C���[�𒍎����ĉ�].
	m_pCamera->RotationLookAtObject( { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z },
		m_Parameter.CameraLerpValue );
	m_pCamera->SetLength( m_Parameter.CameraDistance );	// ���S�Ƃ̋�����ݒ�.
	m_pCamera->SetHeight( m_CameraHeight );	// �����̐ݒ�.

	// �J�������}�l�[�W���[�ɐݒ�.
	CCameraManager::SetCamera( m_pCamera );

	// �̗͂�1/3�ɂȂ�����SE��炷.
	if (m_LifePoint <= m_Parameter.LifeMax / 3)
	{
		if (CSoundManager::GetIsPlaySE("HP", 0) == true) return;
		CSoundManager::NoMultipleSEPlay("HP");
	}
}

// �`��֐�.
void CPlayer::Render()
{
	MeshRender();	// ���b�V���̕`��.
	EffectRender();	// �G�t�F�N�g�̕`��.

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_pAttackCollManager == nullptr ) return;
	m_pAttackCollManager->DebugRender();
#endif	// #if _DEBUG.
}

// �����蔻��֐�.
void CPlayer::Collision( CActor* pActor )
{
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	AttackCollision( pActor );	// �U�����̓����蔻��.

}

// ������W�̐ݒ�֐�.
void CPlayer::SetTargetPos( CActor& actor )
{
	//	m_vPosition = actor.GetPosition();
}

// �X�v���C�g�̕`��.
void CPlayer::SpriteRender()
{
	// �Q�[�W.
	CCharacterWidget::SCharacterParam param;
	// ���C�t.
	param.Life = m_LifePoint;
	param.LifeMax = m_Parameter.LifeMax;
	// ����\��.
	param.SpecialAbility = m_SpecialAbility;
	param.SpecialAbilityMax = m_Parameter.SpecialAbilityMax;
	for (const auto& s : m_pWidget)
	{
		s->SetParameter(param);
		s->Update();
		s->Render();
	}

#if _DEBUG
	// �G�f�B�b�g�p�̕`��֐����G�f�B�b�g�����_���[�ɒǉ�.
	CEditRenderer::PushRenderProc( [&](){ EditRender(); } );
	// �f�o�b�O�`��.
	DebugRender();
#endif	// #if _DEBUG.
}

// ����\�͂��g���Ă��邩.
bool CPlayer::IsSpecialAbility()
{
	if( m_HasUsableSP == false ) return false;
	// ����\�͂��g����Ȃ�.
	m_HasUsableSP = false;	// ����������.
	return true;			// true��Ԃ�.
}

// �J�����̕���.
float CPlayer::GetCameraRadianX()
{
	return m_pCamera->GetRadianX();
}

// ����֐�.
void CPlayer::Controller()
{
	// �R���g���[���[��L�X�e�B�b�N�̌X�����擾.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	if( GetAsyncKeyState(VK_UP) & 0x8000 )		m_vPosition.z -= m_MoveSpeed;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 )	m_vPosition.z += m_MoveSpeed;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )	m_vPosition.x -= m_MoveSpeed;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )	m_vPosition.x += m_MoveSpeed;
}

// �J��������.
void CPlayer::CameraController()
{
	// �J�����̉�]�ړ�.
	// ������.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalMove(  m_Parameter.CameraMoveSpeed );	// �E����.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalMove( -m_Parameter.CameraMoveSpeed );	// ������.

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000) m_pCamera->DegreeHorizontalMove(m_Parameter.CameraMoveSpeed);
	if (GetAsyncKeyState(VK_SHIFT)  & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)	m_pCamera->DegreeHorizontalMove(-m_Parameter.CameraMoveSpeed);
}

// �U������֐�.
void CPlayer::AttackController()
{
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;
	// X�{�^�����������u�Ԃ���Ȃ���ΏI��.
	if( CXInput::X_Button() != CXInput::enPRESSED_MOMENT ) return;
	// �U���J�E���g���ő�ȏ�Ȃ�I��.
	if( m_AttackComboCount >= m_Parameter.AttackComboMax ) return;
	CSoundManager::NoMultipleSEPlay("PlayerAttackSE");
	m_AttackComboCount++;	// �U���J�E���g�����Z.
	// �U���f�[�^���L���[�ɒǉ����ꂽ��I��.
	if( IsPushAttack() == true ) return;
	m_AttackComboCount--;	// �U���J�E���g�����Z.

}

// ����\�͑���֐�.
void CPlayer::SPController()
{
	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	// Y�{�^���������ꂽ�u�Ԃ���Ȃ���ΏI��.
	if( CXInput::Y_Button() != CXInput::enPRESSED_MOMENT ) return;

	m_SpecialAbility = 0.0f;
	m_HasUsableSP = true;
}

// ��𑀍�֐�.
void CPlayer::AvoidController()
{
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// A�{�^���������ꂽ�u�Ԃ���Ȃ���ΏI��.
	if( CXInput::A_Button() != CXInput::enPRESSED_MOMENT ) return;
	m_IsDuringAvoid = true;
	m_AvoidVector = m_MoveVector;	// �ړ��x�N�g����ݒ�.
	m_OldPosition = m_vPosition;	// ���݂̍��W��ݒ�.
	CSoundManager::NoMultipleSEPlay("AvoidMove");
	m_pEffects[EEffectNo::enEffectNo_Avoidance]->Play( m_vPosition );
	// ����A�j���[�V������ݒ肷��Ȃ� ����.
	//

}

// �ړ��֐�.
void CPlayer::Move()
{
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;

	// �^�[�Q�b�g�̃x�N�g����p��.
	float targetRot = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
	D3DXVECTOR3 targetVec = { 0.0f, 0.0f, 0.0f };
	targetVec.x = sinf( targetRot );
	targetVec.z = cosf( targetRot );

	// ���g�̃x�N�g����p��.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// �x�N�g���̒��������߂�.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(targetVec.x*targetVec.x + targetVec.z*targetVec.z);

	// ���ς����߂�.
	float dot = myVector.x*targetVec.x + myVector.z*targetVec.z;
	dot = acosf( dot / ( myLenght * targetLenght ));
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(20.0));
	
	if( ( -TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN ) ||	// ���ς����e�͈͂Ȃ�.
		( std::isfinite( dot ) ) == false ){						// ���ς̒l���v�Z�ł��Ȃ��l�Ȃ�.
		// �^�[�Q�b�g�̉�]���擾.
		m_vRotation.y = targetRot;
		// ��]���ňړ�.
		m_vPosition.x -= targetVec.x * m_MoveSpeed;
		m_vPosition.z -= targetVec.z * m_MoveSpeed;
		m_OldPosition = m_vPosition;
	} else {
		const float ROTATIONAL_SPEED = 0.3f;	// ��]���x.
		// �ړI�̃x�N�g���ƁA�����̃x�N�g���̊O�ς����߂�.
		float cross = myVector.x*targetVec.z - myVector.z*targetVec.x;
		// �O�ς�0.0��菭�Ȃ���� ���v��� : �����v��� �ɉ�]����.
		m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;
	}
}

// ��𓮍�֐�.
void CPlayer::AvoidMove()
{
	// ��𒆂���Ȃ���ΏI��.
	if( m_IsDuringAvoid == false ) return;

	// �X�e�B�b�N�̌X���������Ɍ���.
	m_vRotation.y = atan2f( m_AvoidVector.x, m_AvoidVector.z );
	// �J�����̊p�x�Ƒ������킹��.
	m_vRotation.y += m_pCamera->GetRadianX();

	// ��]���ňړ�.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;

	// ��]�@�A�j���[�V�����ݒ�����.
	m_vRotation.z -= 0.1f;

	D3DXVECTOR3 length = m_OldPosition - m_vPosition;
	// �ړ��������v�Z.
	float moveDistance = D3DXVec3Length( &length );
	// �ړ����������ȉ��Ȃ�I��.
	if( moveDistance <= m_Parameter.AvoidMoveDistance ) return;
	m_IsDuringAvoid = false;	// ��𒆂���Ȃ�����.
	m_vRotation.z = 0.0f;		// ��]�@�A�j���[�V�����ݒ�����.
}

// �G�t�F�N�g�`��֐�.
void CPlayer::EffectRender()
{
	if( m_AttackComboCount > EAttackNo_None ){
		m_pEffects[m_AttackComboCount-1]->SetLocation( m_vPosition );
	}
	for( auto& e : m_pEffects ){
		e->Render();
	}
}

// �U���̓����蔻��.
void CPlayer::AttackCollision( CActor* pActor )
{
	const float attackLength = 10.0f;
	if( m_AttackComboCount <= EAttackNo_None ){
		m_AttackPosition = { 0.0f, -1.0f, 0.0f };
		return;
	}

	//
	//	�A�j���[�V����������܂Ńv���C���[�̖ڂ̑O�����U��.
	//

	// ��]���ňړ�.
	m_AttackPosition.x = m_vPosition.x - sinf( m_vRotation.y ) * attackLength;
	m_AttackPosition.y = 5.0f;
	m_AttackPosition.z = m_vPosition.z - cosf( m_vRotation.y ) * attackLength;

	// ���̂̓����蔻��.
	if( m_pAttackCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// �U���֐�.
	auto attackProc = [&]( float& life ){ life -= 10.0f; };
	pActor->LifeCalculation( attackProc );
	m_IsAttackHitCamera = true;
}

// �U���q�b�g���̃J��������.
void CPlayer::AttackHitCameraUpdate()
{
	if( m_IsAttackHitCamera == false ) return;
	m_CameraShakeCount += m_CameraShakeCountAdd;
	m_CameraHeight = m_CameraDefaultHeight + sinf( m_CameraShakeCount ) * (m_AttackComboCount*0.1f);
	if( m_CameraShakeCount <= m_CameraShakeTieme ) return;
	m_CameraShakeCount = 0.0f;
	m_CameraHeight = m_CameraDefaultHeight;
	m_IsAttackHitCamera = false;
}

// ����\�͉񕜍X�V�֐�.
void CPlayer::SpecialAbilityUpdate()
{							
	// �A�C�e���ł̉񕜏�ԂȂ�.
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_SPRecovery]->Update() == true ){
		m_SpecialAbilityValue = m_Parameter.SpecialAbilityValue;	// �񕜒l�����Ƃɖ߂�.
	}

	// ����\�͒l���ő�ȏ�Ȃ�I��.
	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_SpecialAbility += m_SpecialAbilityValue;	// ����\�͒l�����Z.

	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	m_SpecialAbility = m_Parameter.SpecialAbilityMax;	// �ő�l�𒴂��Ȃ��悤�ɂ���.
}

// �U����UP�X�V�֐�.
void CPlayer::AttackUpUpdate()
{
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Attack]->Update() == false ) return;
	m_AttackPower		= m_Parameter.AttackPower;
}

// �ړ����xUP�X�V�֐�.
void CPlayer::MoveSpeedUpUpdate()
{
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_MoveSpeedUp]->Update() == false ) return;
	m_MoveSpeed = m_Parameter.MoveSpeed;
}

// ��გ��̍X�V�֐�.
void CPlayer::ParalysisUpdate()
{
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Paralysis]->Update() == false ) return;
}

// �U���A�j���[�V����.
void CPlayer::AttackAnimation()
{
	// �L���[����Ȃ�I��.
	if( m_AttackDataQueue.empty() == true ) return;
	if( m_AttackDataQueue.front().Frame >= m_AttackDataQueue.front().EndFrame ){
		m_AttackDataQueue.pop();	// �L���[�̐擪�����o��.
		// �L���[����Ȃ�I��.
		if( m_AttackDataQueue.empty() == true ){
			// ���ꂪ�Ō�̍U���Ȃ̂ŁA�U���J�E���g��0�ɂ���.
			m_AttackComboCount = 0;	
			// �A�j���[�V������ҋ@�ɐݒ�.
			SetAnimation( enAnimNo::Dead );
			return;
		}
		// �V�����A�j���[�V�������Z�b�g����.
		SetAnimation( m_AttackDataQueue.front().AnimNo );
	}
	m_AttackDataQueue.front().Frame += 0.01;	// �t���[���̍X�V.
}

// �A�j���[�V�����ݒ�.
void CPlayer::SetAnimation( const enAnimNo& animNo )
{
	if( m_pSkinMesh == nullptr ) return;
	if( m_NowAnimNo == m_OldAnimNo ) return;
	m_OldAnimNo = m_NowAnimNo;
	m_NowAnimNo = animNo;
	m_pSkinMesh->ChangeAnimBlend( static_cast<int>(m_NowAnimNo), static_cast<int>(m_OldAnimNo) );
}

// �U���A�j���[�V�����t���[���̐ݒ�.
void CPlayer::SetAttackFrameList()
{
	if( m_pSkinMesh == nullptr ) return;
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(3) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(4) );
	m_AttackEnabledFrameList.emplace_back( m_pSkinMesh->GetAnimPeriod(1) );
}

// �U���̒ǉ����ł�����.
bool CPlayer::IsPushAttack()
{
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackComboMax ) return false;
	if( static_cast<int>(m_AttackDataQueue.size()) >= m_Parameter.AttackQueueMax ) return false;
	if( m_AttackDataQueue.empty() == false ){
		// �L���[�Ƀf�[�^�������Ă�����.
		// �o�߃t���[�����U���L���t���[�����傫����ΏI��.
		if( m_AttackDataQueue.front().Frame >= 
			m_AttackDataQueue.front().EnabledEndFrame ) return false;
	}

	SAttackData tmpAttackData;	// ���f�[�^��p��.
	switch( m_AttackComboCount )
	{
	case EAttackNo::EAttackNo_One:	// �U��1.
#ifndef INTERMEDIATE_ANNOUCEMENT_ATTACK
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Wait1;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[EAttackNo::EAttackNo_One-1];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(0);
		// �ŏ��̓A�j���[�V������ݒ肷��.
		SetAnimation( tmpAttackData.AnimNo );
#endif	// #if INTERMEDIATE_ANNOUCEMENT_ATTACK.
		
		break;
	case EAttackNo::EAttackNo_Two:	// �U��2.
#ifndef INTERMEDIATE_ANNOUCEMENT_ATTACK
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Happy;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[EAttackNo::EAttackNo_Two-1];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(4);
#endif	// #if INTERMEDIATE_ANNOUCEMENT_ATTACK.

		break;
	case EAttackNo::EAttackNo_Three:// �U��3.
#ifndef INTERMEDIATE_ANNOUCEMENT_ATTACK
		tmpAttackData.AnimNo = CPlayer::enAnimNo::Wait;
		tmpAttackData.EnabledEndFrame = m_AttackEnabledFrameList[EAttackNo::EAttackNo_Three-1];
		tmpAttackData.EndFrame = m_pSkinMesh->GetAnimPeriod(1);
#endif	// #if INTERMEDIATE_ANNOUCEMENT_ATTACK.

		break;
	default:
		break;
	}
	// �L���[�Ƀf�[�^��}��.
	m_AttackDataQueue.push( tmpAttackData );
	// �G�t�F�N�g���Đ�.
	m_pEffects[m_AttackComboCount-1]->Play( m_vPosition );
	return true;
}

// ���C�t�v�Z�֐�.
void CPlayer::LifeCalculation( const std::function<void(float&)>& proc )
{	
	proc( m_LifePoint );
	if( m_LifePoint < m_Parameter.LifeMax ) return;
	m_LifePoint = m_Parameter.LifeMax;
}

// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
void CPlayer::SetSPEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_SPRecovery]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_ItemSpecialAbilityValue, tmpTime );
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_SPRecovery]->SetTime( tmpTime );

	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_SPRecovery]->Set();
}

// �U���́A���͎��Ԑݒ�֐�.
void CPlayer::SetAttackEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Attack]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_AttackPower, tmpTime );
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Attack]->SetTime( tmpTime );
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Attack]->Set();
}

// �ړ����x�A���͎��Ԑݒ�֐�.
void CPlayer::SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_MoveSpeedUp]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_MoveSpeed, tmpTime );
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_MoveSpeedUp]->SetTime( tmpTime );
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_MoveSpeedUp]->Set();
}

// ��Ⴢ̐ݒ�.
void CPlayer::SetParalysisTime( const std::function<void(float&)>& proc )
{
	if( m_IsDuringAvoid == true ) return;
	float tmpTime = 0.0f;
	proc( tmpTime );
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Paralysis]->SetTime( tmpTime );
	m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Paralysis]->Set();
	CSoundManager::NoMultipleSEPlay("PlayerHitSE");

}

// �����蔻��̐ݒ�.
bool CPlayer::ColliderSetting()
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
#else
	if( m_pTempStaticMesh == nullptr ) return false;
	// ���b�V�����g�p���Ă̓����蔻�菉����.
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pCollManager->InitSphere( 
		m_pTempStaticMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
#endif	// #ifndef IS_MODEL_RENDER.

	// �U���p�̓����蔻�菉����.
	if( m_pAttackCollManager == nullptr ){
		m_pAttackCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pAttackCollManager->InitSphere(
		&m_AttackPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		1.0f ) )) return false;
	return true;
}

// �G�t�F�N�g�̐ݒ�.
bool CPlayer::EffectSetting()
{
	const char* effectNames[] =
	{
		ATTACK_ONE_EFFECT_NAME,
		ATTACK_TWO_EFFECT_NAME,
		ATTACK_THREE_EFFECT_NAME,
		SPECIAL_ABILITY_EFFECT_NAME,
		AVOIDANCE_EFFECT_NAME,
	};
	const int effectNum = sizeof(effectNames)/sizeof(effectNames[0]);
	// �������̍ő�l�ݒ�.
	m_pEffects.reserve(effectNum);

	for( int i = 0; i < effectNum; i++ ){
		m_pEffects.emplace_back( std::make_shared<CEffectManager>() );
		if( m_pEffects[i]->SetEffect( effectNames[i] ) == false ) return false;
	}

	return true;
}

// �G�f�B�b�g�p�̕`��֐�.
void CPlayer::EditRender()
{
#if _DEBUG
	ImGui::SetNextWindowSize( ImVec2(440.0f,470.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::SetNextWindowPos( ImVec2(WND_W-440,0.0f), ImGuiCond_::ImGuiCond_Once );
	ImGui::GetWindowSize();
	bool isOpen = true;
	ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.3f, 0.3f, 0.3f, 0.9f };
	ImGui::Begin( u8"�v���C���[�̐ݒ�", &isOpen );

	// �e�p�����[�^�̐ݒ�.
	ImGui::InputFloat( u8"�ړ����x", &m_MoveSpeed );
	ImGui::InputFloat( u8"�̗�", &m_LifePoint );
	ImGui::InputFloat( u8"�U����", &m_Parameter.AttackPower );
	ImGui::InputInt( u8"���G����", &m_Parameter.InvincibleTime );
	ImGui::InputFloat( u8"����\�͍ő�l", &m_Parameter.SpecialAbilityMax );
	ImGui::InputFloat( u8"����\�͉񕜒l", &m_Parameter.SpecialAbilityValue );
	ImGui::InputInt( u8"�U���R���{�ő吔", &m_Parameter.AttackComboMax );
	ImGui::InputInt( u8"�U���L���[�ǉ��ő吔", &m_Parameter.AttackQueueMax );
	ImGui::InputFloat( u8"����̈ړ�����", &m_Parameter.AvoidMoveDistance );
	ImGui::InputFloat( u8"���p�̈ړ����x", &m_Parameter.AvoidMoveSpeed );
	ImGui::InputFloat( u8"�J�����̈ړ����x", &m_Parameter.CameraMoveSpeed );
	ImGui::InputFloat( u8"�J�����̋���", &m_Parameter.CameraDistance );
	ImGui::InputFloat( u8"�J�����̍���", &m_Parameter.CameraHeight );
	ImGui::InputFloat( u8"�J���������_�̍���", &m_Parameter.CameraLookHeight );
	ImGui::InputFloat( u8"�J�����ړ��̕⊮�l", &m_Parameter.CameraLerpValue );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W X", &m_Parameter.SphereAdjPos.x );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Y", &m_Parameter.SphereAdjPos.y );
	ImGui::InputFloat( u8"�X�t�B�A�̒������W Z", &m_Parameter.SphereAdjPos.z );
	ImGui::InputFloat( u8"�X�t�B�A�̒������a", &m_Parameter.SphereAdjRadius );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Parameter );
		if( s_Success.IsSucceeded == true ){
			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_Parameter );
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.

	ImGui::End();
#endif	// #if _DEBUG.
}

// �f�o�b�O�p�̕`��.
void CPlayer::DebugRender()
{
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*-1, 0.0f } );
	CDebugText::Render( "- Player Parameter -" );

	// ���W�̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*0 ,0.0f } );
	CDebugText::Render( "----- Position -----" );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*1, 0.0f } );
	CDebugText::Render( "Pos_x : ", m_vPosition.x );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*2, 0.0f } );
	CDebugText::Render( "Pos_y : ", m_vPosition.y );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*3, 0.0f } );
	CDebugText::Render( "Pos_z : ", m_vPosition.z );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*5, 0.0f } );
	CDebugText::Render( "----- Rotation -----" );

	// ��]�̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*6, 0.0f } );
	CDebugText::Render( "Rot_x : ", m_vRotation.x );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*7, 0.0f } );
	CDebugText::Render( "Rot_y : ", m_vRotation.y );				 
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*8, 0.0f } );
	CDebugText::Render( "Rot_z : ", m_vRotation.z );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*10, 0.0f } );
	CDebugText::Render( "----- Parameter ----" );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*11, 0.0f } );
	CDebugText::Render( "LifePoint : ", m_LifePoint );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*12, 0.0f } );
	CDebugText::Render( "SpecialAbility : ", m_SpecialAbility );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*13, 0.0f } );
	CDebugText::Render( "AttackPower : ", m_AttackPower );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*14, 0.0f } );
	CDebugText::Render( "InvincibleTime : ", m_Parameter.InvincibleTime );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*16, 0.0f } );
	CDebugText::Render( "----- Animation ----" );

	// �A�j���[�V�����ԍ��̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*17, 0.0f } );
	CDebugText::Render( "Now_AnimationNo : ", (int)m_NowAnimNo );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*18, 0.0f } );
	CDebugText::Render( "Old_AnimationNo : ", (int)m_OldAnimNo );

	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*20, 0.0f } );
	CDebugText::Render( "------ Other -------" );

	// �U���J�E���g�̕`��.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*21, 0.0f } );
	CDebugText::Render( "AttackComboCount : ", m_AttackComboCount );

	// ��𒆂�.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*22, 0.0f } );
	CDebugText::Render( "IsDuringAvoid : ", m_IsDuringAvoid==true?"true":"false" );

	// ����\�͉񕜒���.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*28, 0.0f } );
	CDebugText::Render( "IsParalysis : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_SPRecovery]->IsUpdate()==true?"true":"false" );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*29, 0.0f } );
	CDebugText::Render( "ParalysisTime : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_SPRecovery]->GetTime()/FPS );

	// �U��UP����.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*23, 0.0f } );
	CDebugText::Render( "IsItemAttackUp : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Attack]->IsUpdate()==true?"true":"false" );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*24, 0.0f } );
	CDebugText::Render( "AttackUpTime : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Attack]->GetTime()/FPS );

	// �ړ����xUP����.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*24, 0.0f } );
	CDebugText::Render( "IsItemMoveSpeedUp : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_MoveSpeedUp]->IsUpdate()==true?"true":"false" );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*25, 0.0f } );
	CDebugText::Render( "MoveSpeedUpTime : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_MoveSpeedUp]->GetTime()/FPS );

	// ��გ���.
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*26, 0.0f } );
	CDebugText::Render( "IsParalysis : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Paralysis]->IsUpdate()==true?"true":"false" );
	CDebugText::SetPosition( { 0.0f, 80.0f+CDebugText::GetScale()*27, 0.0f } );
	CDebugText::Render( "ParalysisTime : ", m_pEffectTimers[EEffectTimerNo::EEffectTimerNo_Paralysis]->GetTime()/FPS );
}

// �E�B�W�F�b�g�ݒ�.
bool CPlayer::WidgetSetting()
{
	m_pWidget.emplace_back(std::make_shared<CLifeGauge>());
	m_pWidget.emplace_back(std::make_shared<CSpecialAbilityGauge>());

	for (const auto& s : m_pWidget)
	{
		if (s->Init() == false) return false;
	}

	return true;
}