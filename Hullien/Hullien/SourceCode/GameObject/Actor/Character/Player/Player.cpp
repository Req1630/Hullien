#include "Player.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\ChatacterWidget.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\LifeGauge\LifeGauge.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\BackGauge\BackGauge.h"
#include "..\..\..\Widget\SceneWidget\GameWidget\CharacterWidget\SpecialAbilityGauge\SpecialAbilityGauge.h"
#include "..\..\..\..\Common\Effect\EffectManager.h"
#include "..\..\Item\EffectTimer\EffectTimer.h"

#include "..\..\..\..\Utility\BitFlagManager\BitFlagManager.h"
#include "..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\..\Common\DebugText\DebugText.h"
#include "..\...\..\..\..\..\XAudio2\SoundManager.h"

CPlayer::CPlayer()
	: m_pCamera						( nullptr )
	, m_pSPCamera					( nullptr )
	, m_pAttackCollManager			( nullptr )
	, m_pEffects					()
	, m_pEffectTimers				( player::EEffectTimerNo_Max )
	, m_pWidget						()
	, m_AttackComboCount			( player::EAttackNo_None )
	, m_AttackDataQueue				()
	, m_AttackPosition				( FOR_INIT_ZERO_VEC3 )
	, m_GirlPosition				( FOR_INIT_ZERO_VEC3 )
	, m_AttackVector				( FOR_INIT_ZERO_VEC3 )
	, m_AvoidVector					( FOR_INIT_ZERO_VEC3 )
	, m_HitVector					( FOR_INIT_ZERO_VEC3 )
	, m_TargetVector				( FOR_INIT_ZERO_VEC3 )
	, m_AnimAdjDrggingParam			()
	, m_AttackAdjParam				()
	, m_Parameter					()
	, m_LifePoint					( FOR_INIT_ZERO_FLOAT )
	, m_SpecialAbility				( FOR_INIT_ZERO_FLOAT )
	, m_StatusFlag					( player::EStatusFlag_None )
	, m_AttackRangeLenght			( FOR_INIT_ZERO_FLOAT )
	, m_SpecialAbilityValue			( FOR_INIT_ZERO_FLOAT )
	, m_ItemSpecialAbilityValue		( FOR_INIT_ZERO_FLOAT )
	, m_AttackPower					( FOR_INIT_ZERO_FLOAT )
	, m_MoveSpeed					( FOR_INIT_ZERO_FLOAT )
	, m_MoveSpeedMulValue			( FOR_INIT_ZERO_FLOAT )
//	, m_HitStopCameraPosition		( FOR_INIT_ZERO_VEC3 )
	, m_CameraPosition				( FOR_INIT_ZERO_VEC3 )
	, m_CameraLookPosition			( FOR_INIT_ZERO_VEC3 )
	, m_CameraCount					( CAMERA_COUNT_MAX )
	, m_CameraLerp					( FOR_INIT_ZERO_FLOAT )
	, m_NowSPCameraStete			( player::ESPCameraState_Start )
	, m_ParalysisCameraShakeCount	( FOR_INIT_ZERO_INT )
	, m_OneStepCmaeraSeting			( false )
{
	m_ObjectTag = EObjectTag::Player;	// �v���C���[�^�O��ݒ�.
	m_NowAnimNo = player::EAnimNo_Wait;	// ���݂̃A�j���[�V������ҋ@�ɐݒ�.
	m_OldAnimNo = player::EAnimNo_None;	// �ߋ��̃A�j���[�V�����͖���.
	m_pCamera	= std::make_shared<CRotLookAtCenter>( true );
	m_pSPCamera = std::make_shared<CCamera>();
	m_AnimFrameList.resize( player::EAnimNo_Max );
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
	m_CameraLookPosition	= { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_CameraLerp			= m_Parameter.CameraLerpValue;

	m_ParalysisCameraShakeCount = FPS*2;

	// �ҋ@�A�j���[�V�����ɕύX.
	m_pSkinMesh->ChangeAnimSet_StartPos( player::EAnimNo_Wait, 0.0 );

	// �v���C���[�̉�]�x�N�g�����擾.
	m_AttackVector = { sinf(m_vRotation.y), FOR_INIT_ZERO_FLOAT, cosf(m_vRotation.y), };

	return true;
}

// �X�V�֐�.
void CPlayer::Update()
{
	// �A�j���[�V�����t���[���̍X�V.
	m_AnimFrameList[m_NowAnimNo].UpdateFrame( m_AnimSpeed );
//	if( m_IsHitStop == false ){
	// ��Ⴣ^�C�}�[�����삵�ĂȂ����.
	if( m_pEffectTimers[player::EEffectTimerNo_Paralysis]->IsUpdate() == false ){
		Controller();			// ����.
		AttackController();		// �U������.
		SPController();			// ����\�͑���.
		AvoidController();		// ��𑀍�.
		AttackAnimation();		// �U���A�j���[�V����.
		Move();					// �ړ�.
		AttackMove();			// �U���ړ�.
		AvoidMove();			// ��𓮍�.
		KnockBack();			// �m�b�N�o�b�N����֐�.
		Dead();					// ���S�֐�.
		m_HitStopCameraPosition = m_pCamera->GetPosition();
	} else {
		ParalysisUpdate();		// ��჎��̍X�V.
	}
	CameraController();			// �J��������.
	SPCameraUpdate();			// ����\�͎��̃J��������.
	SpecialAbilityUpdate();		// ����\�͉񕜍X�V.
	AttackUpUpdate();			// �U����UP�X�V.
	MoveSpeedUpUpdate();		// �ړ����xUP�X�V.

	CameraUpdate();				// �J�����̍X�V.
//	} else {
//		HitStopUpdate();			// �q�b�g�X�g�b�v�̍X�V.
//	}


	// �U���͈͂̃t���O������.
	bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_AttackRange );
	m_AttackRangeLenght = FOR_INIT_ZERO_FLOAT;	// �U���͈͂̒�����������.

	// �̗͂�1/4�ɂȂ�����SE��炷.
	if (m_LifePoint <= m_Parameter.LifeMax * PLAY_WARNING_SE_LIFE_ONE_THIRD)
	{
		if (CSoundManager::GetIsPlaySE("HP", 0) == false) {
			CSoundManager::PlaySE("HP");
		}
	}
	// ����.
	FootStep(RIGHT_FOOT_BONE_NAME, LEFT_FOOT_BONE_NAME);
}

