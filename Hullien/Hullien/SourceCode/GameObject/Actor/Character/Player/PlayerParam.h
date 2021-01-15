#ifndef PLAYER_PARAM_H
#define PLAYER_PARAM_H

#include "..\Character.h"

namespace player
{
// プレイヤーパラメータ.
struct stPlayerParam : public CCharacter::SParameter
{
	float		SpecialAbilityMax;	// 特殊能力最大値.
	float		SpecialAbilityValue;// 特殊能力回復値.
	int			AttackComboMax;		// 攻撃の最大数.
	int			AttackQueueMax;		// 攻撃キューの最大数.
	float		AvoidMoveDistance;	// 回避の移動距離.
	float		AvoidMoveSpeed;		// 回避用の移動速度.
	float		CameraMoveSpeed;	// カメラの移動速度.
	float		CameraDistance;		// カメラの距離.
	float		CameraHeight;		// カメラの高さ.
	float		CameraLookHeight;	// カメラの注視点の高さ.
	float		CameraLerpValue;	// カメラ移動の補完値.
	D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
	float		SphereAdjRadius;	// スフィアの調整半径.

	float		HitKnocBackMoveSpeed;	// ノックバック移動速度.
	float		ToleranceRadian;		// 回転の許容範囲.
	float		RotationSpeed;			// 回転速度.

	float		MoveSpeedMulAddValue;	// 掛け合わせる移動量の加算値.
	float		MoveSpeedMulMaxValue;	// 掛け合わせる移動量の最大値.

	float		AttackSearcLenght;		// 攻撃の索敵距離.
	float		AttackFanRadian;		// 扇の攻撃範囲.
	float		EnemyKnockBackPower;	// 敵のノックバック力.

	stPlayerParam()
		: SpecialAbilityMax			( 0.0f )
		, AttackComboMax			( 0 )
		, AttackQueueMax			( 0 )
		, AvoidMoveDistance			( 0.0f )
		, AvoidMoveSpeed			( 0.0f )
		, CameraMoveSpeed			( 0.01f )
		, CameraDistance			( 7.0f )
		, CameraHeight				( 4.0f )
		, CameraLookHeight			( 0.0 )
		, CameraLerpValue			( 1.0f )
		, SphereAdjPos				( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius			( 0.0f )
		, HitKnocBackMoveSpeed		( 0.3f )
		, ToleranceRadian			( static_cast<float>(D3DXToRadian(7.0)) )
		, RotationSpeed				( 0.2f )
		, MoveSpeedMulAddValue		( 0.035f )
		, MoveSpeedMulMaxValue		( 1.0f )
		, AttackSearcLenght			( 20.0f )
		, AttackFanRadian			( static_cast<float>(D3DXToRadian(180.0*0.5)) )
		, EnemyKnockBackPower		( 0.5f )
	{}
} typedef SPlayerParam;

// アニメーション番号.
enum enAnimNo : char
{
	EAnimNo_None = -1,

	EAnimNo_Wait,		// 待機.
	EAnimNo_Walk,		// 走り.
	EAnimNo_Attack1,	// 攻撃1.
	EAnimNo_Attack2,	// 攻撃2.
	EAnimNo_Attack3,	// 攻撃3.
	EAnimNo_Avoid,		// 回避.
	EAnimNo_SP,			// 特殊能力.
	EAnimNo_Damage,		// ヒット時.
	EAnimNo_Dead,		// 死亡.

	EAnimNo_Max,		// 最大.

	EAnimNo_Begin	= EAnimNo_Wait,	// 初め.
	EAnimNo_End		= EAnimNo_Max,	// 終了.

} typedef EAnimNo;

// 引きずり調整リスト.
enum enDraggingAdjList : char
{
	EDraggingAdjList_None = -1,

	EDraggingAdjList_Attack1,	// 攻撃1.
	EDraggingAdjList_Attack2,	// 攻撃2.
	EDraggingAdjList_Attack3,	// 攻撃3.
	EDraggingAdjList_Dead,		// 死亡.

	EDraggingAdjList_Max,
} typedef EDraggingAdjList;

// 攻撃番号,
enum enAttackNo : unsigned char
{
	EAttackNo_None,

	EAttackNo_One,		// 一回目.
	EAttackNo_Two,		// 二回目.
	EAttackNo_Three,	// 三回目.

	EAttackNo_Max = EAttackNo_Three,
} typedef EAttackNo;

// 効果時間計測番号.
enum enEffectTimerNo : unsigned char
{
	EEffectTimerNo_None,

	EEffectTimerNo_SPRecovery = 0,	// 特殊能力回復.
	EEffectTimerNo_Attack,			// 攻撃力.
	EEffectTimerNo_MoveSpeedUp,		// 移動速度.
	EEffectTimerNo_Paralysis,		// 麻痺.

