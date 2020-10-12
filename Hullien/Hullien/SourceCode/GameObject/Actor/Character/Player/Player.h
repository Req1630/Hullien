#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include "PlayerParam.h"
#include <queue>

class CRotLookAtCenter;
class CCharacterWidget;
class CEffectManager;
class CEffectTimer;

class CPlayer : public CCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME			= "kaito_s";	// ���f����.
	const char* MODEL_TEMP_NAME		= "kaito";		// �����f����.

	// �g�p�U���G�t�F�N�g��.
	const char* ATTACK_ONE_EFFECT_NAME		= "slash_effect";
	const char* ATTACK_TWO_EFFECT_NAME		= "slash_effect";
	const char* ATTACK_THREE_EFFECT_NAME	= "slash_effect";
	// ����\�̓G�t�F�N�g��.
	const char* SPECIAL_ABILITY_EFFECT_NAME	= "uvtest";
	// ����G�t�F�N�g��.
	const char* AVOIDANCE_EFFECT_NAME		= "uvtest";

	// ���̃{�[����.
	const char* RIGHT_FOOT = "kaito_rifa_2_R_asi_4";
	const char* LEFT_FOOT = "kaito_rifa_2_L_asi_4";

	// ���̉���.
	const float VOICE_VOLUME = 1.5f;

	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(7.0));
	const float ROTATIONAL_SPEED = 0.2f;	// ��]���x.

	const float MOVE_SPEED_MUL_VALUE_ADD	= 0.035f;	// �|�����킹��ړ��ʂ̉��Z�l.
	const float MOVE_SPEED_MUL_VALUE_MAX	= 1.0f;		// �|�����킹��ړ��ʂ̍ő�l.

	const float TWO = 2.0f;
	const float CAMERA_COUNT_MAX			= 100.0f;
	const float CAMERA_BACK_DIRECTION_X		= 20.0f;
	const float CAMERA_BACK_DIRECTION_Y		= 10.0f;
	const float CAMERA_BACK_HEIGHT			= 8.0f;
	const float CAMERA_BACK_LERP_VALUE		= 0.1f;
	const float CAMERA_FREQUENCY_LOOKPOS	= 15.0f;	// �J���������ʒu�̎��g��.
	const float CAMERA_AMPLITUDE_LOOKPOS	= 0.01f;	// �J���������ʒu�̐U��.
	const float CAMERA_RETURN_COUNT_ADD		= 0.001f;
	const float CAMERA_RETURN_COUNT_MAX		= 0.1f;

