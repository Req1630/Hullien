#ifndef INVISIBLE_WALL_H
#define INVISIBLE_WALL_H

#include "..\GameObject.h"
#include "BoxStruct.h"

class CCollisionManager;	// 当たり判定クラス.
class CInvisibleWallShader;	// 見えない壁シェーダークラス.

/**************************************
*	見えない壁.
*/
class CInvisibleWall : public CGameObject
{
	const char* INVISIBLE_WALL_PARAM_FILE_NAME = "Data\\GameParam\\InvisibleWall.bin";
	const float HALF			= 0.5f;		// 半分.
	const float POSITION_HEIGHT	= 10.0f;	// 座標の高さ.
public:
	CInvisibleWall();
	virtual ~CInvisibleWall();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// ボックスの設定.
	void SetBoxWall( const SBoxWall& boxWall );
	// ボックスの取得.
	inline SBoxWall* GetBoxWall() { return &m_BoxWall; }
	// 座標設定関数.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

private:
	// パラメータの初期化.
	bool InitParam();
	// 当たり判定の初期化.
	bool InitCollision();

private:
	SBoxWall	m_BoxWall;	//　壁のパラメータ.
	std::unique_ptr<CInvisibleWallShader>	m_pInvisibleWallShader;
#ifdef _DEBUG
	std::unique_ptr<CCollisionManager>	m_pCollision;	// 当たり判定クラス(視覚用).
#endif	// #ifdef _DEBUG
	D3DXVECTOR3	m_PlayerPosition;
};

#endif	// #ifndef INVISIBLE_WALL_H.