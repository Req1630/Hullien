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
	, m_pSPCamera					( nullptr )
	, m_pWidget						()
	, m_pAttackCollManager			( nullptr )
	, m_GirlPosition				( 0.0f, 0.0f, 0.0f )
	, m_AttackComboCount			( player::EAttackNo_None )
	, m_AttackDataQueue				()
	, m_AttackPosition				( 0.0f, 0.0f, 0.0f )
	, m_pEffects					()
	, m_AvoidVector					( 0.0f, 0.0f, 0.0f )
	, m_HitVector					( 0.0f, 0.0f, 0.0f )
	, m_TargetVector				( 0.0f, 0.0f, 0.0f )
	, m_Parameter					()
	, m_LifePoint					( 0.0f )
	, m_SpecialAbility				( 0.0f )
	, m_IsDuringAvoid				( false )
	, m_IsYButtonPressed			( false )
	, m_IsUsableSP					( false )
	, m_IsDead						( false )
	, m_IsKnockBack					( false )
	, m_SpecialAbilityValue			( 0.0f )
	, m_ItemSpecialAbilityValue		( 0.0f )
	, m_AttackPower					( 0.0f )
	, m_MoveSpeed					( 0.0f )
	, m_MoveSpeedMulValue			( 0.0f )
	, m_AnimFrameList				( player::EAnimNo_Max )
	, m_CameraDefaultHeight			( 0.0f )
	, m_CameraHeight				( 0.0f )
	, m_CameraPosition				( 0.0f, 0.0f, 0.0f )
	, m_CameraLookPosition			( 0.0f, 0.0f, 0.0f )
	, m_CameraCount					( CAMERA_COUNT_MAX )
	, m_CameraLerp					( 0.0f )
	, m_NowSPCameraStete			( player::ESPCameraState_Start )
	, m_IsAttackHitCamera			( false )
	, m_CameraShakeCount			( 0.0f )
	, m_CameraShakeTime				( 10.0f )
	, m_CameraShakeCountAdd			( 1.0f )
	, m_pEffectTimers				( player::EEffectTimerNo_Max )
	, m_IsAttackSE					( false )
{
	m_ObjectTag = EObjectTag::Player;	// �v���C���[�^�O��ݒ�.
	m_NowAnimNo = player::EAnimNo_Wait;	// ���݂̃A�j���[�V������ҋ@�ɐݒ�.
	m_OldAnimNo = player::EAnimNo_None;	// �ߋ��̃A�j���[�V�����͖���.
	m_pCamera	= std::make_shared<CRotLookAtCenter>();
	m_pSPCamera = std::make_shared<CCamera>();
	for( auto& e : m_pEffectTimers ) e = std::make_shared<CEffectTimer>();
}

CPlayer::~CPlayer()
{
}

