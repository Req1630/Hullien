#ifndef UFO_LIGHT_H
#define UFO_LIGHT_H

#include "..\GameObject.h"

class CUFOLight : public CGameObject
{
	const char* MODEL_NAME = "light";	// モデル名.

	const float MODEL_ALPHA					= 0.15f;	// モデルアルファ値.
	const float SCALE_UP_HEIGHT_SPEED		= 0.2f;		// デフォルト高さ拡大速度.
	const float SCALE_UP_WEDTH_SPEED		= 0.02f;	// デフォルト幅拡大速度.
	const float DEFAULT_SCALE_HEIGHT_MAX	= 1.0f;		// 高さの最大サイズ.
	const float DEFAULT_SCALE_WEDTH_MAX		= 1.0f;		// 幅の最大サイズ,
	const float DEFAULT_SCALE_HEIGHT_MIN	= 0.0f;		// 高さの最小サイズ.
	const float DEFAULT_SCALE_WEDTH_MIN		= 0.0f;		// 幅の最小サイズ,

	// UFOライトの状態.
	enum class enUFOLightState
	{
		None,
		
		Discharge,		// 取り出す.
		CleanUP,		// 片づける.
		EndDischarge,	// 取り出し終わった.
		EndCleanUP,		// 片づけ終わった.

		Max,
	} typedef EUFOLightState;
public:
	CUFOLight();
	CUFOLight( const float& height, const float& wedth );
	virtual ~CUFOLight();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 光を放出する.
	void Discharge();
	// 光を片づける.
	void CleanUP();
	// 光を完全に放出する.
	void DischargePreparation();
	// 光を片づける.
	void CleanUPPreparation();

	// ライトの大きさを設定.
	void SetLightScale( const float& height, const float& wedth );

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// メッシュ.
	EUFOLightState					m_NowState;		// 現在の状態.
	float	m_ScaleHeightMax;
	float	m_ScaleWedthMax;
};

#endif	// #ifndef UFO_LIGHT_H.