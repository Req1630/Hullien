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
		, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius	( 0.0f )
	{}
} typedef SPlayerParam;

#endif	// #ifndef PLAYER_PARAM_H.