// �������֐�.
bool CPlayer::Init()
{
	// �p�����[�^�̎擾.
	if( ParameterSetting( PARAMETER_FILE_PATH, m_Parameter ) == false ) return false;
	if( GetModel( MODEL_NAME )		== false ) return false;	// ���f���̎擾.
	if( SetAnimFrameList()			== false ) return false;	// �A�j���[�V�����t���[�����X�g�̐ݒ�.
	if( FootStepCollisionSetting()	== false ) return false;	// �����p�����蔻��̐ݒ�.
	if( ColliderSetting()			== false ) return false;	// �����蔻��̐ݒ�.
	if( WidgetSetting()				== false ) return false;	// UI�̐ݒ�.
	if( EffectSetting()				== false ) return false;	// �G�t�F�N�g�̐ݒ�.
	if( SoundSetting()				== false ) return false;	// �T�E���h�̐ݒ�.
	m_MoveSpeed		= m_Parameter.MoveSpeed;	// �ړ����x�̐ݒ�.
	m_AttackPower	= m_Parameter.AttackPower;	// �U���͂̐ݒ�.
	m_LifePoint		= m_Parameter.LifeMax;		// �̗͂̐ݒ�.
	m_SpecialAbilityValue	= m_Parameter.SpecialAbilityValue;	// ����\�͉񕜒l�̐ݒ�.
	m_CameraHeight			= m_CameraDefaultHeight = m_Parameter.CameraHeight;
	m_CameraLookPosition	= { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_CameraLerp			= m_Parameter.CameraLerpValue;

	// �ҋ@�A�j���[�V�����ɕύX.
	m_pSkinMesh->ChangeAnimSet_StartPos( player::EAnimNo_Wait, 0.0 );

	return true;
}

// �X�V�֐�.
void CPlayer::Update()
{
	// ��Ⴣ^�C�}�[�����삵�ĂȂ����.
	if( m_pEffectTimers[player::EEffectTimerNo_Paralysis]->IsUpdate() == false ){
		Controller();			// ����.
		AttackController();		// �U������.
		SPController();			// ����\�͑���.
		AvoidController();		// ��𑀍�.
		AttackAnimation();		// �U���A�j���[�V����.
		Move();					// �ړ�.
		AvoidMove();			// ��𓮍�.
		KnockBack();			// �m�b�N�o�b�N����֐�.
		Dead();					// ���S�֐�.
	} else {
		ParalysisUpdate();		// ��჎��̍X�V.
	}
	CameraController();			// �J��������.
	AttackHitCameraUpdate();	// �U���q�b�g���̃J��������.
	SPCameraUpdate();			// ����\�͎��̃J��������.
	SpecialAbilityUpdate();		// ����\�͉񕜍X�V.
	AttackUpUpdate();			// �U����UP�X�V.
	MoveSpeedUpUpdate();		// �ړ����xUP�X�V.

	CameraUpdate();				// �J�����̍X�V.

	// �̗͂�1/3�ɂȂ�����SE��炷.
	if (m_LifePoint <= m_Parameter.LifeMax / 3)
	{
		if (CSoundManager::GetIsPlaySE("HP", 0) == true) return;
		CSoundManager::NoMultipleSEPlay("HP");
	}

	// ����.
	FootStep(RIGHT_FOOT, LEFT_FOOT);
}

// �`��֐�.
void CPlayer::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vSclae );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render();

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
	if( actor.GetObjectTag() != EObjectTag::Girl ) return;
	m_GirlPosition = actor.GetPosition();
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
#endif	// #if _DEBUG.
}

// ����\�͂��g���Ă��邩.
bool CPlayer::IsSpecialAbility()
{
	if( m_IsUsableSP  == false ) return false;
	// ����\�͂��g����Ȃ�.
	m_IsUsableSP = false;	// ����������.
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
	if( m_IsKnockBack == true ) return;	// �m�b�N�o�b�N���Ȃ�I��.
	if( m_IsDead == true ) return;

	// �R���g���[���[��L�X�e�B�b�N�̌X�����擾.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	if( GetAsyncKeyState(VK_UP) & 0x8000 )		m_MoveVector.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_DOWN) & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MIN;
}

// �J��������.
void CPlayer::CameraController()
{
	if( m_IsYButtonPressed == true ) return;
	if( m_IsDead == true ) return;
	// �J�����̉�]�ړ�.
	// ������.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalMove(  m_Parameter.CameraMoveSpeed );	// �E����.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalMove( -m_Parameter.CameraMoveSpeed );	// ������.

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000) 
		m_pCamera->DegreeHorizontalMove(m_Parameter.CameraMoveSpeed);
	if (GetAsyncKeyState(VK_SHIFT)  & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000)	
		m_pCamera->DegreeHorizontalMove(-m_Parameter.CameraMoveSpeed);
}

// �U������֐�.
void CPlayer::AttackController()
{
	// �m�b�N�o�b�N���Ȃ�I��.
	if( m_IsKnockBack == true ) return;
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;
	// Y�{�^���F����\�͂��g���Ă�����.
	if( m_IsYButtonPressed == true ) return;
	if( m_IsDead == true ) return;

	// X�{�^�����������u�Ԃ���Ȃ���ΏI��.
	if( CXInput::X_Button() != CXInput::enPRESSED_MOMENT ) return;
	// �U���J�E���g���ő�ȏ�Ȃ�I��.
	if( m_AttackComboCount >= m_Parameter.AttackComboMax ) return;
	m_AttackComboCount++;	// �U���J�E���g�����Z.
	// �U���f�[�^���L���[�ɒǉ����ꂽ��I��.
	if( IsPushAttack() == true ) return;
	m_AttackComboCount--;	// �U���J�E���g�����Z.
}

