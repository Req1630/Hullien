/**
* @file STGManager.h.
* @brief STG全体を管理するクラス.
* @author 福田玲也.
*/
#ifndef STG_MANAGER_H
#define STG_MANAGER_H

#include <memory>

#include "..\..\..\Global.h"

namespace STG
{
	class CPlayer;			// プレイヤークラス.
	class CEnemyManager;	// 敵管理クラス.
}
class CSkyDome;	// 背景.
class CCamera;	// カメラ.

/*********************************
*	STGを管理するクラス.
**/
class CSTGManager
{
	const float STG_END_FRAME				= 3*FPS;	// STGの終了フレーム.
	const float SKY_DOME_SCROLL_SPEED		= -0.001f;
	const D3DXVECTOR3 CAMERA_POSITION		= { 0.0f, 150.0f, 10.0f };
	const D3DXVECTOR3 CAMERA_LOOK_POSITION	= { 0.0f, 0.0f, 0.0f };
public:
	CSTGManager();
	~CSTGManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();
	// シューティングが終了したか.
	bool IsSTGEnd();

private:
	std::shared_ptr<CCamera>			m_pCamera;
	std::unique_ptr<CSkyDome>			m_pSkyDome;			// 背景.
	std::shared_ptr<STG::CPlayer>		m_pPlayer;			// プレイヤークラス.
	std::unique_ptr<STG::CEnemyManager>	m_pEnemyManager;	// 敵管理クラス.
	float	m_STGEndFrame;	// STGの終了フレーム.
	float	m_STGNowFrame;	// STGの現在フレーム.
	float	m_SkyDomeRotX;	// 背景のスクロール回転.
	int		m_EndCount;		// 終了カウント.
};

#endif	// #ifndef STG_MANAGER_H.