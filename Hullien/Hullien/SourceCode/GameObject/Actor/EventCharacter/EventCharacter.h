#ifndef EVENT_CHARACTER_H
#define EVENT_CHARACTER_H

#include "..\Actor.h"

#define IS_TEMP_MODEL_RENDER	// 仮モデル表示.

/***************************************
*	イベント用キャラクタクラス.
**/
class CEventCharacter : public CActor
{
protected:
	const float INIT_POSITION_ADJ_HEIGHT = 4.0f;	// 調整用座標の高さ.

public:
	CEventCharacter();
	virtual ~CEventCharacter();

	// 移動関数.
	virtual void Move() = 0;
	// 目的地の取得関数.
	D3DXVECTOR3 SetDestination() const { return m_vDestination; }
	// 目的地の設定関数.
	void SetDestination(const D3DXVECTOR3& vPos) { m_vDestination = vPos; }

	// 座標別位置設定関数.
	void SetPositionX(const float& x);
	void SetPositionY(const float& y);
	void SetPositionZ(const float& z);

	// 回転値取得関数.
	D3DXVECTOR3 GetRotation() const;
	// 回転値設定関数.
	void SetRotation(const D3DXVECTOR3& vRot);
	void SetRotationX(const float& x);
	void SetRotationY(const float& y);
	void SetRotationZ(const float& z);

	// 大きさ取得関数.
	D3DXVECTOR3 GetScale() const;
	
protected:
	// メッシュの表示関数.
	void MeshRender();
	// モデルの取得関数.
	bool GetModel(const char* modelName);

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// スキンメッシュ.
#ifdef IS_TEMP_MODEL_RENDER
	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;	// 仮のモデルデータ.
#endif	// #ifdef IS_TEMP_MODEL_RENDER.

	D3DXVECTOR3 m_vDestination;		// 目的地.
	D3DXVECTOR3 m_MoveVector;			// 移動ベクトル.
	float m_MoveSpeed;							// 移動速度.
};

#endif	//#ifndef EVENT_CHARACTER_H.