// ����\�͑���֐�.
void CPlayer::SPController()
{
	// �m�b�N�o�b�N���Ȃ�I��.
	if( m_IsKnockBack == true ) return;
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;
	// �U�����͈ړ����Ȃ�.
	if( m_AttackComboCount > 0 ) return;
	if( m_IsDead == true ) return;

	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	// Y�{�^���������ꂽ�u�Ԃ���Ȃ���ΏI��.
	if( CXInput::Y_Button() != CXInput::enPRESSED_MOMENT ) return;
	CSoundManager::NoMultipleSEPlay("PlayerVoiceSpecial");
	m_CameraPosition = m_pCamera->GetPosition();
	m_SpecialAbility = 0.0f;
	m_IsYButtonPressed = true;

	SetAnimationBlend( player::EAnimNo_Wait );	// �ҋ@�A�j���[�V������ݒ�.
}

// ��𑀍�֐�.
void CPlayer::AvoidController()
{
	// �m�b�N�o�b�N���Ȃ�I��.
	if( m_IsKnockBack == true ) return;
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;
	// Y�{�^���F����\�͂��g���Ă�����.
	if( m_IsYButtonPressed == true ) return;
	// �U�����͔������Ȃ�.
	if( m_AttackComboCount > 0 ) return;
	if( m_IsDead == true ) return;
	// ���ɉ���A�j���[�V������������I��.
	if( m_NowAnimNo == player::EAnimNo_Avoid ) return;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// A�{�^���������ꂽ�u�Ԃ���Ȃ���ΏI��.
	if( CXInput::A_Button() != CXInput::enPRESSED_MOMENT ) return;
	m_IsDuringAvoid		= true;
	m_AnimFrameList[player::EAnimNo_Avoid].NowFrame	= 0.0;
	m_AvoidVector = m_MoveVector;	// �ړ��x�N�g����ݒ�.
	CSoundManager::PlaySE("PlayerAvoidMove");
	CSoundManager::PlaySE("PlayerVoiceAvoidMove");

	m_pEffects[player::enEffectNo_Avoidance]->Play( m_vPosition );
	
	// ����A�j���[�V�����̐ݒ�.
	SetAnimationBlend( player::EAnimNo_Avoid );
}

// �ړ��֐�.
void CPlayer::Move()
{
	// �m�b�N�o�b�N���Ȃ�I��.
	if( m_IsKnockBack == true ) return;
	// ��𒆂Ȃ�I��.
	if( m_IsDuringAvoid == true ) return;
	// �U�����͈ړ����Ȃ�.
	if( m_AttackComboCount > 0 ) return;
	// Y�{�^���F����\�͂��g���Ă�����.
	if( m_IsYButtonPressed == true ) return;
	if( m_IsDead == true ) return;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
		m_MoveSpeedMulValue = 0.0f;	// ������.
		if( m_NowAnimNo == player::EAnimNo_Attack1 )	return;	// �A�j���[�V�������U��1,2,3�̎���.
		if( m_NowAnimNo == player::EAnimNo_Attack2 )	return;	//�@�ҋ@���[�V�����ɐݒ�ł��Ȃ��悤�ɂ���.
		if( m_NowAnimNo == player::EAnimNo_Attack3 )	return;	// 
		SetAnimationBlend( player::EAnimNo_Wait );	// �ҋ@�A�j���[�V������ݒ�.
		return;
	}

	// �|�����킹��ړ��ʂ̉��Z.
	m_MoveSpeedMulValue += MOVE_SPEED_MUL_VALUE_ADD;
	// ���l�𒴂��Ȃ��悤�ɂ���.
	if( m_MoveSpeedMulValue >= MOVE_SPEED_MUL_VALUE_MAX ) m_MoveSpeedMulValue = MOVE_SPEED_MUL_VALUE_MAX;

	// �^�[�Q�b�g�̃x�N�g����p�� �J�����̃��W�A���l�𑫂��Ē���.
	const float targetRot = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
	D3DXVECTOR3 targetVec = { 0.0f, 0.0f, 0.0f };
	targetVec.x = sinf( targetRot );
	targetVec.z = cosf( targetRot );

	if( TargetRotation( targetVec, ROTATIONAL_SPEED, TOLERANCE_RADIAN ) == true ){
		// �^�[�Q�b�g�̉�]���擾.
		m_vRotation.y = targetRot;

		// ��]���ňړ�.
		m_vPosition.x -= targetVec.x * m_MoveSpeed * m_MoveSpeedMulValue;
		m_vPosition.z -= targetVec.z * m_MoveSpeed * m_MoveSpeedMulValue;
		// �����Ȃ��ǂƂ̓����蔻��.
		if( CActor::IsCrashedWallX() == true ) m_vPosition.x += targetVec.x * m_MoveSpeed * m_MoveSpeedMulValue;
		if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += targetVec.z * m_MoveSpeed * m_MoveSpeedMulValue;

		if( m_NowAnimNo == player::EAnimNo_Attack1 )	return;	// �A�j���[�V�������U��1,2,3�̏ꍇ�́A
		if( m_NowAnimNo == player::EAnimNo_Attack2 )	return;	//�@�ړ����Ȃ��̂ŁA
		if( m_NowAnimNo == player::EAnimNo_Attack3 )	return;	//	�A�j���[�V������ݒ肹���ɏI��.
		SetAnimationBlend( player::EAnimNo_Walk );
	}
}

