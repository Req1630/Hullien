#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"
#include "AnimationParamInfo.h"

class CCharacter : public CActor
{
	inline static const float FOOT_COLLISION_SIZE	= 0.5f;	// ���̓����蔻��̃T�C�Y.
	inline static const float GROUND_COLLISION_SIZE	= 1.0f;	// �n�ʂ̓����蔻��̃T�C�Y.
protected:
	inline static const double DEFAULT_ANIM_SPEED		= 0.01;						// �f�t�H���g�A�j���[�V�������x.
	inline static const double DEFAULT_ANIM_HALF_SPEED	= DEFAULT_ANIM_SPEED*0.5;	// �f�t�H���g�A�j���[�V�������x�̔����̑��x.
	inline static const double STOP_ANIM_SPEED			= 0.0;						// ��~�A�j���[�V�������x.

	// ���̔ԍ�.
	enum enFootNumber : char
	{
		EFootNumber_None = -1,

		EFootNumber_Right,	// �E��.
		EFootNumber_Left,	// ����.

		EFootNumber_Max,
	} typedef EFootNumber;

public:
	// �L�����N�^�[�p�����[�^.
	struct stParameter
	{
		float	LifeMax;		// �̗�.
		float	AttackPower;	// �U����.
		float	MoveSpeed;		// �ړ����x.
		int		InvincibleTime;	// ���G����.
		stParameter()
			: LifeMax			( 1.0f )
			, AttackPower		( 0.5f )
			, MoveSpeed			( 0.5f )
			, InvincibleTime	( 0 )
		{}
	} typedef SParameter;

public:
	CCharacter();
	virtual ~CCharacter();

	// �ړ��֐�.
	virtual void Move() = 0;
	// �X�v���C�g�̕`��.
	virtual void SpriteRender(){};

	// �A�j���[�V�������~�߂�.
	virtual void StopAnimation() override
	{
		m_OldAnimSpeed = m_AnimSpeed;
		m_AnimSpeed = STOP_ANIM_SPEED; 
	}
	// �A�j���[�V�������ĊJ����.
	virtual void ResumeAnimation() override { m_AnimSpeed = m_OldAnimSpeed; }

protected:
	// ���b�V���̕\��.
	void MeshRender();
	// ���G���Ԃ��ǂ���.
	bool IsInvincibleTime( const int& invincibleTime );
	// ���f���̎擾.
	bool GetModel( const char* modelName );
	// �����蔻��̐ݒ�.
	bool ColliderSetting(){};
	// �p�����[�^�̐ݒ�.
	template<class inParam>
	bool ParameterSetting( const char* fileName, inParam& param )
	{
		// ���Ƀp�����[�^���ݒ�ς݂Ȃ�I��.
		if( m_HasFinishedParamSetting == true ) return true;

		// �ǂݍ��݂�����ɍs���ĂȂ���ΏI��.
		if( CFileManager::BinaryReading( fileName, param ) == false ) return false;

		m_HasFinishedParamSetting = true;
		return true;
	}
	// �ړI�̍��W�։�].
	bool TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian );
	// �A�j���[�V�����ݒ�.
	void SetAnimation( const int& animNo, LPD3DXANIMATIONCONTROLLER	pAc = nullptr );
	// �A�j���[�V�������u�����h���Đݒ�.
	void SetAnimationBlend( const int& animNo );

	// �A�j���[�V�����t���[���̐ݒ�.
	virtual bool SetAnimFrameList() = 0;

	// ����.
	void FootStep(const char* rightfoot, const char* leftfoot);
	// �����p�����蔻��̐ݒ�.
	bool FootStepCollisionSetting();

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// �X�L�����b�V��.
	anim::AAnimFrameList			m_AnimFrameList;	// �A�j���[�V�����t���[���̃��X�g.
	int		m_NowAnimNo;	// ���݂̃A�j���[�V�����ԍ�.
	int		m_OldAnimNo;	// �ߋ��̃A�j���[�V�����ԍ�.
	double	m_AnimSpeed;	// �A�j���[�V�������x.
	double	m_OldAnimSpeed;	// �A�j���[�V�������x.

	D3DXVECTOR3	m_MoveVector;				// �ړ��x�N�g��.
	int			m_InvincibleCount;			// ���G�J�E���g.
	bool		m_HasFinishedParamSetting;	// �p�����[�^�[�̐ݒ肪�I�������.

private:
	std::vector<std::shared_ptr<CCollisionManager>> m_pFootCollision;
	std::shared_ptr<CCollisionManager>				m_pGroundCollision;
	D3DXVECTOR3 m_vGroundPosition;	// �n�ʂ̈ʒu.
	D3DXVECTOR3 m_vFootPosition[2];	// ���̈ʒu.
};

#endif	// #ifndef CHARACTER_H.