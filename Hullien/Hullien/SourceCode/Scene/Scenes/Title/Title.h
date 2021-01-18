#ifndef TITLE_H
#define TITLE_H

#include "..\..\SceneBase\SceneBase.h"

#include "..\..\..\Common\Sprite\CSprite.h"

#include "..\..\..\Collider\Colliders\Capsule\CapsuleModel.h"
#include "..\..\..\Collider\Colliders\Capsule\CapsuleCollision.h"

class CRotLookAtCenter;
class CTitleWidget;
class CConfigWidget;
class CButtonExp;	// ボタン説明UI.

/**********************************
*	タイトルシーンクラス.
*/
class CTitle : public CSceneBase
{
private:
	const float ALPHA_SPEED = 0.01f;

public:
	CTitle(CSceneManager* pSceneManager);
	virtual ~CTitle();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// シーン切り替え関数.
	void ChangeScene();

private:
	std::unique_ptr<CTitleWidget>	m_pWidget;			// UIクラス.
	std::unique_ptr<CButtonExp>		m_pButtonExp;		// ボタン説明UIクラス.
	bool							m_IsChangeScene;	// シーンの切り替えが可能か.
	bool							m_IsGameStart;		// ゲームがスタートしたか.
	bool							m_IsDecision;		// 決定したか.
	bool							m_IsNowConfig;		// 設定中か.
};

#endif	// #ifndef TITLE_H.