// ��𓮍�֐�.
void CPlayer::AvoidMove()
{
	// �m�b�N�o�b�N���Ȃ�I��.
	if( m_IsKnockBack == true ) return;
	// ��𒆂���Ȃ���ΏI��.
	if( m_IsDuringAvoid == false ) return;
	if( m_IsDead == true ) return;

	// �X�e�B�b�N�̌X���������Ɍ���.
	m_vRotation.y = atan2f( m_AvoidVector.x, m_AvoidVector.z );
	// �J�����̊p�x�Ƒ������킹��.
	m_vRotation.y += m_pCamera->GetRadianX();

	m_AnimFrameList[player::EAnimNo_Avoid].NowFrame += m_AnimSpeed;	// �o�߃t���[���̉��Z.

	// ��]���ňړ�.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	// �����Ȃ��ǂƂ̓����蔻��.
	if( CActor::IsCrashedWallX() == true ) m_vPosition.x += sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;

	if( m_AnimFrameList[player::EAnimNo_Avoid].IsNowFrameOver() == false ) return;
	// ����A�j���[�V�����̌o�߃t���[�����I���t���[���𒴂��Ă�����.
	m_IsDuringAvoid = false;	// ��𒆂���Ȃ�����.
}

// �m�b�N�o�b�N����֐�.
void CPlayer::KnockBack()
{
	if( m_IsKnockBack == false ) return;
	if( m_IsDead == true ) return;

	m_AnimFrameList[player::EAnimNo_Damage].NowFrame += m_AnimSpeed;

	m_vPosition.x -= m_MoveVector.x*DAMAGE_HIT_KNOC_BACK_SPEED;
	m_vPosition.z -= m_MoveVector.z*DAMAGE_HIT_KNOC_BACK_SPEED;

	// �����Ȃ��ǂƂ̓����蔻��.
	if( CActor::IsCrashedWallX() == true ) m_vPosition.x -= m_MoveVector.x*DAMAGE_HIT_KNOC_BACK_SPEED;
	if( CActor::IsCrashedWallZ() == true ) m_vPosition.z -= m_MoveVector.z*DAMAGE_HIT_KNOC_BACK_SPEED;

	if( m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver() == true ){
		m_AnimFrameList[player::EAnimNo_Damage].NowFrame = 0.0;
		m_IsKnockBack = false;
	}
}

// ���S����֐�.
void CPlayer::Dead()
{
	if( m_IsDead == false ) return;

	m_AnimFrameList[player::EAnimNo_Dead].NowFrame += m_AnimSpeed;	// �A�j���[�V�����t���[���̉��Z.

	// �A�j���[�V�����t���[�������͈͓̔��Ȃ�.
	if( DEAD_CERTAIN_RANGE_ANIM_FRAME_MIN <= m_AnimFrameList[player::EAnimNo_Dead].NowFrame && 
		m_AnimFrameList[player::EAnimNo_Dead].NowFrame <= DEAD_CERTAIN_RANGE_ANIM_FRAME_MAX ){
		// �x�N�g�����g�p���đO�ɍ��W���ړ�.
		//	(�A�j���[�V�����̈�������̒����̂���).
		m_vPosition.x -= m_MoveVector.x*DEAD_ANIM_DRAGING_ADJ_SPEED;
		m_vPosition.z -= m_MoveVector.z*DEAD_ANIM_DRAGING_ADJ_SPEED;

		// �����Ȃ��ǂƂ̓����蔻��.
		if( CActor::IsCrashedWallX() == true ) m_vPosition.x += m_MoveVector.x*DEAD_ANIM_DRAGING_ADJ_SPEED;
		if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += m_MoveVector.z*DEAD_ANIM_DRAGING_ADJ_SPEED;

		// �A�j���[�V�������x���������ɂ���.
		m_AnimSpeed = 0.005;
	}
	// ���̃t���[���ȏ�ɂȂ�����A�j���[�V�������x���������ɂ���.
	if( m_AnimFrameList[player::EAnimNo_Dead].NowFrame >= 0.5 ) m_AnimSpeed = 0.005;
	// �A�j���[�V�������Đ������Ȃ��悤�ɂ���.
	if( m_AnimFrameList[player::EAnimNo_Dead].IsNowFrameOver() == true ) m_AnimSpeed = 0.0;
}

