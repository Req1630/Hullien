#ifndef PLAYER_PARAM_H
#define PLAYER_PARAM_H

#include "..\Character.h"

// �v���C���[�p�����[�^.
struct stPlayerParam : public CCharacter::SParameter
{
	float		SpecialAbilityMax;	// ����\�͍ő�l.
	float		SpecialAbilityValue;// ����\�͉񕜒l.
	int			AttackComboMax;		// �U���̍ő吔.
	int			AttackQueueMax;		// �U���L���[�̍ő吔.
	float		AvoidMoveDistance;	// ����̈ړ�����.
	float		AvoidMoveSpeed;		// ���p�̈ړ����x.
	float		CameraMoveSpeed;	// �J�����̈ړ����x.
	float		CameraDistance;		// �J�����̋���.
	float		CameraHeight;		// �J�����̍���.
	float		CameraLookHeight;	// �J�����̒����_�̍���.
	float		CameraLerpValue;	// �J�����ړ��̕⊮�l.
	D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
	float		SphereAdjRadius;	// �X�t�B�A�̒������a.

	stPlayerParam()
		: SpecialAbilityMax	( 0.0f )
		, AttackComboMax	( 0 )
		, AttackQueueMax	( 0 )
		, AvoidMoveDistance	( 0.0f )
		, AvoidMoveSpeed	( 0.0f )
		, CameraMoveSpeed	( 0.01f )
		, CameraDistance	( 7.0f )
		, CameraHeight		( 4.0f )
		, CameraLookHeight	( 0.0 )
		, CameraLerpValue	( 1.0f )
		, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius	( 0.0f )
	{}
} typedef SPlayerParam;

namespace player
{
// �A�j���[�V�����ԍ�.
enum enAnimNo
{
	EAnimNo_None = -1,

	EAnimNo_Wait,		// �ҋ@.
	EAnimNo_Walk,		// ����.
	EAnimNo_Attack1,	// �U��1.
	EAnimNo_Attack2,	// �U��2.
	EAnimNo_Attack3,	// �U��3.

	EAnimNo_Max = EAnimNo_Attack3,
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

// �g�p�G�t�F�N�g�ԍ�.
enum enEffectNo
{
	enEffectNo_AttackOne,		// �U��1.
	enEffectNo_AttackTwo,		// �U��2.
	enEffectNo_AttackThree,		// �U��3.
	enEffectNo_SP,				// ����\��.
	enEffectNo_Avoidance,		// ���.

	enEffectNo_Max,
} typedef EEffectNo;

// �U���p�f�[�^.
struct stAttackData
{
	EAnimNo		AnimNo;				// �A�j���[�V�����ԍ�.
	double		Frame;				// �o�߃t���[��.
	double		EnabledEndFrame;	// �L���I���t���[��.
	double		EndFrame;			// �I���t���[��.

	stAttackData()
		: AnimNo			( EAnimNo_None )
		, Frame				( 0.0 )
		, EnabledEndFrame	( 1.0 )
		, EndFrame			( 2.0 )
	{}
} typedef SAttackData;

};	// namespace PLAYER.

#endif	// #ifndef PLAYER_PARAM_H.