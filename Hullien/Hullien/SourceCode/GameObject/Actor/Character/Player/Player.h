#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include "PlayerParam.h"
#include <queue>

class CRotLookAtCenter;	// カメラクラス.
class CCharacterWidget;	// キャラクタUIクラス.
class CEffectManager;	// エフェクトクラス.
class CEffectTimer;		// アイテムの時間管理クラス.

class CPlayer : public CCharacter
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";

	// モデル名.
	const char*	MODEL_NAME			= "kaito_s";

	// エフェクト名.
	const char* ATTACK_ONE_EFFECT_NAME		= "slash_effect_1";	// 攻撃1.
	const char* ATTACK_TWO_EFFECT_NAME		= "slash_effect_2";	// 攻撃2.
	const char* ATTACK_THREE_EFFECT_NAME	= "slash_effect_3";	// 攻撃3.
	const char* SPECIAL_ABILITY_EFFECT_NAME	= "uvtest";			// 特殊能力.
	const char* AVOIDANCE_EFFECT_NAME		= "sandSmoke";		// 回避.
	const char* PARALYSIS_EFFECT_NAME		= "uvtest";			// 麻痺.

	// ボーン名.
	const char* RIGHT_FOOT_BONE_NAME	= "kaito_rifa_2_R_asi_4";		// 右足.
	const char* LEFT_FOOT_BONE_NAME		= "kaito_rifa_2_L_asi_4";		// 左足.
	const char* LITTLE_FINGER_BONE_NAME	= "kaito_rifa_2_L_yubi_5_1";	// 小指.
	const char* RING_FINGER_BONE_NAME	= "kaito_rifa_2_L_yubi_4_1";	// 薬指.

	const float TWO = 2.0f;
	const float CAMERA_COUNT_MAX			= 100.0f;	// カメラのカウント最大値.
	const float CAMERA_BACK_DIRECTION_X		= 20.0f;	// カメラの距離 : x;
	const float CAMERA_BACK_DIRECTION_Z		= 10.0f;	// カメラの距離 : z;
	const float CAMERA_BACK_HEIGHT			= 8.0f;		// カメラの高さ.
	const float CAMERA_BACK_LERP_VALUE		= 0.1f;		// カメラの補完値.
	const float CAMERA_FREQUENCY_LOOKPOS	= 15.0f;	// カメラ注視位置の周波数.
	const float CAMERA_AMPLITUDE_LOOKPOS	= 0.01f;	// カメラ注視位置の振幅.
	const float CAMERA_RETURN_COUNT_ADD		= 0.001f;	// カメラの戻るときのカウント加算値.
	const float CAMERA_RETURN_COUNT_MAX		= 0.1f;		// カメラの戻るときのカウント最大値.

	const float ATTACK_EFFECT_RENDER_HEIGHT	= 5.0f;				// 攻撃エフェクトの描画高さ.
	const float AVOID_EFFECT_RENDER_HEIGHT	= 10.0f;			// 回避エフェクトの描画高さ.

	const float PLAY_WARNING_SE_LIFE_ONE_THIRD	= 1.0f / 4.0f;	// 警告音を再生する体力の

	// 声の音量.
	const float VOICE_VOLUME = 1.5f;

public:
	CPlayer();
	virtual ~CPlayer();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override; 
	// エフェクト描画関数.
	virtual void EffectRender() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos( CActor& actor ) override;
	// スプライトの描画.
	virtual void SpriteRender() override;

	// 特殊能力を使っているか.
	bool IsSpecialAbility();
	// 死亡したか.
	inline bool IsDead(){ return m_AnimFrameList[player::EAnimNo_Dead].IsNowFrameOver(); }
	// カメラの方向.
	float GetCameraRadianX();

	// ベクトルの取得.
	virtual void SetVector( const D3DXVECTOR3& vec ) override { m_HitVector = vec; }
	// アニメーションを再開する.
	virtual void ResumeAnimation() override;
	// カメラを揺らす.
	bool CameraShake();

	// パラメータの設定(editなどで使用).
	void SetParameter( const player::SPlayerParam& param, const bool& isEdit = false );