// �J�����̍X�V.
void CPlayer::CameraUpdate()
{
	if( m_IsDead == true ) return;

	// �v���C���[�𒍎����ĉ�].
	m_pCamera->RotationLookAtObject( { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z }, m_CameraLerp );
	m_pCamera->SetLength( m_Parameter.CameraDistance );	// ���S�Ƃ̋�����ݒ�.
	m_pCamera->SetHeight( m_CameraHeight );				// �����̐ݒ�.

	// Y�{�^��(����\�͂��g���Ă��Ȃ����).
	if( m_IsYButtonPressed == false ){
		m_CameraLerp = m_Parameter.CameraLerpValue;
		// ���C���J�������}�l�[�W���[�ɐݒ�.
		CCameraManager::SetCamera( m_pCamera );
	} else {
		// ����\�͗p�̃J�������}�l�[�W���[�ɐݒ�.
		CCameraManager::SetCamera( m_pSPCamera );
	}
}

// �G�t�F�N�g�`��֐�.
void CPlayer::EffectRender()
{
	// �U���J�E���g�����l��葽�����.
	if( m_AttackComboCount > player::EAttackNo_None ){
		// �U���G�t�F�N�g�̍��W��ݒ肷��.
		m_pEffects[m_AttackComboCount-1]->SetLocation( m_vPosition );
	}
	// �G�t�F�N�g�̕`��.
	for( auto& e : m_pEffects ) e->Render();
}

// �U���̓����蔻��.
void CPlayer::AttackCollision( CActor* pActor )
{
	if( m_AttackComboCount <= player::EAttackNo_None ){
		// �U�����ĂȂ��ꍇ�A�U���p�����蔻����W�������̂ق��ɐݒ�.
		m_AttackPosition = { 0.0f, -1.0f, 0.0f };
		return;
	}

	// �Ƃ肠�������̃{�[�����W��ݒ�.
	m_pSkinMesh->GetPosFromBone( "kaito_rifa_2_L_ude_1", &m_AttackPosition );
	// ���̂̓����蔻��.
	if( m_pAttackCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// �U���֐�.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{ 
		life	-= m_AttackPower;
		isAttack = true;
	});
	
	if (m_IsAttackSE == false)
	{
		CSoundManager::PlaySE("PlayerAttackHit");
		m_IsAttackSE = true;
	}
	m_IsAttackHitCamera = true;
}

// �U���q�b�g���̃J��������.
void CPlayer::AttackHitCameraUpdate()
{
	if( m_IsAttackHitCamera == false ) return;
	m_CameraShakeCount += m_CameraShakeCountAdd;
	m_CameraHeight = m_CameraDefaultHeight + sinf( m_CameraShakeCount ) * (m_AttackComboCount*0.1f);
	if( m_CameraShakeCount <= m_CameraShakeTime ) return;
	m_CameraShakeCount = 0.0f;
	m_CameraHeight = m_CameraDefaultHeight;
	m_IsAttackHitCamera = false;
}