// �`��֐�.
void CPlayer::Render()
{
	if( m_pSkinMesh == nullptr ) return;

	m_pSkinMesh->SetPosition( m_vPosition );
	m_pSkinMesh->SetRotation( m_vRotation );
	m_pSkinMesh->SetScale( m_vScale );
	m_pSkinMesh->SetAnimSpeed( m_AnimSpeed );
	m_pSkinMesh->Render();

#if _DEBUG
	if( m_pCollManager == nullptr ) return;
	m_pCollManager->DebugRender();
	if( m_AttackDataQueue.empty() == true ) return;
	if( m_AnimFrameList[m_NowAnimNo].NowFrame >= m_AttackDataQueue.front().AttackCollEndFrame ) return;
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

	AttackCollision( pActor );		// �U�����̓����蔻��.
	AttackRangeDecision( pActor );	// �U���͈͂̔���.
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
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_UsableSP )  == false ) return false;
	// ����\�͂��g����Ȃ�.
	bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_UsableSP );
	return true;			// true��Ԃ�.
}

// �J�����̕���.
float CPlayer::GetCameraRadianX()
{
	return m_pCamera->GetRadianX();
}

// �A�j���[�V�������ĊJ����.
void CPlayer::ResumeAnimation()
{
	// �ݒ��ʂɍs������J�����̐ݒ肪�ς���Ă��邩������Ȃ��̂�.
	// �ēx�ǂݍ���.
	if( m_pCamera->LoadParameter( true ) == false ){}
	m_AnimSpeed = m_OldAnimSpeed;
}

// �J������h�炷.
bool CPlayer::CameraShake()
{
	const float SHAKE_VALUE = 
		cosf(static_cast<float>(D3DX_PI) * TWO / 15.0f * m_ParalysisCameraShakeCount) * (m_ParalysisCameraShakeCount * 0.01f);
	const D3DXVECTOR3 lookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_pCamera->SetAddPositionValue( { SHAKE_VALUE, FOR_INIT_ZERO_FLOAT, SHAKE_VALUE } );
	m_ParalysisCameraShakeCount--;
	if( m_ParalysisCameraShakeCount < 0 ){
		m_ParalysisCameraShakeCount = FPS*2;
		return false;
	}
	return true;
}

// �p�����[�^�̐ݒ�(edit�ȂǂŎg�p).
void CPlayer::SetParameter( const player::SPlayerParam& param, const bool& isEdit )
{
	m_Parameter				= param;
	m_MoveSpeed				= m_Parameter.MoveSpeed;			// �ړ����x�̐ݒ�.
	m_AttackPower			= m_Parameter.AttackPower;			// �U���͂̐ݒ�.
	m_LifePoint				= m_Parameter.LifeMax;				// �̗͂̐ݒ�.
	m_SpecialAbilityValue	= m_Parameter.SpecialAbilityValue;	// ����\�͉񕜒l�̐ݒ�.
	m_CameraLookPosition	= { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	m_CameraLerp			= m_Parameter.CameraLerpValue;
	if( isEdit == true ) m_SpecialAbility = m_Parameter.SpecialAbilityMax;
	ColliderSetting();
}

// ����֐�.
void CPlayer::Controller()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== true ) return;	// �m�b�N�o�b�N���͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true ) return;	// ���S���͏I��.

	// �R���g���[���[��L�X�e�B�b�N�̌X�����擾.
	m_MoveVector.x = static_cast<float>(CXInput::LThumbX_Axis());
	m_MoveVector.z = static_cast<float>(CXInput::LThumbY_Axis());

	if( GetAsyncKeyState('W') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('S') & 0x8000 )	m_MoveVector.z = IDLE_THUMB_MIN;
	if( GetAsyncKeyState('D') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MAX;
	if( GetAsyncKeyState('A') & 0x8000 )	m_MoveVector.x = IDLE_THUMB_MIN;
}

// �J��������.
void CPlayer::CameraController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove )	== true ) return;	// SP�J�����̓��쒆�͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// ���S���͏I��.
	// �J�����̉�]�ړ�.
	// ������.
	if( CXInput::RThumbX_Axis() >= IDLE_THUMB_MAX ) 
		m_pCamera->DegreeHorizontalRightMove();	// �E����.
	if( CXInput::RThumbX_Axis() <= IDLE_THUMB_MIN ) 
		m_pCamera->DegreeHorizontalLeftMove();	// ������.

	if( GetAsyncKeyState(VK_RIGHT) & 0x8000 ) 
		m_pCamera->DegreeHorizontalRightMove();	// �E����.
	if( GetAsyncKeyState(VK_LEFT) & 0x8000 )
		m_pCamera->DegreeHorizontalLeftMove();	// ������.
}

// �U������֐�.
void CPlayer::AttackController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// ��𒆂͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// �m�b�N�o�b�N���͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove )	== true ) return;	// SP�J�����̓��쒆�͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// ���S���͏I��.

	// X�{�^�����������u�ԂɂȂ��.
	if( CInput::IsMomentPress( EKeyBind::Attack ) == true ){
		// �U���J�E���g���ő�ȏ�Ȃ�I��.
		if( m_AttackComboCount >= m_Parameter.AttackComboMax ) return;
		m_AttackComboCount++;	// �U���J�E���g�����Z.
		// �U���f�[�^���L���[�ɒǉ����ꂽ��I��.
		if( IsPushAttack() == true ){
			// �v���C���[�̍��G�͈͊O�Ȃ�I��.
			if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_AttackRange ) == true ){
				m_vRotation.y = atan2f( m_AttackVector.x, m_AttackVector.z );
			} else {
				// �v���C���[�̉�]�x�N�g�����擾.
				m_AttackVector = { sinf(m_vRotation.y), FOR_INIT_ZERO_FLOAT, cosf(m_vRotation.y), };
			}
			return;
		}
		m_AttackComboCount--;	// �U���J�E���g�����Z.
	}
}