	EEffectTimerNo_Max,
} typedef EEffectTimerNo;

// 使用エフェクト番号.
enum enEffectNo : unsigned char
{
	EEffectNo_AttackOne,		// 攻撃1.
	EEffectNo_AttackTwo,		// 攻撃2.
	EEffectNo_AttackThree,		// 攻撃3.
	EEffectNo_SP,				// 特殊能力.
	EEffectNo_Avoidance,		// 回避.
	EEffectNo_Paralysis,		// 麻痺.
	
	EEffectNo_Max,
} typedef EEffectNo;

// 特殊能力のカメラ状態.
enum enSPCameraState : unsigned char
{
	ESPCameraState_None,

	ESPCameraState_TargetRotation,	// 目的の座標へ回転.
	ESPCameraState_PlayerBack,		// カメラをプレイヤーの後ろに移動.
	ESPCameraState_CameraShake,		// カメラを揺らす.
	ESPCameraState_CameraReturn,	// カメラを元の位置に戻す.
	
	ESPCameraState_Max,

	ESPCameraState_Start	= ESPCameraState_TargetRotation,	// 始まり.
	ESPCameraState_End		= ESPCameraState_CameraReturn,		// 終了.

} typedef ESPCameraState;

// 現在の状態のフラグ(ビットフラグ用).
enum enStatusFlag : unsigned char
{
	EStatusFlag_None,

	EStatusFlag_DuringAvoid		= 1 << 0,	// 回避中か.
	EStatusFlag_EndSPCameraMove	= 1 << 1,	// 特殊能力のカメラの移動が終了したか.
	EStatusFlag_AttackRange		= 1 << 2,	// 攻撃範囲に敵がいるか.
	EStatusFlag_UsableSP		= 1 << 3,	// 特殊能力が使えるか.
	EStatusFlag_KnockBack		= 1 << 4,	// ノックバック中か.
	EStatusFlag_Dead			= 1 << 5,	// 死亡中か.
	EStatusFlag_AttackSE		= 1 << 6,	// 攻撃SEを鳴らすか.

	EStatusFlag_Max,

} typedef EStatusFlag;

// 攻撃用データ.
struct stAttackData
{
	EAnimNo		AnimNo;				// アニメーション番号.
	double		Frame;				// 経過フレーム.
	double		EnabledEndFrame;	// 有効終了フレーム.
	double		EndFrame;			// 終了フレーム.
	double		AttackCollEndFrame;	// 攻撃当たり判定の終了フレーム.

	stAttackData()
		: AnimNo			( EAnimNo_None )
		, Frame				( 0.0 )
		, EnabledEndFrame	( 1.0 )
		, EndFrame			( 2.0 )
		, AttackCollEndFrame( 0.0 )
	{}
} typedef SAttackData;

// 攻撃調整用構造体.
struct stAttackAdjParam
{
	double	EnabledEndFrame[EAttackNo_Max];		// 攻撃有効終了フレーム.
	double	CollEnabledEndFrame[EAttackNo_Max];	// 攻撃当たり判定有効終了フレーム.
	float	CollisionRadius[EAttackNo_Max];		// 攻撃当たり判定半径,
	float	CollisionDistance;					// 攻撃当たり判定の距離.

	D3DXVECTOR3	CollInvalidPosition;			// 攻撃してない時の座標.

	stAttackAdjParam()
		: EnabledEndFrame		()
		, CollEnabledEndFrame	()
		, CollisionRadius		()
		, CollisionDistance		( 0.0f )
		, CollInvalidPosition	( 0.0f, 0.0f, 0.0f )
	{
		for( int i = 0; i < EAttackNo_Max; i++ ){
			EnabledEndFrame[i] = CollEnabledEndFrame[i] = CollisionRadius[i] = 0;
		}
	}
} typedef SAttackAdjParam;

// アニメーションの調整フレーム.
struct stAnimationAdjFrameList
{
	double Frame[EAnimNo_Max];	// 調整フレーム.

} typedef SAnimationAdjFrameList;

// アニメーションの引きずり修正用.
// 開始フレームから終了フレーム間、
// プレイヤーのベクトルと移動速度を掛け合わせて、
//	アニメーションの引きずりを調整する.
struct stAnimDraggingAdjParam
{
	double	StartFrame[EDraggingAdjList_Max];	// 開始フレーム.
	double	EndFrame[EDraggingAdjList_Max];		// 終了フレーム.
	float	MoveSpeed[EDraggingAdjList_Max];	// 調整用の移動速度.

	stAnimDraggingAdjParam()
		: StartFrame	()
		, EndFrame		()
		, MoveSpeed		()
	{
		for( int i = 0; i < EDraggingAdjList_Max; i++ ){
			StartFrame[i] = EndFrame[i] = MoveSpeed[i] = 0;
		}
	}

} typedef SAnimDraggingAdjParam;

};	// namespace PLAYER.

#endif	// #ifndef PLAYER_PARAM_H.