// ����\�͎��̃J��������.
void CPlayer::SPCameraUpdate()
{
	// ����\�͂��g���Ȃ��Ȃ�I��.
	if( m_IsYButtonPressed == false ){
		m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		return;
	}

	m_CameraLookPosition = m_GirlPosition;	// ���̎q�����_�ɂ���.
	// ��]�l��ݒ肷��.
	const float targetRot = atan2f( 
		m_vPosition.x - m_CameraLookPosition.x,
		m_vPosition.z - m_CameraLookPosition.z );

	switch( m_NowSPCameraStete )
	{
	case player::ESPCameraState_TargetRotation:
	{
		//-------------------------------------.
		// �v���C���[�����̎q�̂ق��֌�����.
		//-------------------------------------.
		// �ړI�̃x�N�g����p��.
		m_TargetVector.x = sinf( targetRot );
		m_TargetVector.z = cosf( targetRot );
		
		// �ړI�̍��W�Ɍ����ĉ�].
		if( TargetRotation( m_TargetVector, ROTATIONAL_SPEED, TOLERANCE_RADIAN ) == true ){
			m_vRotation.y		= targetRot;							// �x�N�g���̉�]���擾.
			m_NowSPCameraStete	= player::ESPCameraState_PlayerBack;	// ���̏�Ԃֈړ�.
		} else {
			// ��]���̓v���C���[�����_�ɐݒ�.
			m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		}
		break;
	}
	case player::ESPCameraState_PlayerBack:
	{
		//-------------------------------------.
		// �J�������v���C���[�̌��Ɉړ�������.
		//-------------------------------------.
		m_vRotation.y = targetRot;	// �x�N�g���̉�]���擾.
		// �J�����̍��W��ݒ肷��.
		m_CameraNextPosition = m_vPosition;
		m_CameraNextPosition.x += m_TargetVector.x * CAMERA_BACK_DIRECTION_X;
		m_CameraNextPosition.z += m_TargetVector.z * CAMERA_BACK_DIRECTION_Z;
		m_CameraNextPosition.y = CAMERA_BACK_HEIGHT;
		// �v���C���[�̌��Ɉړ�.
		D3DXVec3Lerp( &m_CameraPosition, &m_CameraPosition, &m_CameraNextPosition, CAMERA_BACK_LERP_VALUE );
		if( fabsf(D3DXVec3Length(&m_CameraPosition) - D3DXVec3Length(&m_CameraNextPosition)) < 0.01f ){
//			m_IsUsableSP = true;										// ����\�͂��g��.
			m_NowSPCameraStete = player::ESPCameraState_CameraShake;	// ���̏�Ԃւ��ǂ�.
			SetAnimationBlend( player::EAnimNo_SP );
			m_AnimFrameList[player::EAnimNo_SP].NowFrame = 0.0;
		}
	}
		break;
	case player::ESPCameraState_CameraShake:
	{
		//-------------------------------------.
		// �J������h�炷.
		//-------------------------------------.
		m_AnimFrameList[player::EAnimNo_SP].NowFrame += 0.01;

		if( m_AnimFrameList[player::EAnimNo_SP].NowFrame >= m_AnimFrameList[player::EAnimNo_SP].EndFrame-0.5 ){
			m_IsUsableSP = true;
			m_AnimSpeed = 0.0;
		}
		if( m_AnimFrameList[player::EAnimNo_SP].IsNowFrameOver() == true ){
			m_CameraCount--;	// �J�E���g�̌��Z.
			// �J�����̗h��.
			const float SHAKE_VALUE = sinf(static_cast<float>(D3DX_PI) * TWO / CAMERA_FREQUENCY_LOOKPOS * m_CameraCount) * (m_CameraCount * CAMERA_AMPLITUDE_LOOKPOS);
			m_CameraLookPosition.x += SHAKE_VALUE;
			m_CameraLookPosition.y += SHAKE_VALUE;
		}
		if( m_CameraCount <= 20 ) m_AnimSpeed = 0.01;
		
		// �J�����J�E���g��0�ȉ��ɂȂ�����.
		if( m_CameraCount <= 0 ){
			m_CameraCount			= CAMERA_COUNT_MAX;			// �J�E���g��������.
			m_CameraNextPosition	= m_pCamera->GetPosition();	// ���C���J�����̍��W��ݒ�.
			m_CameraReturnCount		= 0.0f;						// �J������߂��J�E���g��������.
			m_NowSPCameraStete		= player::ESPCameraState_CameraReturn;	// ���̏�Ԃֈړ�.
			m_AnimSpeed				= 0.01;
			SetAnimationBlend( player::EAnimNo_Wait );
		}
	}
		break;
	case player::ESPCameraState_CameraReturn:
	{
		//-------------------------------------.
		// �J���������Ƃ̈ʒu�ɖ߂�.
		//-------------------------------------.
		m_CameraReturnCount		+= CAMERA_RETURN_COUNT_ADD;	// �J������߂��J�E���g�����Z.
		// ���l�������Ȃ��悤�ɂ���.
		if( m_CameraReturnCount >= CAMERA_RETURN_COUNT_MAX ) m_CameraReturnCount = CAMERA_RETURN_COUNT_MAX;
		// ���_���v���C���[�ɐݒ�.
		m_CameraLookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
		// �v���C���[�̌��Ɉړ�.
		D3DXVec3Lerp( &m_CameraPosition, &m_CameraPosition, &m_CameraNextPosition, m_CameraReturnCount );
		// �J������߂��J�E���g�����l�ȏ�Ȃ�.
		if( m_CameraReturnCount >= CAMERA_RETURN_COUNT_MAX ){
			m_pCamera->SetLookPosition( m_CameraLookPosition );	// ���C���J�����̎��_���W��ݒ�.
			m_pCamera->SetPosition( m_CameraPosition );			// ���C���J�����̍��W��ݒ�.
			m_IsYButtonPressed	= false;	// ����\�͂��I�������̂Ńt���O������.
			m_CameraReturnCount = 0.0f;		// �J������߂��J�E���g��������.
			m_CameraLerp		= 0.0f;		// �J�����̕⊮�l��������.
			m_NowSPCameraStete	= player::ESPCameraState_Start;	// ���߂̏�Ԃ֖߂�.

			return;	// ����J�����͐ݒ肵�Ȃ��Ă��悢�̂ł����ŏI��.
		}
	}
		break;
	default:
		break;
	}
	// ����\�̓J�����p�̍��W�Ǝ��_���W��ݒ�.
	m_pSPCamera->SetLookPosition( m_CameraLookPosition );
	m_pSPCamera->SetPosition( m_CameraPosition );
}

