/**
* @file Arm.h.
* @brief アームクラス.
* @author 福田玲也.
*/
#ifndef ARM_H
#define ARM_H

#include "..\GameObject.h"

/*************************************
*	アームクラス.
**/
class CArm : public CGameObject
{
public:
	static constexpr float GRAB_DISTANCE	= 5.0f;		// 掴むときの距離.
	static constexpr float GRAB_HEIGHT		= 5.0f;		// 掴むときの高さ.

private:
	inline static const char* MODEL_NAME = "armsandwich_s";

	static constexpr float APPEARANCE_ADD_VALUE	= 0.1f;		// 出現時の加算値.
	static constexpr float APPEARANCE_COUNT_MAX	= 1.0f;		// 出現時の最大数.
	static constexpr float CLEAN_UP_SUB_VALUE	= 0.015f;	// 片づけ時の減算値.
	static constexpr float CLEAN_UP_COUNT_MIN	= 0.0f;		// 片づけ時の最小数.

	static constexpr float SCALE_MAX			= 1.0f;		// モデルサイズの最大値.
	static constexpr float SCALE_MIN			= 0.0f;		// モデルサイズの最小値.
	static constexpr float SCALING_VALUE		= 0.5f;		// 拡縮値.
	static constexpr float SCALING_VALUE_MAX	= 1.0f;		// 拡縮の最大値.
	static constexpr float SCALING_ADD_VALUE	= (1.0f-SCALING_VALUE)*0.04f;	// 拡縮の加算値.

	enum class enArmState
	{
		None,

		Appearance,	// 出現.
		Grab,		// 掴む.
		CleanUp,	// 片づける.

		End,		// 終了.

		Start = None,

		Max,


	} typedef EArmState;
public:
	CArm();
	virtual ~CArm();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// Y軸の設定.
	inline void SetRotationY( const float& rotY ){ m_vRotation.y = rotY; }

	// 掴んでいるか.
	inline bool IsGrab(){ return m_NowArmState == EArmState::End; }
	// 片づけているか.
	inline bool IsCleanUp(){ return m_NowArmState == EArmState::Start; }
	// 掴んでいる座標の取得.
	D3DXVECTOR3 GetGrabPosition();

	// 出現する用意.
	void SetAppearancePreparation();
	// 片づける用意.
	void SetCleanUpPreparation();

	// 完全に取り出す
	void SetAppearance();
	// 完全に片づける.
	void SetCleanUp();

	// アニメーションを止める.
	void StopAnimation();
	// アニメーションを再開する.
	void ResumeAnimation();

	inline void SetCleanUpScale( const D3DXVECTOR3& scale ){ m_vScale = scale; }

private:
	// 出現.
	void Appearance();
	// 掴む.
	void Grab();
	// 片づける.
	void CleanUp();

private:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// スキンメッシュ.
	LPD3DXANIMATIONCONTROLLER		m_pAC;			// アニメーションコントローラー.
	D3DXVECTOR3						m_GrabPosition;	// 掴んでいる時の座標.
	EArmState						m_NowArmState;	// 現在のアームの状態.
	EArmState						m_OldArmState;	// 前回のアームの状態.
	float	m_AppearanceCount;	// 出現カウント.
	float	m_ScalingValue;		// 拡縮値.
	double	m_AnimSpeed;		// アニメーション速度.
	double	m_OldAnimSpeed;		// 前回のアニメーション速度.
	double	m_AnimEndFrame;		// アニメーションの終了フレーム.

};

#endif	// #ifndef ARM_H.