private:
	// 操作関数.
	void Controller();
	// カメラ操作.
	void CameraController();
	// 攻撃操作関数.
	void AttackController();
	// 特殊能力操作関数.
	void SPController();
	// 回避操作関数.
	void AvoidController();

	// 移動関数(アニメーションとのずれの調整).
	virtual void Move() override;
	// 攻撃時の移動.
	void AttackMove();
	// 回避動作関数.
	void AvoidMove();
	// ノックバック動作関数.
	void KnockBack();
	// 死亡動作関数.
	void Dead();
	// カメラの更新.
	void CameraUpdate();

	// 攻撃の当たり判定.
	void AttackCollision( CActor* pActor );
	// 攻撃範囲との判定.
	void AttackRangeDecision( CActor* pActor );
	// 特殊能力時のカメラ動作.
	void SPCameraUpdate();

	// ヒットストップの更新.
	void HitStopUpdate();

	// 特殊能力回復更新関数.
	void SpecialAbilityUpdate();
	// 攻撃力UP更新関数.
	void AttackUpUpdate();
	// 移動速度UP更新関数.
	void MoveSpeedUpUpdate();
	// 麻痺中の更新関数.
	void ParalysisUpdate();

	// 攻撃アニメーション.
	void AttackAnimation();
	// 攻撃の追加ができたか.
	bool IsPushAttack();

	// ライフ計算関数.
	virtual void LifeCalculation( const std::function<void(float&,bool&)>& ) override;
	// 特殊能力回復時間、効力時間設定関数.
	virtual void SetSPEffectTime( const std::function<void(float&,float&)>& ) override;
	// 攻撃力、効力時間設定関数.
	virtual void SetAttackEffectTime( const std::function<void(float&,float&)>& ) override;
	// 移動速度、効力時間設定関数.
	virtual void SetMoveSpeedEffectTime( const std::function<void(float&,float&)>& ) override;
	// 麻痺の設定.
	virtual void SetParalysisTime( const std::function<void(float&)>& ) override;

	// アニメーションのずれ(引きずり)を調整.
	bool AnimationDraggingAdj( const int& animAdjNo );

	// 当たり判定の設定.
	bool ColliderSetting();
	// エフェクトの設定.
	bool EffectSetting();
	// アニメーションフレームの設定.
	virtual bool SetAnimFrameList() override;

	// エディット用の描画関数.
	void EditRender();

	// サウンドの設定.
	bool SoundSetting();
	// 音量の設定.
	void VolumeSetting(const char* soung, float volume);
	// ウィジェット設定.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;				// カメラクラス.
	std::shared_ptr<CCamera>						m_pSPCamera;			// 特殊能力カメラクラス.
	std::shared_ptr<CCollisionManager>				m_pAttackCollManager;	// 攻撃用の当たり判定.
	std::vector<std::shared_ptr<CEffectManager>>	m_pEffects;				// エフェクト.
	std::vector<std::shared_ptr<CEffectTimer>>		m_pEffectTimers;		// 効果時間計測クラス.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;				// Widgetクラス.

	int								m_AttackComboCount;			// 攻撃コンボカウント.
	std::queue<player::SAttackData>	m_AttackDataQueue;			// 攻撃データのキュー.
	D3DXVECTOR3						m_AttackPosition;			// 攻撃用当たり判定座標.

	D3DXVECTOR3		m_GirlPosition;		// 女の子の座標.
	D3DXVECTOR3		m_AttackVector;		// 攻撃時のベクトル.
	D3DXVECTOR3		m_AvoidVector;		// 回避ベクトル.
	D3DXVECTOR3		m_HitVector;		// 衝突時のベクトル.
	D3DXVECTOR3		m_TargetVector;		// 目的のベクトル.

	player::SAnimDraggingAdjParam		m_AnimAdjDrggingParam;
	player::SAttackAdjParam				m_AttackAdjParam;
	player::SPlayerParam	m_Parameter;			// パラメーター.
	float			m_LifePoint;			// 体力.
	float			m_SpecialAbility;		// 特殊能力.
	unsigned char	m_StatusFlag;			// 現在の状態のフラグ.

	float			m_AttackRangeLenght;		// 攻撃範囲のターゲットとの距離(比較用).
	float			m_SpecialAbilityValue;		// 特殊能力回復力.
	float			m_ItemSpecialAbilityValue;	// アイテム特殊能力回復値.
	float			m_AttackPower;				// 攻撃力.
	float			m_MoveSpeed;				// 移動速度.
	float			m_MoveSpeedMulValue;		// 移動速度に掛け合わせる値.

	D3DXVECTOR3		m_HitStopCameraPosition;	// ヒットストップ用のカメラの座標.
	D3DXVECTOR3		m_CameraNextPosition;		// カメラの座標.
	D3DXVECTOR3		m_CameraPosition;			// カメラの座標.
	D3DXVECTOR3		m_CameraLookPosition;		// カメラの視点座標.
	float			m_CameraCount;				// カメラのカウント.
	float			m_CameraReturnCount;		// カメラの戻るカウント.
	float			m_CameraLerp;				// カメラ移動の補間値.
	int				m_NowSPCameraStete;			// 特殊能力のカメラ状態.
	int				m_ParalysisCameraShakeCount;
	
	bool			m_OneStepCmaeraSeting;		// カメラの設定(一回だけ使用).
};

#endif	// #ifndef PLAYER_H.