// ����\�͉񕜍X�V�֐�.
void CPlayer::SpecialAbilityUpdate()
{							
	// �A�C�e���ł̉񕜏�ԂȂ�.
	if( m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->Update() == true ){
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
	if( m_pEffectTimers[player::EEffectTimerNo_Attack]->Update() == false ) return;
	// �^�C�}�[���I�������猳�̒l�ɖ߂�.
	m_AttackPower = m_Parameter.AttackPower;
}

// �ړ����xUP�X�V�֐�.
void CPlayer::MoveSpeedUpUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->Update() == false ) return;
	// �^�C�}�[���I�������猳�̒l�ɖ߂�.
	m_MoveSpeed	= m_Parameter.MoveSpeed;
}

// ��გ��̍X�V�֐�.
void CPlayer::ParalysisUpdate()
{
	if( m_pEffectTimers[player::EEffectTimerNo_Paralysis]->Update() == false ) return;
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
			// �ҋ@���[�V�������A�ړ����[�V�����ɂ��邩�̔�r.
			// �e�l���L���͈͊O�Ȃ�I��.
			if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
				m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
				// �A�j���[�V������ҋ@�ɐݒ�.
				SetAnimationBlend( player::EAnimNo_Wait );
			} else {
				// �A�j���[�V�������ړ��ɐݒ�.
				SetAnimationBlend( player::EAnimNo_Walk );
			}
			return;
		}
		// �V�����A�j���[�V�������Z�b�g����.
		SetAnimation( m_AttackDataQueue.front().AnimNo );
		// �U��SE��炷.
		CSoundManager::PlaySE("PlayerAttack");
		if(m_AttackComboCount == player::EAttackNo_Two)		CSoundManager::PlaySE("PlayerVoiceAttack2");
		if(m_AttackComboCount == player::EAttackNo_Three)	CSoundManager::PlaySE("PlayerVoiceAttack3");
	}
	m_AttackDataQueue.front().Frame += m_AnimSpeed;	// �t���[���̍X�V.
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

	player::SAttackData tmpAttackData;	// ���f�[�^��p��.
	// ���f�[�^�̐ݒ�.
	const auto setAttackData = [&]( const player::EAnimNo& animNo, const double& adjEndFrame )
	{
		tmpAttackData.AnimNo			= animNo;
		tmpAttackData.EnabledEndFrame	= m_AnimFrameList[animNo].EndFrame - adjEndFrame;
		tmpAttackData.EndFrame			= m_AnimFrameList[animNo].EndFrame;
	};

	switch( m_AttackComboCount )
	{
	case player::EAttackNo_One:	// �U��1.
		setAttackData( player::EAnimNo_Attack1, ATTACK1_ADJ_ENABLED_END_FRAME );
		// �ŏ��̍U���̓A�j���[�V������ݒ肷��.
		SetAnimation( tmpAttackData.AnimNo );
		CSoundManager::PlaySE("PlayerAttack");
		CSoundManager::PlaySE("PlayerVoiceAttack1");
		break;

	case player::EAttackNo_Two:	// �U��2.
		setAttackData( player::EAnimNo_Attack2, ATTACK2_ADJ_ENABLED_END_FRAME );
		break;

	case player::EAttackNo_Three:// �U��3.
		setAttackData( player::EAnimNo_Attack3, ATTACK3_ADJ_ENABLED_END_FRAME );
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
void CPlayer::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{	
	if( m_IsDuringAvoid == true ) return;		// ��𒆂Ȃ�I��.
	if( m_IsYButtonPressed == true ) return;	// ����\�͂��g�p���Ă���Ȃ�I��.
	if( m_IsKnockBack == true ) return;			// �m�b�N�o�b�N���Ȃ�I��.

	bool isAttack = false;
	proc( m_LifePoint, isAttack );
	// �U����H�������.
	if( isAttack == true ){
		// �_���[�W�A�j���[�V������ݒ�.
		SetAnimation( player::EAnimNo_Damage );
		m_AnimFrameList[player::EAnimNo_Damage].NowFrame = 0.0;
		m_IsKnockBack	= true;
		m_vRotation.y	= atan2( m_HitVector.x, m_HitVector.z )+static_cast<float>(D3DX_PI);
		m_MoveVector	= m_HitVector;
	}
	// �̗͂��Ȃ��Ȃ�����.
	if( m_LifePoint <= 0.0f ){
		// ���S�A�j���[�V������ݒ�.
		SetAnimation( player::EAnimNo_Dead );
		m_AnimFrameList[player::EAnimNo_Dead].NowFrame	= 0.0;
		m_IsDead		= true;
		m_vRotation.y	= atan2( m_HitVector.x, m_HitVector.z )+static_cast<float>(D3DX_PI);
		m_MoveVector	= -m_HitVector;	// �x�N�g���l�𔽓]���Ē���.
	}

	// �̗͂����l�𒴂��Ȃ��悤�ɂ���.
	if( m_LifePoint < m_Parameter.LifeMax ) return;
	m_LifePoint = m_Parameter.LifeMax;
}

// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
void CPlayer::SetSPEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_ItemSpecialAbilityValue, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->SetTime( tmpTime );

	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->Set();
}