public:
	CPlayer();
	virtual ~CPlayer();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override; 
	// �G�t�F�N�g�`��֐�.
	virtual void EffectRender() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �X�v���C�g�̕`��.
	virtual void SpriteRender() override;

	// ����\�͂��g���Ă��邩.
	bool IsSpecialAbility();
	// ���S������.
	bool IsDead(){ return m_LifePoint <= 0.0f; }
	// �J�����̕���.
	float GetCameraRadianX();

private:
	// ����֐�.
	void Controller();
	// �J��������.
	void CameraController();
	// �U������֐�.
	void AttackController();
	// ����\�͑���֐�.
	void SPController();
	// ��𑀍�֐�.
	void AvoidController();

	// �ړ��֐�.
	virtual void Move() override;
	// ��𓮍�֐�.
	void AvoidMove();

	// �ړI�̍��W�։�].
	bool TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian );

	// �U���̓����蔻��.
	void AttackCollision( CActor* pActor );
	// �U���q�b�g���̃J��������.
	void AttackHitCameraUpdate();
	// ����\�͎��̃J��������.
	void SPCameraUpdate();

	// ����\�͉񕜍X�V�֐�.
	void SpecialAbilityUpdate();
	// �U����UP�X�V�֐�.
	void AttackUpUpdate();
	// �ړ����xUP�X�V�֐�.
	void MoveSpeedUpUpdate();
	// ��გ��̍X�V�֐�.
	void ParalysisUpdate();

	// �U���A�j���[�V����.
	void AttackAnimation();
	// �A�j���[�V�����ݒ�.
	void SetAnimation( const player::EAnimNo& animNo );
	void SetAnimationBlend( const player::EAnimNo& animNo );

	// �U���A�j���[�V�����t���[�����X�g�̐ݒ�.
	void SetAttackFrameList();
	// �U���̒ǉ����ł�����.
	bool IsPushAttack();

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&)>& ) override;
	// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ) override;
	// �U���́A���͎��Ԑݒ�֐�.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ) override;
	// �ړ����x�A���͎��Ԑݒ�֐�.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ) override;
	// ��Ⴢ̐ݒ�.
	virtual void SetParalysisTime( const std::function<void(float&)>& ) override;

	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// �G�t�F�N�g�̐ݒ�.
	bool EffectSetting();

	// �G�f�B�b�g�p�̕`��֐�.
	void EditRender();

	// �T�E���h�̐ݒ�.
	bool SoundSetting();
	// ���ʂ̐ݒ�.
	void VolumeSetting(const char* soung, float volume);
	// �E�B�W�F�b�g�ݒ�.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;				// �J�����N���X.
	std::shared_ptr<CCamera>						m_pSPCamera;			// ����\�̓J�����N���X.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;				// Widget�N���X.
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// �U���p�̓����蔻��.
	D3DXVECTOR3		m_OldPosition;			// �O��̍��W.
	D3DXVECTOR3		m_GirlPosition;			// ���̎q�̍��W.
	player::EAnimNo	m_NowAnimNo;			// ���̃A�j���[�V�����ԍ�.
	player::EAnimNo	m_OldAnimNo;			// �O�̃A�j���[�V�����ԍ�.

	int								m_AttackComboCount;			// �U���R���{�J�E���g.
	std::vector<double>				m_AttackEnabledFrameList;	// �U���L���t���[���̃��X�g.
	std::queue<player::SAttackData>	m_AttackDataQueue;			// �U���f�[�^�̃L���[.
	D3DXVECTOR3						m_AttackPosition;			// �U���p�����蔻����W.

	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;	// �G�t�F�N�g.
	bool			m_IsDuringAvoid;	// ��𒆂��ǂ���.
	D3DXVECTOR3		m_AvoidVector;		// ����x�N�g��.

	SPlayerParam	m_Parameter;			// �p�����[�^�[.
	float			m_LifePoint;			// �̗�.
	float			m_SpecialAbility;		// ����\��.
	bool			m_IsYButtonPressed;		// Y�{�^���������ꂽ��.
	bool			m_IsUsableSP;			// ����\�͂��g������.

	float			m_SpecialAbilityValue;		// ����\�͉񕜗�.
	float			m_ItemSpecialAbilityValue;	// �A�C�e������\�͉񕜒l.
	float			m_AttackPower;				// �U����.
	float			m_MoveSpeed;				// �ړ����x.
	float			m_MoveSpeedMulValue;		// �ړ����x�Ɋ|�����킹��l.

	float			m_CameraDefaultHeight;		// �J�����̃f�t�H���g����.
	float			m_CameraHeight;				// �J�����̍���.


	D3DXVECTOR3		m_CameraNextPosition;		// �J�����̍��W.
	D3DXVECTOR3		m_CameraPosition;			// �J�����̍��W.
	D3DXVECTOR3		m_CameraLookPosition;		// �J�����̎��_���W.
	float			m_CameraCount;				// �J�����̃J�E���g.
	float			m_CameraReturnCount;		// �J�����̖߂�J�E���g.
	float			m_CameraLerp;				// �J�����ړ��̕�Ԓl.
	int				m_NowSPCameraStete;			// ����\�͂̃J�������.

	bool			m_IsAttackHitCamera;		// �U���q�b�g���̃J�������L����.
	float			m_CameraShakeCount;			// �J�����̗h��J�E���g.
	float			m_CameraShakeTime;			// �J�����̗h�ꎞ��.
	float			m_CameraShakeCountAdd;		// �J�����̗h��J�E���g���Z�l.
	std::vector<std::shared_ptr<CEffectTimer>>	m_pEffectTimers;	// ���ʎ��Ԍv���N���X.
	
	bool			m_IsAttackSE;				//�U��SE��炷��.

};

#endif	// #ifndef PLAYER_H.