/**
* @file MotherShipUFO.h.
* @brief マザーシップUFOクラス.
* @author 福田玲也.
*/
#ifndef MOTHER_SHIP_UFO_H
#define MOTHER_SHIP_UFO_H

#include "..\GameObject.h"

class CActor;
class CCollisionManager;	// 当たり判定クラス.
class CUFOLight;

/**********************************************
*	マザーシップUFOクラス.
**/
class CMotherShipUFO : public CGameObject
{
public:
	// マザーシップのパラメータ.
	struct stMotherShipUFOParam
	{
		D3DXVECTOR3 Position;			// 座標.
		float		AddPosYPower;		// 持ち上げる力.
		D3DXVECTOR3 SphereAdjPos;		// スフィアの調整座標.
		float		CollisionRadius;	// 当たり判定の半径.
		D3DXVECTOR3 LightPosition;
		D3DXVECTOR2 LightSize;

		stMotherShipUFOParam()
			: Position			( 0.0f, 0.0f, 0.0f )
			, AddPosYPower		( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, CollisionRadius	( 1.0f )
			, LightPosition		( 0.0f, 0.0f, 0.0f )
			, LightSize			( 2.85f, 2.5f )
		{}

	} typedef SMotherShipUFOParam;

private:
	const char* MODEL_NAME = "ufo";	// モデル名.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Alien\\MotherShipUFOParam\\MotherShipUFOParam.bin";
	const float WND_OUT_SIZE = 700.0f;

public:
	CMotherShipUFO();
	virtual ~CMotherShipUFO();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定.
	void Collision( CActor* pActor );
	// パラメータの設定.
	void SetParameter( const SMotherShipUFOParam& param );
	// 描画フラグ設定関数.
	inline void SetDisp(const bool& disp) { m_IsDisp = disp; }
	// 宇宙人が帰還したか取得関数.
	inline bool IsReturnAlien() const { return m_IsReturnAlien; }

	// ライトを取り出す.
	void LightDischarge();
	// ライトをしまう.
	void LightCleanUP();
	// 光を完全に放出する.
	void DischargePreparation();
	// 光を片づける.
	void CleanUPPreparation();

	// 当たり判定(イベントで使用).
	D3DXVECTOR3 Collision( CActor* pActor, const bool& isEvent );
private:
	// モデルの取得.
	bool GetModel();
	// 当たり判定の設定.
	bool CollisionSetting();

private:
	std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;	// スタティックメッシュ.
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// 当たり判定クラス.
	std::unique_ptr<CUFOLight>			m_pUFOLight;	// UFOのライト.
	SMotherShipUFOParam					m_Param;		// パラメーター.
	bool								m_IsDisp;		// 描画しているか.
	bool								m_IsReturnAlien;// 宇宙人が帰還したか.
};

#endif	// #ifndef MOTHER_SHIP_UFO_H.