// �U���́A���͎��Ԑݒ�֐�.
void CPlayer::SetAttackEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_Attack]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_AttackPower, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->Set();
}

// �ړ����x�A���͎��Ԑݒ�֐�.
void CPlayer::SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->IsUpdate() == true ) return;

	float tmpTime = 0.0f;
	proc( m_MoveSpeed, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->Set();
}

// ��Ⴢ̐ݒ�.
void CPlayer::SetParalysisTime( const std::function<void(float&)>& proc )
{
	if( m_IsDuringAvoid == true ) return;
	float tmpTime = 0.0f;
	proc( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->Set();
	CSoundManager::NoMultipleSEPlay("PlayerVoiceParalysis");
}

// �����蔻��̐ݒ�.
bool CPlayer::ColliderSetting()
{
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
	if( FAILED( m_pCollManager->InitCapsule( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		m_Parameter.SphereAdjPos,
		-18.0f,
		0.0f ) )) return false;

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

// �A�j���[�V�����t���[���̐ݒ�.
bool CPlayer::SetAnimFrameList()
{
	// �����p�A�j���[�V�����t���[���̃��X�g��p��.
	const double animAdjFrames[] =
	{
		ANIM_ADJ_FRAME_Wait,
		ANIM_ADJ_FRAME_Walk,
		ANIM_ADJ_FRAME_Attack1,
		ANIM_ADJ_FRAME_Attack2,
		ANIM_ADJ_FRAME_Attack3,
		ANIM_ADJ_FRAME_Avoid,
		ANIM_ADJ_FRAME_SP,	
		ANIM_ADJ_FRAME_Damage,
		ANIM_ADJ_FRAME_Dead,
	};
	if( m_pSkinMesh == nullptr ) return false;
	for( int i = player::EAnimNo_Begin; i < player::EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-animAdjFrames[i] };
	}
	return true;
}

// �T�E���h�̐ݒ�.
bool CPlayer::SoundSetting()
{
	VolumeSetting("PlayerVoiceAvoidMove", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceHit",		  VOICE_VOLUME);
	VolumeSetting("PlayerVoiceParalysis", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceAttack1", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceAttack2", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceAttack3", VOICE_VOLUME);
	VolumeSetting("PlayerVoiceSpecial", VOICE_VOLUME);
	return true;
}

// ���ʂ̐ݒ�.
void CPlayer::VolumeSetting(const char * soung, float volume)
{
	CSoundManager::SetAnotherSEVolume(soung, volume);
	CSoundManager::SetUseAnotherSEVolumeFlag(soung, true);
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