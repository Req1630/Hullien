#ifndef GAME_H
#define GAME_H

#include "..\..\SceneBase\SceneBase.h"


class CLimitTime;		// 制限時間クラス.
class CMiniMap;			// ミニマップクラス.

class CGameActorManager;	// ゲームオブジェクト管理クラス.


/**********************************
*	ゲームシーンクラス.
*/
class CGame : public CSceneBase
{
public:
	CGame( CSceneManager* pSceneManager );
	virtual ~CGame();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:

	std::unique_ptr<CLimitTime>		m_pLimitTime;
	std::unique_ptr<CMiniMap>		m_pMiniMap;

	std::unique_ptr<CGameActorManager> m_GameObjManager;

};

#endif	// #ifndef GAME_H.