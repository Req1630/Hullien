#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include "PlayerParam.h"
#include <queue>

class CRotLookAtCenter;
class CCharacterWidget;
class CEffectManager;
class CEffectTimer;

#define INTERMEDIATE_ANNOUCEMENT_ATTACK // ���Ԕ��\�p�̍U��.

class CPlayer : public CCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME			= "Sayaka_s";	// ���f����.
	const char* MODEL_TEMP_NAME		= "kaito";		// �����f����.

	// �g�p�U���G�t�F�N�g��.
	const char* ATTACK_ONE_EFFECT_NAME		= "barrier";
	const char* ATTACK_TWO_EFFECT_NAME		= "uvtest";
	const char* ATTACK_THREE_EFFECT_NAME	= "uvtest";

	// �A�j���[�V�����ԍ�.
	enum class enAnimNo
	{
		None = -1,

		Dead = 0,
		Wait,
		Walk,
		Wait1,
		Happy,

		Max = Happy,
	} typedef EAnimNo;

	// �U���ԍ�,
	enum enAttackNo
	{
		EAttackNo_None,

		EAttackNo_One,
		EAttackNo_Two, 
		EAttackNo_Three,

		EAttackNo_Max = EAttackNo_Three,
	} typedef EAttackNo;

	// ���ʎ��Ԍv���ԍ�.
	enum enEffectTimerNo
	{
		EEffectTimerNo_None,

		EEffectTimerNo_SPRecovery = 0,	// ����\�͉�.
		EEffectTimerNo_Attack,			// �U����.
		EEffectTimerNo_MoveSpeedUp,		// �ړ����x.
		EEffectTimerNo_Paralysis,		// ���.

		EEffectTimerNo_Max,
	} typedef EEffectTimerNo;

	// �U���p�f�[�^.
	struct stAttackData
	{
		EAnimNo		AnimNo;				// �A�j���[�V�����ԍ�.
		double		Frame;				// �o�߃t���[��.
		double		EnabledEndFrame;	// �L���I���t���[��.
		double		EndFrame;			// �I���t���[��.

		stAttackData()
			: AnimNo			( EAnimNo::None )
			, Frame				( 0.0 )
			, EnabledEndFrame	( 1.0 )
			, EndFrame			( 2.0 )
		{}
	} typedef SAttackData;

public:
	CPlayer();
	virtual ~CPlayer();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override; 
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

	// �G�t�F�N�g�`��֐�.
	void EffectRender();

	// �U���̓����蔻��.
	void AttackCollision( CActor* pActor );

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
	void SetAnimation( const EAnimNo& animNo );

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
	// �f�o�b�O�p�̕`��.
	void DebugRender();

	// �E�B�W�F�b�g�ݒ�.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;		// �J�����N���X.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;		// Widget�N���X.
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// �U���p�̓����蔻��.
	D3DXVECTOR3	m_OldPosition;		// �O��̍��W.
	EAnimNo	m_NowAnimNo;			// ���̃A�j���[�V�����ԍ�.
	EAnimNo	m_OldAnimNo;			// �O�̃A�j���[�V�����ԍ�.
	int						m_AttackComboCount;			// �U���R���{�J�E���g.
	std::vector<double>		m_AttackEnabledFrameList;	// �U���L���t���[���̃��X�g.
	std::queue<SAttackData>	m_AttackDataQueue;			// �U���f�[�^�̃L���[.
	D3DXVECTOR3				m_AttackPosition;			// �U���p�����蔻����W.
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;	// �G�t�F�N�g.
	bool			m_IsDuringAvoid;	// ��𒆂��ǂ���.
	D3DXVECTOR3		m_AvoidVector;		// ����x�N�g��.

	SPlayerParam	m_Parameter;			// �p�����[�^�[.
	float			m_LifePoint;			// �̗�.
	float			m_SpecialAbility;		// ����\��.
	bool			m_HasUsableSP;			// ����\�͂��g���邩.

	float			m_SpecialAbilityValue;		// ����\�͉񕜗�.
	float			m_ItemSpecialAbilityValue;	// �A�C�e������\�͉񕜒l.
	float			m_AttackPower;				// �U����.
	float			m_MoveSpeed;				// �ړ����x.

	std::vector<std::shared_ptr<CEffectTimer>>	m_pEffectTimers;	// ���ʎ��Ԍv���N���X.
	
	bool				m_IsAttackSE;				//�U��SE��炷��.
};

#endif	// #ifndef PLAYER_H.