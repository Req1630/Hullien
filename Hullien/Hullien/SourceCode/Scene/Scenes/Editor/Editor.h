#ifndef EDITOR_H
#define EDITOR_H

#include "..\..\SceneBase\SceneBase.h"

class CControllerEdit;
class CSpawnEdit;
class CAlienParamEdit;
class CExplosionEdit;
class CItemEdit;

class CEditor : public CSceneBase
{
	enum enEditScenes
	{
		None,

		SpawnEdit,
		AlienParam,
		Explosion,
		ItemEdit,

		Max

	} typedef EEditScenes;
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
	std::unique_ptr<CControllerEdit>	m_pControllerEdit;
	std::unique_ptr<CSpawnEdit>			m_pSpawnEdit;
	std::unique_ptr<CAlienParamEdit>	m_pAlienParamEdit;
	std::unique_ptr<CExplosionEdit>		m_pExplosionEdit;
	std::unique_ptr<CItemEdit>			m_pItemEdit;

	int m_NowEditScene;
};

#endif	// #ifndef EDITOR_H.