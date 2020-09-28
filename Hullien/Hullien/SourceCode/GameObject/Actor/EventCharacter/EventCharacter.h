#ifndef EVENT_CHARACTER_H
#define EVENT_CHARACTER_H

#include "..\Actor.h"

#define IS_TEMP_MODEL_RENDER	// 仮モデル表示.

/***************************************
*	イベント用キャラクタクラス.
**/
class CEventCharacter : public CActor
{
	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
//	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.

public:
	// 任意で設定する情報.
	struct stOptionalState
	{
		D3DXVECTOR3		Destination;				// 目的地.
		float						RotationalSpeed;			// 回転速度.
		float						MoveSpeed;				// 移動速度.

	}typedef SOptionalState;

protected:
	const float INIT_POSITION_ADJ_HEIGHT = 4.0f;	// 調整用座標の高さ.

	// 移動状態.
	enum class enMoveState
	{
		None,

		Rotation,
		Move,

		Max,
	} typedef EMoveState;

	// キャラクタの情報.
	struct stCharacterParam : public SOptionalState
	{
		float						ModelAlphaAddValue;	// モデルのアルファ値の加算する値.
		float						ResearchLenght;			// 再検査する際の距離.
		D3DXVECTOR3		SphereAdjPos;				// スフィアの調整座標.
		float						SphereAdjRadius;			// スフィアの調整半径.

		stCharacterParam()
			: ModelAlphaAddValue(0.0f)
			, ResearchLenght(0.0f)
			, SphereAdjPos(0.0f, 0.0f, 0.0f)
			, SphereAdjRadius(0.0f)
		{}
	}typedef SCharacterParam;

public:
	CEventCharacter();
	virtual ~CEventCharacter();

	// 移動関数.
	virtual void Move();

	// 情報設定関数.
	void SetOptionalState(SOptionalState state);

	// 座標別位置設定関数.
	void SetPositionX(const float& vPos_x);
	void SetPositionY(const float& vPos_y);
	void SetPositionZ(const float& vPos_z);

	// 回転値取得関数.
	D3DXVECTOR3 GetRotation() const;
	// 回転値設定関数.
	void SetRotation(const D3DXVECTOR3& vRot);
	void SetRotationX(const float& vRot_x);
	void SetRotationY(const float& vRot_y);
	void SetRotationZ(const float& vRot_z);

	// 大きさ取得関数.
	D3DXVECTOR3 GetScale() const;
	// 大きさ設定関数.
	void SetScale(const D3DXVECTOR3& vScale);
	void SetScaleX(const float& vScale_x);
	void SetScaleY(const float& vScale_y);
	void SetScaleZ(const float& vScale_z);
	
protected:
	// メッシュの表示関数.
	void MeshRender();
	// モデルの取得関数.
	bool GetModel(const char* modelName);
	// 移動ベクトル設定関数.
	virtual void SetMoveVector(const D3DXVECTOR3& targetPos);
	// 目的の座標へ回転.
	void TargetRotation();
	// 移動関数.
	virtual void VectorMove(const float& moveSpeed);


protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// スキンメッシュ.
#ifdef IS_TEMP_MODEL_RENDER
	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;	// 仮のモデルデータ.
#endif	// #ifdef IS_TEMP_MODEL_RENDER.

	D3DXVECTOR3		m_MoveVector;					// 移動ベクトル.
	D3DXVECTOR3		m_TargetRotation;				// 目標の回転情報.
	D3DXVECTOR3		m_BeforeMoveingPosition;		// 移動前の座標.
	SCharacterParam	m_Parameter;						// パラメータ.
	EMoveState			m_NowMoveState;				// 現在の移動状態.
	float						m_ModelAlpha;						// モデルのアルファ値.
};

#endif	//#ifndef EVENT_CHARACTER_H.