// ����\�͑���֐�.
void CPlayer::SPController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack ) == true ) return;
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid ) == true ) return;
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;
	if( m_AttackComboCount > player::EAttackNo_None ) return;
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead ) == true ) return;

	if( m_SpecialAbility < m_Parameter.SpecialAbilityMax ) return;
	// Y�{�^���������ꂽ�u�ԂɂȂ��.
	if( CInput::IsMomentPress( EKeyBind::SpecialAbility ) == true ){
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_EndSPCameraMove );
		m_CameraPosition	= m_pCamera->GetPosition();
		m_SpecialAbility	= FOR_INIT_ZERO_FLOAT;

		SetAnimationBlend( player::EAnimNo_Wait );	// �ҋ@�A�j���[�V������ݒ�.
		CSoundManager::PlaySE("PlayerVoiceSpecial");
	}
}

// ��𑀍�֐�.
void CPlayer::AvoidController()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// ��𒆂͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// �m�b�N�o�b�N���͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;	// SP�J�������͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// ���S���͏I��.
// �U�����ɉ�����ł��Ȃ��ƁA�G����̍U���������ɂ����Ȃ�,
//	���S���₷���Ȃ�̂ŉ���ł���悤�Ɉȉ��̕����R�����g�A�E�g.
//	if( m_AttackComboCount > player::EAttackNo_None ) return;	// �U�����͔������Ȃ�.

	// ���ɉ���A�j���[�V������������I��.
	if( m_NowAnimNo == player::EAnimNo_Avoid ) return;

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ) return;
	// A�{�^���������ꂽ�u�ԂɂȂ��.
	if( CInput::IsMomentPress( EKeyBind::Avoidance ) == true ){
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_DuringAvoid );	// ����t���O�𗧂Ă�.
		m_AvoidVector = m_MoveVector;	// �ړ��x�N�g����ݒ�.
		m_pEffects[player::EEffectNo_Avoidance]->Play( { m_vPosition.x, m_vPosition.y+AVOID_EFFECT_RENDER_HEIGHT, m_vPosition.z } );
		// ����A�j���[�V�����̐ݒ�.
		SetAnimationBlend( player::EAnimNo_Avoid );
		// �U���L���[��S�����o��.
		for( int i = 0; i < static_cast<int>(m_AttackDataQueue.size()); i++ ) m_AttackDataQueue.pop();
		m_AttackComboCount = player::EAttackNo_None;
		CSoundManager::PlaySE("PlayerAvoidMove");
		CSoundManager::PlaySE("PlayerVoiceAvoidMove");
	}
}

// �ړ��֐�.
void CPlayer::Move()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// �m�b�N�o�b�N���͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// ��𒆂͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;	// SP�J�����̓��쒆�͏I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// ���S���͏I��.
	if( m_AttackComboCount > player::EAttackNo_None ) return;	// �U�����͏I��.

	// �e�l���L���͈͊O�Ȃ�I��.
	if( m_MoveVector.x < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.x &&
		m_MoveVector.z < IDLE_THUMB_MAX && IDLE_THUMB_MIN < m_MoveVector.z ){
		m_MoveSpeedMulValue = FOR_INIT_ZERO_FLOAT;	// ������.
		if( m_NowAnimNo == player::EAnimNo_Attack1 )	return;	// �A�j���[�V�������U��1,2,3�̎���.
		if( m_NowAnimNo == player::EAnimNo_Attack2 )	return;	//�@�ҋ@���[�V�����ɐݒ�ł��Ȃ��悤�ɂ���.
		if( m_NowAnimNo == player::EAnimNo_Attack3 )	return;	// 
		SetAnimationBlend( player::EAnimNo_Wait );	// �ҋ@�A�j���[�V������ݒ�.
		return;
	}

	// �|�����킹��ړ��ʂ̉��Z.
	m_MoveSpeedMulValue += m_Parameter.MoveSpeedMulAddValue;
	// ���l�𒴂��Ȃ��悤�ɂ���.
	if( m_MoveSpeedMulValue >= m_Parameter.MoveSpeedMulMaxValue ) m_MoveSpeedMulValue = m_Parameter.MoveSpeedMulMaxValue;

	// �^�[�Q�b�g�̃x�N�g����p�� �J�����̃��W�A���l�𑫂��Ē���.
	const float targetRot = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
	D3DXVECTOR3 targetVec = FOR_INIT_ZERO_VEC3;
	targetVec.x = sinf( targetRot );
	targetVec.z = cosf( targetRot );

	if( TargetRotation( targetVec, m_Parameter.RotationSpeed, m_Parameter.ToleranceRadian ) == true ){
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

// �U�����̈ړ�.
void CPlayer::AttackMove()
{
	if( m_AttackComboCount <= player::EAttackNo_None ) return;	// �U���J�E���g��������ΏI��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid ) == true ) return;	// ��𒆂Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== true ) return;	// �m�b�N�o�b�N���Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true ) return;	// ���S���Ȃ�I��.

	int attackNo = FOR_INIT_ZERO_INT;
	switch( m_NowAnimNo )
	{
	case player::EAnimNo_Attack1:	attackNo = player::EAttackNo_One-1;		break;
	case player::EAnimNo_Attack3:	attackNo = player::EAttackNo_Two-1;		break;
	case player::EAnimNo_Attack2:	attackNo = player::EAttackNo_Three-1;	break;
	default: break;
	}
	// �U���A�j���[�V�����̂���(��������)�𒲐�.
	AnimationDraggingAdj( attackNo );
}

// ��𓮍�֐�.
void CPlayer::AvoidMove()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )	== false ) return;	// ������ĂȂ���ΏI��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true ) return;	// ���S���Ă���ΏI��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== true ) return;	// �m�b�N�o�b�N���Ă���ΏI��.

	// �X�e�B�b�N�̌X���������Ɍ���.
	m_vRotation.y = atan2f( m_AvoidVector.x, m_AvoidVector.z );
	// �J�����̊p�x�Ƒ������킹��.
	m_vRotation.y += m_pCamera->GetRadianX();

	// ��]���ňړ�.
	m_vPosition.x -= sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	// �����Ȃ��ǂƂ̓����蔻��.
	if( CActor::IsCrashedWallX() == true ) m_vPosition.x += sinf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;
	if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += cosf( m_vRotation.y ) * m_Parameter.AvoidMoveSpeed;

	if( m_AnimFrameList[player::EAnimNo_Avoid].IsNowFrameOver() == false ) return;
	// ����A�j���[�V�����̌o�߃t���[�����I���t���[���𒴂��Ă�����.
	// ��𒆂���Ȃ�����.
	bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_DuringAvoid );
}

