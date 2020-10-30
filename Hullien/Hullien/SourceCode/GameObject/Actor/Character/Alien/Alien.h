#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"
#include "..\..\Item\ItemList.h"

#include <random>

class CArm;
class CEffectManager;

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// ��]�̋��e�͈�.
	const float BARRIER_HIT_MOVE_SPEED = -5.0f;	// �o���A�ƏՓˎ��̈ړ����x.
	const float ADD_SCALE_VALUE = 0.03f;
	const float SCALE_MAX		= 1.0f;
	const float DOWN_SPEED		= 0.02f;
	const int KNOCK_BACK_TIME	= 5;
	const float DEATH_COUNT_ADD_VALUE = 0.005f;
	const float DEATH_SCALE_SUB_VALUE = 0.005f;
	const float DEATH_SCALE_PI	= 6.0f*static_cast<float>(D3DX_PI);
	const float RISING_MOTHER_SHIP_SCALE_SUB_VALUE = 0.01f;
protected:
	const float MODEL_ALPHA_MAX			= 1.0f;				// ���f���A���t�@�̍ő�l.
	const char* HIT_EEFECT_NAME			= "hiteffekt";		// �q�b�g�G�t�F�N�g.
	const char* SPAWN_EFFECT_NAME		= "b_attackeffkt";	// �X�|�[���G�t�F�N�g.
	const char* DEAD_EFFECT_NAME		= "b_attackeffkt";	// ���S�G�t�F�N�g.
	const char* ATTACK_EFFECT_NAME		= "b_attackeffkt";	// �U���G�t�F�N�g.

