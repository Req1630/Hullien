#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include "PlayerParam.h"
#include <queue>

class CRotLookAtCenter;	// �J�����N���X.
class CCharacterWidget;	// �L�����N�^UI�N���X.
class CEffectManager;	// �G�t�F�N�g�N���X.
class CEffectTimer;		// �A�C�e���̎��ԊǗ��N���X.

class CPlayer : public CCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";

	// ���f����.
	const char*	MODEL_NAME			= "kaito_s";

	// �G�t�F�N�g��.
	const char* ATTACK_ONE_EFFECT_NAME		= "slash_effect_1";	// �U��1.
	const char* ATTACK_TWO_EFFECT_NAME		= "slash_effect_2";	// �U��2.
	const char* ATTACK_THREE_EFFECT_NAME	= "slash_effect_3";	// �U��3.
	const char* SPECIAL_ABILITY_EFFECT_NAME	= "uvtest";			// ����\��.
	const char* AVOIDANCE_EFFECT_NAME		= "sandSmoke";		// ���.
	const char* PARALYSIS_EFFECT_NAME		= "uvtest";			// ���.

	// �{�[����.
	const char* RIGHT_FOOT_BONE_NAME	= "kaito_rifa_2_R_asi_4";		// �E��.
	const char* LEFT_FOOT_BONE_NAME		= "kaito_rifa_2_L_asi_4";		// ����.
	const char* LITTLE_FINGER_BONE_NAME	= "kaito_rifa_2_L_yubi_5_1";	// ���w.
	const char* RING_FINGER_BONE_NAME	= "kaito_rifa_2_L_yubi_4_1";	// ��w.

	const float TWO = 2.0f;
	const float CAMERA_COUNT_MAX			= 100.0f;	// �J�����̃J�E���g�ő�l.
	const float CAMERA_BACK_DIRECTION_X		= 20.0f;	// �J�����̋��� : x;
	const float CAMERA_BACK_DIRECTION_Z		= 10.0f;	// �J�����̋��� : z;
	const float CAMERA_BACK_HEIGHT			= 8.0f;		// �J�����̍���.
	const float CAMERA_BACK_LERP_VALUE		= 0.1f;		// �J�����̕⊮�l.
	const float CAMERA_FREQUENCY_LOOKPOS	= 15.0f;	// �J���������ʒu�̎��g��.
	const float CAMERA_AMPLITUDE_LOOKPOS	= 0.01f;	// �J���������ʒu�̐U��.
	const float CAMERA_RETURN_COUNT_ADD		= 0.001f;	// �J�����̖߂�Ƃ��̃J�E���g���Z�l.
	const float CAMERA_RETURN_COUNT_MAX		= 0.1f;		// �J�����̖߂�Ƃ��̃J�E���g�ő�l.

	const float ATTACK_EFFECT_RENDER_HEIGHT	= 5.0f;				// �U���G�t�F�N�g�̕`�捂��.
	const float AVOID_EFFECT_RENDER_HEIGHT	= 10.0f;			// ����G�t�F�N�g�̕`�捂��.

	const float PLAY_WARNING_SE_LIFE_ONE_THIRD	= 1.0f / 4.0f;	// �x�������Đ�����̗͂�

	// ���̉���.
	const float VOICE_VOLUME = 1.5f;

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
	inline bool IsDead(){ return m_AnimFrameList[player::EAnimNo_Dead].IsNowFrameOver(); }
	// �J�����̕���.
	float GetCameraRadianX();

	// �x�N�g���̎擾.
	virtual void SetVector( const D3DXVECTOR3& vec ) override { m_HitVector = vec; }
	// �A�j���[�V�������ĊJ����.
	virtual void ResumeAnimation() override;
	// �J������h�炷.
	bool CameraShake();

	// �p�����[�^�̐ݒ�(edit�ȂǂŎg�p).
	void SetParameter( const player::SPlayerParam& param, const bool& isEdit = false );

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

	// �ړ��֐�(�A�j���[�V�����Ƃ̂���̒���).
	virtual void Move() override;
	// �U�����̈ړ�.
	void AttackMove();
	// ��𓮍�֐�.
	void AvoidMove();
	// �m�b�N�o�b�N����֐�.
	void KnockBack();
	// ���S����֐�.
	void Dead();
	// �J�����̍X�V.
	void CameraUpdate();

	// �U���̓����蔻��.
	void AttackCollision( CActor* pActor );
	// �U���͈͂Ƃ̔���.
	void AttackRangeDecision( CActor* pActor );
	// ����\�͎��̃J��������.
	void SPCameraUpdate();

	// �q�b�g�X�g�b�v�̍X�V.
	void HitStopUpdate();

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
	// �U���̒ǉ����ł�����.
	bool IsPushAttack();

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
	// ����\�͉񕜎��ԁA���͎��Ԑݒ�֐�.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ) override;
	// �U���́A���͎��Ԑݒ�֐�.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ) override;
	// �ړ����x�A���͎��Ԑݒ�֐�.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ) override;
	// ��Ⴢ̐ݒ�.
	virtual void SetParalysisTime( const std::function<void(float&)>& ) override;

	// �A�j���[�V�����̂���(��������)�𒲐�.
	bool AnimationDraggingAdj( const int& animAdjNo );

	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// �G�t�F�N�g�̐ݒ�.
	bool EffectSetting();
	// �A�j���[�V�����t���[���̐ݒ�.
	virtual bool SetAnimFrameList() override;

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
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// �U���p�̓����蔻��.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;				// �G�t�F�N�g.
	std::vector<std::shared_ptr<CEffectTimer>>		m_pEffectTimers;		// ���ʎ��Ԍv���N���X.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;				// Widget�N���X.

	int								m_AttackComboCount;			// �U���R���{�J�E���g.
	std::queue<player::SAttackData>	m_AttackDataQueue;			// �U���f�[�^�̃L���[.
	D3DXVECTOR3						m_AttackPosition;			// �U���p�����蔻����W.

	D3DXVECTOR3		m_GirlPosition;		// ���̎q�̍��W.
	D3DXVECTOR3		m_AttackVector;		// �U�����̃x�N�g��.
	D3DXVECTOR3		m_AvoidVector;		// ����x�N�g��.
	D3DXVECTOR3		m_HitVector;		// �Փˎ��̃x�N�g��.
	D3DXVECTOR3		m_TargetVector;		// �ړI�̃x�N�g��.

	player::SAnimDraggingAdjParam		m_AnimAdjDrggingParam;
	player::SAttackAdjParam				m_AttackAdjParam;
	player::SPlayerParam	m_Parameter;			// �p�����[�^�[.
	float			m_LifePoint;			// �̗�.
	float			m_SpecialAbility;		// ����\��.
	unsigned char	m_StatusFlag;			// ���݂̏�Ԃ̃t���O.

	float			m_AttackRangeLenght;		// �U���͈͂̃^�[�Q�b�g�Ƃ̋���(��r�p).
	float			m_SpecialAbilityValue;		// ����\�͉񕜗�.
	float			m_ItemSpecialAbilityValue;	// �A�C�e������\�͉񕜒l.
	float			m_AttackPower;				// �U����.
	float			m_MoveSpeed;				// �ړ����x.
	float			m_MoveSpeedMulValue;		// �ړ����x�Ɋ|�����킹��l.

	D3DXVECTOR3		m_HitStopCameraPosition;	// �q�b�g�X�g�b�v�p�̃J�����̍��W.
	D3DXVECTOR3		m_CameraNextPosition;		// �J�����̍��W.
	D3DXVECTOR3		m_CameraPosition;			// �J�����̍��W.
	D3DXVECTOR3		m_CameraLookPosition;		// �J�����̎��_���W.
	float			m_CameraCount;				// �J�����̃J�E���g.
	float			m_CameraReturnCount;		// �J�����̖߂�J�E���g.
	float			m_CameraLerp;				// �J�����ړ��̕�Ԓl.
	int				m_NowSPCameraStete;			// ����\�͂̃J�������.
	int				m_ParalysisCameraShakeCount;
	
	bool			m_OneStepCmaeraSeting;		// �J�����̐ݒ�(��񂾂��g�p).
};

#endif	// #ifndef PLAYER_H.