// �m�b�N�o�b�N����֐�.
void CPlayer::KnockBack()
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )	== false )	return;	// �m�b�N�o�b�N���ĂȂ���ΏI��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )		== true )	return;	// ���S���Ȃ�I��.

	m_vPosition.x -= m_MoveVector.x*m_Parameter.HitKnocBackMoveSpeed;
	m_vPosition.z -= m_MoveVector.z*m_Parameter.HitKnocBackMoveSpeed;

	// �����Ȃ��ǂƂ̓����蔻��.
	if( CActor::IsCrashedWallX() == true ) m_vPosition.x += m_MoveVector.x*m_Parameter.HitKnocBackMoveSpeed;
	if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += m_MoveVector.z*m_Parameter.HitKnocBackMoveSpeed;

	if( m_AnimFrameList[player::EAnimNo_Damage].IsNowFrameOver() == true ){
		bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_KnockBack );	// �m�b�N�o�b�N���~�߂�.
	}
}

// ���S����֐�.
void CPlayer::Dead()
{
	// ���S���ĂȂ���ΏI��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead ) == false ) return;

	// �A�j���[�V�����̂���𒲐����Ă���ꍇ�A�A�j���[�V�������x���������ɂ���.
	if( AnimationDraggingAdj( player::EDraggingAdjList_Dead ) == true ) m_AnimSpeed = DEFAULT_ANIM_HALF_SPEED;

	// �A�j���[�V�������Đ������Ȃ��悤�ɂ���.
	if( m_AnimFrameList[player::EAnimNo_Dead].IsNowFrameOver() == true ) STOP_ANIM_SPEED;
}

// �J�����̍X�V.
void CPlayer::CameraUpdate()
{
	// ���S���Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead ) == true ) return;

	if( m_OneStepCmaeraSeting == false ){
		m_pCamera->SetPosition( CCameraManager::GetPosition() );
		m_OneStepCmaeraSeting = true;
	}


	// �v���C���[�𒍎����ĉ�].
	m_pCamera->RotationLookAtObject( { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z }, m_CameraLerp );
	m_pCamera->SetLength( m_Parameter.CameraDistance );	// ���S�Ƃ̋�����ݒ�.
	m_pCamera->SetHeight( m_Parameter.CameraHeight );	// �����̐ݒ�.

	// Y�{�^��(����\�͂��g���Ă��Ȃ����).
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == false ){
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
	for( auto& e : m_pEffects ){
		e->SetLocation( m_vPosition );
		e->SetRotation( { FOR_INIT_ZERO_FLOAT, static_cast<float>(D3DX_PI)+m_vRotation.y, FOR_INIT_ZERO_FLOAT } );
		e->Render();
	}
}

// �U���̓����蔻��.
void CPlayer::AttackCollision( CActor* pActor )
{
	// �U���p�����蔻����W���Q�[���͈͊O�ɐݒ�.
	m_AttackPosition = m_AttackAdjParam.CollInvalidPosition;
	// �U�����ĂȂ��ꍇ�Ȃ�I��.
	if( m_AttackComboCount <= player::EAttackNo_None ) return;
	// ���݂̃A�j���[�V�����t���[���������蔻��̗L���͈͊O�Ȃ�I��.
	if( m_AnimFrameList[m_NowAnimNo].NowFrame >= m_AttackDataQueue.front().AttackCollEndFrame ) return;
	if( pActor->IsPossibleToHit() == false ) return;

	D3DXVECTOR3 littleFingerPos	= FOR_INIT_ZERO_VEC3;	// ���w�{�[���̍��W.
	D3DXVECTOR3 ringFingerPos	= FOR_INIT_ZERO_VEC3;	// ��w�̃{�[�����W.
	// �w�̃{�[�����W���擾.
	m_pSkinMesh->GetPosFromBone( LITTLE_FINGER_BONE_NAME, &littleFingerPos );
	m_pSkinMesh->GetPosFromBone( RING_FINGER_BONE_NAME, &ringFingerPos );
	// �w�̃x�N�g�����擾���ē����蔻��̍��W���v�Z.
	D3DXVECTOR3 fingerVec = ringFingerPos - littleFingerPos;
	D3DXVec3Normalize( &fingerVec, &fingerVec );
	m_AttackPosition = littleFingerPos + fingerVec * m_AttackAdjParam.CollisionDistance;

	// ���̂̓����蔻��.
	if( m_pAttackCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;

	// �m�b�N�o�b�N�p�̃x�N�g�����v�Z.
	D3DXVECTOR3 vec =
	{
		m_AttackPosition.x - pActor->GetPosition().x,
		FOR_INIT_ZERO_FLOAT,
		m_AttackPosition.z - pActor->GetPosition().z
	};
	D3DXVec3Normalize( &vec, &vec );
	vec.x *= m_AttackComboCount*m_Parameter.EnemyKnockBackPower;
	vec.z *= m_AttackComboCount*m_Parameter.EnemyKnockBackPower;
	pActor->SetVector( vec );

	// �U���֐�.
	pActor->LifeCalculation( [&]( float& life, bool& isAttack )
	{ 
		life	-= m_AttackPower;
		isAttack = true;
	});

//	if( m_IsHitStop == false ){
//		m_IsHitStop		= true;
//		m_HitStopTime	= m_AttackComboCount*2;
//		m_AnimSpeed		= 0.0;
//		m_HitStopCameraPosition = m_pCamera->GetPosition();
//	}
//	pActor->SetHitStopTime( m_HitStopTime );
	
	// ������炵�ĂȂ���ԂȂ�.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_AttackSE ) == false ){
		CSoundManager::PlaySE("PlayerAttackHit");
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_AttackSE );
	}
}