public:
	// �F���l�p�����[�^.
	struct stAlienParam : public stParameter
	{
		float		RotationalSpeed;	// ��]���x.

		float		ModelAlphaAddValue;	// ���f���̃A���t�@�l�̉��Z����l.
		float		ModelAlphaSubValue;	// ���f���̃A���t�@�l�̌��Z����l.

		int			WaitTime;			// �ҋ@����.

		float		PlayerAimLenght;	// �v���C���[��_���͈�.

		float		ResearchLenght;		// �Č�������ۂ̋���.

		float		AttackLenght;		// �U�����鋗��.
		float		AttackRotInitPower;	// �U�����̏�����]��.
		float		AttackRotPower;		// �U�����̉�]��.
		float		AttackRotAddValue;	// �U�����̉�]���Z�l.
		float		AttackMoveSpeed;	// �U���ړ����x.
		float		AttackMoveRange;	// �U���ړ��͈�.

		float		LaserMoveSpeed;		// ���[�U�[�̈ړ����x.
		float		ParalysisTime;		// ��Ⴢ̎���.

		float		AttackRangeSpritePosY;	// �U�����͈͕̔\���̒����p.
		float		AttackRangeSpriteScale;	// �U�����͈͕̔\���̉摜�T�C�Y.
		float		AttackRangeAddValue;	// �U�����͈͕̔\���̃A���t�@���������l.
		float		AttackRangeSubValue;	// �U�����͈͕̔\���̃A���t�@���������l.

		float		ControlPointOneLenght;	// �x�W�F�Ȑ��̈�ڑ�����W�̋���.
		float		ControlPointOneLenghtY;	// �x�W�F�Ȑ��̈�ڑ�����W��y���W�̋���.
		float		ControlPointTwoLenght;	// �x�W�F�Ȑ��̓�ڑ�����W�̋���.
		float		ControlPointTwoLenghtY;	// �x�W�F�Ȑ��̓�ڑ�����W��y���W�̋���.

		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		SphereAdjRadius;	// �X�t�B�A�̒������a.

		stAlienParam()
			: RotationalSpeed		( 0.0f )

			, ModelAlphaAddValue	( 0.0f )
			, ModelAlphaSubValue	( 0.0f )

			, WaitTime				( 0 )

			, PlayerAimLenght		( 0.0f )
			, ResearchLenght		( 0.0f )

			, AttackLenght			( 0.0f )
			, AttackRotInitPower	( 0.0f )
			, AttackRotPower		( 0.0f )
			, AttackRotAddValue		( 0.0f )
			, AttackMoveSpeed		( 0.0f )
			, AttackMoveRange		( 0.0f )

			, LaserMoveSpeed		( 0.0f )
			, ParalysisTime			( 0.0f )

			, AttackRangeSpritePosY		( 0.0f )
			, AttackRangeSpriteScale	( 1.0f )
			, AttackRangeSubValue		( 0.0f )

			, ControlPointOneLenght		( 0.0f )
			, ControlPointOneLenghtY	( 0.0f )
			, ControlPointTwoLenght		( 0.0f )
			, ControlPointTwoLenghtY	( 0.0f )

			, SphereAdjPos			( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius		( 0.0f )
		{}
	} typedef SAlienParam;

protected:
	// �F���l���.
	enum class enAlienState
	{
		None,

		Spawn,				// �X�|�[��.
		Move,				// �ړ�.
		Abduct,				// ����.
		KnockBack,			// �m�b�N�o�b�N.
		Fright,				// ����.
		Death,				// ���S.
		Escape,				// ������.
		RisingMotherShip,	// ��ɏオ��(�}�U�[�V�b�v��).

		Max,

	} typedef EAlienState;

	enum enAnimNo
	{
		EAnimNo_None = -1,

		EAnimNo_Move,
		EAnimNo_Arm,
		EAnimNo_Damage,
		EAnimNo_Dead,

		EAnimNo_Max,

		EAnimNo_Begin	= EAnimNo_Move,
		EAnimNo_End		= EAnimNo_Max,
	} typedef EAnimNo;

	// �ړ����.
	enum class enMoveState
	{
		None,

		Rotation,
		Move,
		Attack,
		Wait,

		Max,
	} typedef EMoveState;

	// �G�t�F�N�g�ԍ�.
	enum enEffectNo
	{
		EEffectNo_None = -1,

		EEffectNo_Hit,			// �q�b�g.
		EEffectNo_Spawn,		// �X�|�[��.
		EEffectNo_Dead,			// ���S.
		EEffectNo_Attack,		// �U��.

		EEffectNo_Max,
	} typedef EEffectNo;

public:
	CAlien();
	virtual ~CAlien();

	// �G�t�F�N�g�̕`��.
	virtual void EffectRender() override;

	// ������W�̐ݒ�.
	virtual void SetTargetPos( CActor& actor ) override;
	// �x�N�g���̎擾.
	virtual void SetVector( const D3DXVECTOR3& vec ) override;
	// �X�|�[��.
	virtual bool Spawn( const SAlienParam& param, const D3DXVECTOR3& spawnPos ) = 0;

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
	// �A�ꋎ���Ă��邩�ǂ���.
	bool IsAbduct() const { return m_NowState == EAlienState::Abduct; }
	// �ǂ̃A�C�e���������Ă��邩�擾.
	EItemList GetAnyItem() const { return m_HasAnyItem; }
	// �ق��̉F���l���A�ꋎ���Ă��邩�ݒ�.
	void SetOtherAbduct( bool* pisAbduct ){ m_pIsAlienOtherAbduct = pisAbduct; }
	// �A�ꋎ��UFO�̍��W�̎擾.
	void SetAbductUFOPosition( D3DXVECTOR3* pos ){ m_pAbductUFOPosition = pos; }
	// �A�C�e���̐ݒ�.
	void SetItem( const EItemList& item ){ m_HasAnyItem = item; }
	// �������邩.
	bool IsExplosion() const { return m_IsExplosion; }
	// �������邩�ǂ���.
	bool IsDelete() const { return m_IsDelete; }
	// �}�U�[�V�b�v�ɏ����Ă��邩.
	bool IsRisingMotherShip() const { return m_NowState == EAlienState::RisingMotherShip; }

protected:
	// ���݂̏�Ԃ̍X�V�֐�.
	void CurrentStateUpdate();
	// ���̎q�̍��W��ݒ�.
	void SetGirlPos( CActor& actor );
	// ���W�ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

	// �ړ��x�N�g���ݒ�֐�.
	virtual void SetMoveVector( const D3DXVECTOR3& targetPos );
	// �ړI�̍��W�։�].
	void TargetRotation();
	// �ړ��֐�.
	virtual void VectorMove( const float& moveSpeed );
	// �ҋ@�֐�.
	virtual void WaitMove();

	// �X�|�[����.
	virtual void Spawning();
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct();
	// �m�b�N�o�b�N.
	virtual void KnockBack();
	// ����.
	virtual void Fright();
	// ���S.
	virtual void Death();
	// ������.
	virtual void Escape();
	// �}�U�[�V�b�v�ɏ����Ă���.
	virtual void RisingMotherShip();

	// ���̎q�Ƃ̓����蔻��.
	void GirlCollision( CActor* pActor );
	// �o���A�Ƃ̓����蔻��.
	void BarrierCollision( CActor* pActor );

	// �A�j���[�V�����R���g���[���[�̎擾.
	bool GetAnimationController();

	// �A�j���[�V�����t���[���̐ݒ�.
	virtual bool SetAnimFrameList() override;
	// �G�t�F�N�g�̐ݒ�.
	virtual bool EffectSetting();

protected:
	std::unique_ptr<CArm>		m_pArm;			// �A�[���N���X.
	LPD3DXANIMATIONCONTROLLER	m_pAC;			// �A�j���[�V�����R���g���[���[.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;
	D3DXVECTOR3		m_TargetPosition;			// ���̎q�̍��W.
	D3DXVECTOR3		m_TargetRotation;			// �ڕW�̉�]���.
	D3DXVECTOR3		m_KnockBackVector;
	D3DXVECTOR3		m_BeforeMoveingPosition;	// �ړ��O�̍��W.
	D3DXVECTOR3*	m_pAbductUFOPosition;		// UFO�̍��W.
	SAlienParam		m_Parameter;				// �p�����[�^.
	EAlienState		m_NowState;					// ���݂̏��.
	EMoveState		m_NowMoveState;				// ���݂̈ړ����.
	EItemList		m_HasAnyItem;				// �ǂ̃A�C�e���������Ă��邩.
	float			m_LifePoint;				// �̗�.
	float			m_MoveSpeed;				// �ړ����x.
	float			m_DeathScale;				// ���S���̑傫��.
	float			m_DeathCount;				// ���S�J�E���g.
	int				m_KnockBackCount;			// �m�b�N�o�b�N�J�E���g.
	int				m_WaitCount;				// �ҋ@�J�E���g.
	bool*			m_pIsAlienOtherAbduct;		// ���̉F���l���A�ꋎ���Ă��邩�ǂ���.
	bool			m_IsBarrierHit;				// �o���A�ɓ������Ă��邩.
	bool			m_IsExplosion;				// �������邩.
	bool			m_IsDelete;					// �������邩�ǂ���.

	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;
};

#endif	// #ifndef ALIEN_H.