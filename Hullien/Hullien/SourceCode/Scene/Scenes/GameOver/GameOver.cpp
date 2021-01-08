#include "..\SceneList.h"
#include "..\..\..\GameObject\Widget\SceneWidget\GameOverWidget\GameOverWidget.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

CGameOver::CGameOver( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pGameOverWidget	( nullptr )
	, m_IsChangeScene	( false )
{
	m_pGameOverWidget = std::make_unique<CGameOverWidget>();
}

CGameOver::~CGameOver()
{
}

//============================.
//	読込関数.
//============================.
bool CGameOver::Load()
{
	CFade::SetFadeOut();
	if (m_pGameOverWidget->Init() == false) return false;

	CSoundManager::ThreadPlayBGM("GameOverBGM");
	CSoundManager::FadeInBGM("GameOverBGM");
	m_pSceneManager->SetNowBGMName("GameOverBGM");
	CSceneTexRenderer::SetIsStartLoad( false );
	return true;
}

//============================.
//	更新関数.
//============================.
void CGameOver::Update()
{
	if (CFade::GetFadeState() == CFade::EFadeState::Out
		&& CFade::GetIsFade() == true) return;
	m_pGameOverWidget->Update();


	// 以下シーン遷移.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		if(m_IsChangeScene == true) return;
		CFade::SetFadeIn();
		CSoundManager::PlaySE("Determination");
		CSoundManager::FadeOutBGM("GameOverBGM");
		m_IsChangeScene = true;
	}
	if( m_IsChangeScene == false ) return;
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if (CFade::GetIsFade() == true) return;
	while(CSoundManager::StopBGMThread("GameOverBGM") == false);
	m_pSceneManager->NextSceneMove();
	CSceneTexRenderer::SetIsStartLoad( true );
	CSceneTexRenderer::SetSaveScreen( true );
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();
	CSceneTexRenderer::Render();

}

//============================.
//	描画関数.
//============================.
void CGameOver::Render()
{
	// ゲームオーバーでは影やエフェクトを描画させる予定はないので.
	//	G-Bufferのみ描画させる.
	//--------------------------------------------.
	// 描画パス1.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();

	m_pGameOverWidget->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.
	//	Bloomは使用しないので、無効にする.
	CSceneTexRenderer::Render( false );
}