// �U���͈͂Ƃ̔���.
void CPlayer::AttackRangeDecision( CActor* pActor )
{
	// �v���C���[�ƉF���l�Ƃ̃x�N�g�����擾.
	D3DXVECTOR3 vec =
	{
		m_vPosition.x - pActor->GetPosition().x,
		FOR_INIT_ZERO_FLOAT,
		m_vPosition.z - pActor->GetPosition().z,
	};
	// �x�N�g���̒����Z�o.
	const float vecLength = sqrtf((vec.x * vec.x) + (vec.z * vec.z));
	float rot = m_vRotation.y;
	// �e�l���L���͈͓��Ȃ�ړ��x�N�g�������]�l���Z�o.
	if( m_MoveVector.x >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.x ||
		m_MoveVector.z >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.z ){
		rot = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
	}
	// �v���C���[�̉�]�x�N�g�����擾.
	const D3DXVECTOR3 playerVec = { sinf(rot), FOR_INIT_ZERO_FLOAT, cosf(rot), };

	// �F���l�Ƃ̋��������͈͊O�Ȃ̂ŏI��.
	if( vecLength > m_Parameter.AttackSearcLenght ) return;

	// �P�ʃx�N�g���ɕϊ�.
	D3DXVec3Normalize( &vec, &vec );

	// ���ς��擾.
	const float dot = vec.x * playerVec.x + vec.z * playerVec.z;

	// �_���͈͊O�ł���ΏI��.
	if( dot < cos(m_Parameter.AttackFanRadian) ) return;

	// ��������������ԂȂ璷��������.
	if( m_AttackRangeLenght <= 0.0f ) m_AttackRangeLenght = dot;
	// �O��̒���������̒������傫����ΏI��.
	if( m_AttackRangeLenght < dot ) return;

	m_AttackRangeLenght	= dot;
	m_AttackVector		= vec;
	bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_AttackRange );

}

// ����\�͎��̃J��������.
void CPlayer::SPCameraUpdate()
{
	// ����\�͂��g���Ȃ��Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == false ){
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
		if( TargetRotation( m_TargetVector, m_Parameter.RotationSpeed, m_Parameter.ToleranceRadian ) == true ){
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
			m_NowSPCameraStete = player::ESPCameraState_CameraShake;	// ���̏�Ԃւ��ǂ�.
			SetAnimationBlend( player::EAnimNo_SP );
		}
	}
		break;
	case player::ESPCameraState_CameraShake:
	{
		//-------------------------------------.
		// �J������h�炷.
		//-------------------------------------.

		if( m_AnimFrameList[player::EAnimNo_SP].NowFrame >= m_AnimFrameList[player::EAnimNo_SP].EndFrame-0.5 ){
			bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_UsableSP );	// SP���g����悤�ɂ���.
			m_AnimSpeed = STOP_ANIM_SPEED;
			m_AnimFrameList[player::EAnimNo_SP].UpdateFrame( DEFAULT_ANIM_SPEED );
		}
		if( m_AnimFrameList[player::EAnimNo_SP].IsNowFrameOver() == true ){
			m_CameraCount--;	// �J�E���g�̌��Z.
			// �J�����̗h��.
			const float SHAKE_VALUE = sinf(static_cast<float>(D3DX_PI) * TWO / CAMERA_FREQUENCY_LOOKPOS * m_CameraCount) * (m_CameraCount * CAMERA_AMPLITUDE_LOOKPOS);
			m_CameraLookPosition.x += SHAKE_VALUE;
			m_CameraLookPosition.y += SHAKE_VALUE;
		}
		if( m_CameraCount <= 20 ) m_AnimSpeed = DEFAULT_ANIM_SPEED;
		
		// �J�����J�E���g��0�ȉ��ɂȂ�����.
		if( m_CameraCount <= 0 ){
			m_CameraCount			= CAMERA_COUNT_MAX;			// �J�E���g��������.
			m_CameraNextPosition	= m_pCamera->GetPosition();	// ���C���J�����̍��W��ݒ�.
			m_CameraReturnCount		= FOR_INIT_ZERO_FLOAT;		// �J������߂��J�E���g��������.
			m_NowSPCameraStete		= player::ESPCameraState_CameraReturn;	// ���̏�Ԃֈړ�.
			m_AnimSpeed				= DEFAULT_ANIM_SPEED;
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
			bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_EndSPCameraMove );	// SP�J�����̃t���O������.
			m_CameraReturnCount = FOR_INIT_ZERO_FLOAT;			// �J������߂��J�E���g��������.
			m_CameraLerp		= FOR_INIT_ZERO_FLOAT;			// �J�����̕⊮�l��������.
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

