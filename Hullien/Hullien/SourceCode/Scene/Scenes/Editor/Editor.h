#ifndef EDITOR_H
#define EDITOR_H

#include "..\..\SceneBase\SceneBase.h"

class CControllerEdit;
class CSpawnEdit;
class CAlienParamEdit;
class CExplosionEdit;
class CItemEdit;
class CPlayerEdit;
class CPeraRenderer;
class CEditCamera;
class CSkyDome;
class CGroundStage;

class CEditor : public CSceneBase
{
	enum enEditScenes
	{
		None,

		PlayerEdit,
		SpawnEdit,
		AlienParam,
		Explosion,
		ItemEdit,

		Max,

	} typedef EEditScenes;
public:
	CEditor( CSceneManager* pSceneManager );
	virtual ~CEditor();

	// �Ǎ��֐�.
	virtual bool Load() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// ���f���̕`��.
	void ModelRender();
	// UI�̕\��.
	void ImGuiRender();

private:
	std::unique_ptr<CControllerEdit>	m_pControllerEdit;
	std::unique_ptr<CSpawnEdit>			m_pSpawnEdit;
	std::unique_ptr<CAlienParamEdit>	m_pAlienParamEdit;
	std::unique_ptr<CExplosionEdit>		m_pExplosionEdit;
	std::unique_ptr<CItemEdit>			m_pItemEdit;
	std::unique_ptr<CPlayerEdit>		m_pPlayerEdit;
	std::shared_ptr<CEditCamera>		m_pEditCamera;

	std::unique_ptr<CPeraRenderer>		m_pPeraRenderer;
	std::unique_ptr<CSkyDome>			m_pSkyDome;
	std::unique_ptr<CGroundStage>		m_pGroundStage;

	int m_NowEditScene;
};

#endif	// #ifndef EDITOR_H.