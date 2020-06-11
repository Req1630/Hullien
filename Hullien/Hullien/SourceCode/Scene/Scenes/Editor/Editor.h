#ifndef EDITOR_H
#define EDITOR_H

#include "..\..\SceneBase\SceneBase.h"

class CControllerEdit;
class CSpawnEdit;

class CEditor : public CSceneBase
{
public:
	CEditor( CSceneManager* pSceneManager );
	virtual ~CEditor();

	// 読込関数.
	virtual bool Load() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	std::unique_ptr<CControllerEdit> m_pControllerEdit;
	std::unique_ptr<CSpawnEdit>	m_pSpawnEdit;
};

#endif	// #ifndef EDITOR_H.