// �q�b�g�X�g�b�v�̍X�V.
void CPlayer::HitStopUpdate()
{
	//if( m_IsHitStop == false ) return;

	//m_HitStopCount++;
	//const float shakeValue = 
	//	sinf( static_cast<float>(D3DX_PI)*TWO / static_cast<float>(m_HitStopCount)*TWO ) *
	//	static_cast<float>(m_HitStopTime*m_HitStopTime)/TWO * 0.08f;

	//const D3DXVECTOR3 lookPosition = { m_vPosition.x, m_Parameter.CameraLookHeight, m_vPosition.z };
	//D3DXVECTOR3 vec = lookPosition - m_pCamera->GetPosition();
	//D3DXVec3Normalize( &vec, &vec );
	//m_HitStopCameraPosition.x += vec.x * shakeValue;
	//m_HitStopCameraPosition.z += vec.z * shakeValue;

	//m_pSPCamera->SetLookPosition( lookPosition );
	//m_pSPCamera->SetPosition( m_HitStopCameraPosition );
	//// ����\�͗p�̃J�������}�l�[�W���[�ɐݒ�.
	//CCameraManager::SetCamera( m_pSPCamera );

	//if( m_HitStopCount < m_HitStopTime ) return;

	//m_IsHitStop		= false;
	//m_HitStopCount	= 0;
	//m_AnimSpeed		= DEFAULT_ANIM_SPEED;
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
	m_AnimSpeed = DEFAULT_ANIM_SPEED;
	m_pEffects[player::EEffectNo_Paralysis]->Stop();
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
			m_AttackComboCount = player::EAttackNo_None;	
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
		// �G�t�F�N�g���Đ�.
		m_pEffects[m_AttackComboCount-1]->Play( {m_vPosition.x, m_vPosition.y+ATTACK_EFFECT_RENDER_HEIGHT, m_vPosition.z } );
		// �U��SE��炷.
		CSoundManager::PlaySE("PlayerAttack");
		if(m_AttackComboCount == player::EAttackNo_Two){
			CSoundManager::PlaySE("PlayerVoiceAttack2");
		}
		if(m_AttackComboCount == player::EAttackNo_Three){
			CSoundManager::PlaySE("PlayerVoiceAttack3");
		}
		// �U���̓����蔻��̃T�C�Y��ύX����.
		if( FAILED( m_pAttackCollManager->InitSphere(
			&m_AttackPosition,
			&m_vRotation,
			&m_vScale.x,
			FOR_INIT_ZERO_VEC3,
			m_AttackAdjParam.CollisionRadius[m_AttackComboCount-1] ) )) return;
		// �e�l���L���͈͓��Ȃ�x�N�g�������]�l������.
		if( m_MoveVector.x >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.x ||
			m_MoveVector.z >= IDLE_THUMB_MAX || IDLE_THUMB_MIN >= m_MoveVector.z ){
			// �^�[�Q�b�g�̃x�N�g����p�� �J�����̃��W�A���l�𑫂��Ē���.
			m_vRotation.y = atan2f( m_MoveVector.x, m_MoveVector.z ) + m_pCamera->GetRadianX();
		}
		// �V�����A�j���[�V�������Z�b�g����.
		SetAnimation( m_AttackDataQueue.front().AnimNo );
		bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_AttackSE );
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
	const auto setAttackData = [&]( const player::EAnimNo& animNo, const int& atkNo )
	{
		tmpAttackData.AnimNo			= animNo;
		tmpAttackData.EnabledEndFrame	= m_AnimFrameList[animNo].EndFrame - m_AttackAdjParam.EnabledEndFrame[atkNo];
		tmpAttackData.EndFrame			= m_AnimFrameList[animNo].EndFrame;
		tmpAttackData.AttackCollEndFrame= m_AnimFrameList[animNo].EndFrame - m_AttackAdjParam.CollEnabledEndFrame[atkNo];
	};

	// "player::EAttackNo_One"�� ���l"1"�Ȃ̂�,
	//	-1���Ĕz��̓Y�����𒲐�����.
	const int attackAdjNo = m_AttackComboCount-1;

	switch( m_AttackComboCount )
	{
	case player::EAttackNo_One:	// �U��1.
		// �U�����̓����蔻���ݒ肷��.
		if( FAILED( m_pAttackCollManager->InitSphere(
			&m_AttackPosition,
			&m_vRotation,
			&m_vScale.x,
			FOR_INIT_ZERO_VEC3,
			m_AttackAdjParam.CollisionRadius[attackAdjNo] ) )) return false;
		setAttackData( player::EAnimNo_Attack1, attackAdjNo );
		// �ŏ��̍U���̓A�j���[�V������ݒ肷��.
		SetAnimation( tmpAttackData.AnimNo );
		m_pEffects[player::EEffectNo_AttackOne]->Play( m_vPosition );
		CSoundManager::PlaySE("PlayerAttack");
		CSoundManager::PlaySE("PlayerVoiceAttack1");
		bit::OffBitFlag( &m_StatusFlag, player::EStatusFlag_AttackSE );
		break;

	case player::EAttackNo_Two:	// �U��2.
		setAttackData( player::EAnimNo_Attack2, attackAdjNo );
		break;

	case player::EAttackNo_Three:// �U��3.
		setAttackData( player::EAnimNo_Attack3, attackAdjNo );
		break;

	default:
		break;
	}
	
	// �L���[�Ƀf�[�^��}��.
	m_AttackDataQueue.push( tmpAttackData );
	
	return true;
}

// ���C�t�v�Z�֐�.
void CPlayer::LifeCalculation( const std::function<void(float&,bool&)>& proc )
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return; // SP�J���������삵�Ă���Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return; // �m�b�N�o�b�N���Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return; // ���S���Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return; // ��𒆂Ȃ�I��.
	
	bool isAttack = false;
	const float oldLifePoint = m_LifePoint;
	proc( m_LifePoint, isAttack );

	// ��𒆂����U����H����Ă�����.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )	== true &&
		isAttack == true ){
		m_LifePoint = oldLifePoint;	// �O��̗̑͂�����.
	}
	// �U����H�������.
	if( isAttack == true ){
		if (m_LifePoint > FOR_INIT_ZERO_FLOAT) CSoundManager::PlaySE("PlayerVoiceHit");
		// �_���[�W�A�j���[�V������ݒ�.
		SetAnimation( player::EAnimNo_Damage );
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_KnockBack );
		m_vRotation.y	= atan2( m_HitVector.x, m_HitVector.z )+static_cast<float>(D3DX_PI);
		m_MoveVector	= m_HitVector;
		// �U���L���[��S�����o��.
		for( int i = FOR_INIT_ZERO_INT; i < static_cast<int>(m_AttackDataQueue.size()); i++ ) m_AttackDataQueue.pop();
		m_AttackComboCount = player::EAttackNo_None;
	}
	// �̗͂��Ȃ��Ȃ�����.
	if( m_LifePoint <= 0.0f ){
		CSoundManager::PlaySE("PlayerVoiceDead");
		// �U���L���[��S�����o��.
		for( int i = FOR_INIT_ZERO_INT; i < static_cast<int>(m_AttackDataQueue.size()); i++ ) m_AttackDataQueue.pop();
		m_AttackComboCount = player::EAttackNo_None;
		// ���S�A�j���[�V������ݒ�.
		SetAnimation( player::EAnimNo_Dead );
		bit::OnBitFlag( &m_StatusFlag, player::EStatusFlag_Dead );
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

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( m_ItemSpecialAbilityValue, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->SetTime( tmpTime );

	if( m_SpecialAbility >= m_Parameter.SpecialAbilityMax ) return;
	m_pEffectTimers[player::EEffectTimerNo_SPRecovery]->Set();
}

// �U���́A���͎��Ԑݒ�֐�.
void CPlayer::SetAttackEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_Attack]->IsUpdate() == true ) return;

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( m_AttackPower, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Attack]->Set();
}

// �ړ����x�A���͎��Ԑݒ�֐�.
void CPlayer::SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& proc )
{
	if( m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->IsUpdate() == true ) return;

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( m_MoveSpeed, tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_MoveSpeedUp]->Set();
}

// ��Ⴢ̐ݒ�.
void CPlayer::SetParalysisTime( const std::function<void(float&)>& proc )
{
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_DuringAvoid )		== true ) return;	// ��𒆂Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_KnockBack )		== true ) return;	// �m�b�N�o�b�N���Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_EndSPCameraMove ) == true ) return;	// SP�J���������쒆�Ȃ�I��.
	if( bit::IsBitFlag( m_StatusFlag, player::EStatusFlag_Dead )			== true ) return;	// ���S���Ȃ�I��.

	float tmpTime = FOR_INIT_ZERO_FLOAT;
	proc( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->SetTime( tmpTime );
	m_pEffectTimers[player::EEffectTimerNo_Paralysis]->Set();
	m_pEffects[player::EEffectNo_Paralysis]->Play( m_vPosition );
	m_AnimSpeed = STOP_ANIM_SPEED;
	CSoundManager::NoMultipleSEPlay("PlayerVoiceParalysis");
}

// �A�j���[�V�����̂���(��������)�𒲐�.
bool CPlayer::AnimationDraggingAdj( const int& animAdjNo )
{
	if( m_AnimAdjDrggingParam.StartFrame[animAdjNo] <= m_AnimFrameList[m_NowAnimNo].NowFrame && 
		m_AnimFrameList[m_NowAnimNo].NowFrame <= m_AnimAdjDrggingParam.EndFrame[animAdjNo] ){
		// �A�j���[�V�����̂���𒲐�.
		m_vPosition.x -= sinf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];
		m_vPosition.z -= cosf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];

		// �����Ȃ��ǂƂ̓����蔻��.
		if( CActor::IsCrashedWallX() == true ) m_vPosition.x += sinf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];
		if( CActor::IsCrashedWallZ() == true ) m_vPosition.z += cosf(m_vRotation.y)*m_AnimAdjDrggingParam.MoveSpeed[animAdjNo];

		return true;
	}
	return false;
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
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.SphereAdjRadius ) )) return false;
	if( FAILED( m_pCollManager->InitCapsule( 
		m_pSkinMesh->GetMesh(),
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		m_Parameter.SphereAdjPos,
		m_Parameter.CapsuleAdjRadius,
		FOR_INIT_ZERO_FLOAT ) )) return false;

	// �U���p�̓����蔻�菉����.
	if( m_pAttackCollManager == nullptr ){
		m_pAttackCollManager = std::make_shared<CCollisionManager>();
	}
	if( FAILED( m_pAttackCollManager->InitSphere(
		&m_AttackPosition,
		&m_vRotation,
		&m_vScale.x,
		FOR_INIT_ZERO_VEC3,
		m_AttackAdjParam.CollisionRadius[0] ) )) return false;
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
		PARALYSIS_EFFECT_NAME,
	};
	const int effectNum = sizeof(effectNames)/sizeof(effectNames[0]);
	// �������̍ő�l�ݒ�.
	m_pEffects.reserve(effectNum);

	for( int i = FOR_INIT_ZERO_INT; i < effectNum; i++ ){
		m_pEffects.emplace_back( std::make_shared<CEffectManager>() );
		if( m_pEffects[i]->SetEffect( effectNames[i] ) == false ) return false;
	}

	return true;
}

// �A�j���[�V�����t���[���̐ݒ�.
bool CPlayer::SetAnimFrameList()
{
	if( m_pSkinMesh == nullptr ) return false;
	// �����p�A�j���[�V�����t���[���̃��X�g��p��.
	player::SAnimationAdjFrameList list;
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimAdjList.bin", list ) == false ) return false;

	for( int i = player::EAnimNo_Begin; i < player::EAnimNo_End; i++ ){
		m_AnimFrameList.at(i) = { 0.0, m_pSkinMesh->GetAnimPeriod(i)-list.Frame[i] };
	}

	// �A�j���[�V�����̈������蒲�����X�g��ǂݍ���.
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam ) == false ) return false;
	// �U�������p�p�����[�^�[��ǂݍ���.
	if( CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam ) == false ) return false;

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
	VolumeSetting("PlayerVoiceDead",	VOICE_VOLUME);
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

	if( ImGui::TreeNode( u8"�p�����[�^�̐ݒ�" )){
		CImGuiManager::DragFloat( u8"�ړ����x",						&m_Parameter.MoveSpeed );
		CImGuiManager::DragFloat( u8"�̗�",							&m_Parameter.LifeMax );
		CImGuiManager::DragFloat( u8"�U����",						&m_Parameter.AttackPower );
		CImGuiManager::DragInt(   u8"���G����",						&m_Parameter.InvincibleTime );
		CImGuiManager::DragFloat( u8"����\�͍ő�l",				&m_Parameter.SpecialAbilityMax );
		CImGuiManager::DragFloat( u8"����\�͉񕜒l",				&m_Parameter.SpecialAbilityValue );
		CImGuiManager::DragInt(   u8"�U���R���{�ő吔",				&m_Parameter.AttackComboMax );
		CImGuiManager::DragInt(   u8"�U���L���[�ǉ��ő吔",			&m_Parameter.AttackQueueMax );
		CImGuiManager::DragFloat( u8"����̈ړ�����",				&m_Parameter.AvoidMoveDistance );
		CImGuiManager::DragFloat( u8"���p�̈ړ����x",				&m_Parameter.AvoidMoveSpeed );
		CImGuiManager::DragFloat( u8"�J�����̈ړ����x",				&m_Parameter.CameraMoveSpeed );
		CImGuiManager::DragFloat( u8"�J�����̋���",					&m_Parameter.CameraDistance );
		CImGuiManager::DragFloat( u8"�J�����̍���",					&m_Parameter.CameraHeight );
		CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W X",			&m_Parameter.SphereAdjPos.x );
		CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Y",			&m_Parameter.SphereAdjPos.y );
		CImGuiManager::DragFloat( u8"�X�t�B�A�̒������W Z",			&m_Parameter.SphereAdjPos.z );
		CImGuiManager::DragFloat( u8"�X�t�B�A�̒������a",			&m_Parameter.SphereAdjRadius );
		CImGuiManager::DragFloat( u8"�m�b�N�o�b�N�ړ����x",			&m_Parameter.HitKnocBackMoveSpeed );
		CImGuiManager::DragFloat( u8"��]�̋��e�͈�",				&m_Parameter.ToleranceRadian );
		CImGuiManager::DragFloat( u8"��]���x",						&m_Parameter.RotationSpeed );
		CImGuiManager::DragFloat( u8"�|�����킹��ړ��ʂ̉��Z�l",	&m_Parameter.MoveSpeedMulAddValue );
		CImGuiManager::DragFloat( u8"�|�����킹��ړ��ʂ̍ő�l",	&m_Parameter.MoveSpeedMulMaxValue );
		CImGuiManager::DragFloat( u8"�U���̍��G����",				&m_Parameter.AttackSearcLenght );
		CImGuiManager::DragFloat( u8"��̍U���͈�",					&m_Parameter.AttackFanRadian );
		CImGuiManager::DragFloat( u8"�G�̃m�b�N�o�b�N��",			&m_Parameter.EnemyKnockBackPower );	
		
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�A�j���[�V�����̈������蒲���p�����[�^�̐ݒ�" )){
		if( ImGui::TreeNode( u8"�U��1" )){
			CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack1] );
			CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack1] );
			CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack1] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"�U��2" )){
			CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack2] );
			CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack2] );
			CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack2] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"�U��3" )){
			CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Attack3] );
			CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Attack3] );
			CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Attack3] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"���S" )){
			CImGuiManager::DragDouble(	u8"�J�n�t���[��",		&m_AnimAdjDrggingParam.StartFrame[player::EDraggingAdjList_Dead] );
			CImGuiManager::DragDouble(	u8"�I���t���[��",		&m_AnimAdjDrggingParam.EndFrame[player::EDraggingAdjList_Dead] );
			CImGuiManager::DragFloat(	u8"�����p�̈ړ����x",	&m_AnimAdjDrggingParam.MoveSpeed[player::EDraggingAdjList_Dead] );
			ImGui::TreePop();
		}
		
		ImGui::TreePop();
	}
	if( ImGui::TreeNode( u8"�U�������p�����[�^�̐ݒ�" )){
		if( ImGui::TreeNode( u8"�U��1" )){
			CImGuiManager::DragDouble(	u8"�L���I���t���[��",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_One-1] );
			CImGuiManager::DragDouble(	u8"�����蔻��L���I���t���[��",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_One-1] );
			CImGuiManager::DragFloat(	u8"�����蔻�蔼�a",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_One-1] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"�U��2" )){
			CImGuiManager::DragDouble(	u8"�L���I���t���[��",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Two-1] );
			CImGuiManager::DragDouble(	u8"�����蔻��L���I���t���[��",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Two-1] );
			CImGuiManager::DragFloat(	u8"�����蔻�蔼�a",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Two-1] );
			ImGui::TreePop();
		}
		if( ImGui::TreeNode( u8"�U��3" )){
			CImGuiManager::DragDouble(	u8"�L���I���t���[��",			&m_AttackAdjParam.EnabledEndFrame[player::EAttackNo_Three-1] );
			CImGuiManager::DragDouble(	u8"�����蔻��L���I���t���[��",	&m_AttackAdjParam.CollEnabledEndFrame[player::EAttackNo_Three-1] );
			CImGuiManager::DragFloat(	u8"�����蔻�蔼�a",				&m_AttackAdjParam.CollisionRadius[player::EAttackNo_Three-1] );
			ImGui::TreePop();
		}
		
		ImGui::TreePop();
	}

	// �p�����[�^�̐ݒ�.
	SetParameter( m_Parameter );

	static CImGuiManager::SSuccess s_Success;
	if( ImGui::Button(u8"�Ǎ�") == true ){
		// �f�[�^�̓ǂݍ���.
		s_Success.IsSucceeded = CFileManager::BinaryReading( PARAMETER_FILE_PATH, m_Parameter );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
		if( s_Success.IsSucceeded == true ){
			ColliderSetting();
		}
	}
	ImGui::SameLine();
	if( ImGui::Button(u8"�ۑ�") == true ){
		// �f�[�^�̏�������.
		s_Success.IsSucceeded = CFileManager::BinaryWriting( PARAMETER_FILE_PATH, m_Parameter );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAnimDragging.bin", m_AnimAdjDrggingParam );
		s_Success.IsSucceeded = CFileManager::BinaryReading( "Data\\GameParam\\Player\\PlayerAtkAdjParam.bin", m_AttackAdjParam );
	}
	ImGui::SameLine();
	s_Success.Render();	// �������ǂ�����`��.

	ImGui::End();
#endif	// #if _DEBUG.
}

// �E�B�W�F�b�g�ݒ�.
bool CPlayer::WidgetSetting()
{
	m_pWidget.emplace_back(std::make_shared<CBackGauge>());
	m_pWidget.emplace_back(std::make_shared<CLifeGauge>());
	m_pWidget.emplace_back(std::make_shared<CSpecialAbilityGauge>());

	for (const auto& s : m_pWidget)
	{
		if (s->Init() == false) return false;
	}

	return true;
}