#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\GameObject\Widget\SceneWidget\TItleWidget\TitleWidget.h"
#include "..\..\..\GameObject\Widget\SceneWidget\ConfigWidget\ConfigWidget.h"
#include "..\..\..\GameObject\Widget\Fade\Fade.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\XAudio2\SoundManager.h"
#include "..\..\..\Common\SceneTexRenderer\SceneTexRenderer.h"
#include "..\..\..\GameObject\Widget\SceneTransition\SceneTransition.h"

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase		( pSceneManager )
	, m_pWidget			( nullptr )
	, m_pConfigWidget	( nullptr )
	, m_IsChangeScene	( false )
	, m_IsDecision		( false )
	, m_IsNowConfig		( false )
{
	m_pWidget		= std::make_unique< CTitleWidget >();
	m_pConfigWidget	= std::make_unique<CConfigWidget>();
	CFade::SetFadeOut();
}

CTitle::~CTitle()
{
}

//============================.
//	読込関数.
//============================.
bool CTitle::Load()
{
	if ( m_pWidget->Init() == false ) return false;
	if( m_pConfigWidget->Init() == false ) return false;
	CSoundManager::ThreadPlayBGM("TitleBGM");
	CSoundManager::FadeInBGM("TitleBGM");
	m_pSceneManager->SetNowBGMName("TitleBGM");
	return true;
}

//============================.
//	更新関数.
//============================.
void CTitle::Update()
{
	if (CFade::GetIsFade() == true) return;
	if( m_IsNowConfig == false ){
		// 決定してない場合UIの更新.
		if( m_IsDecision == false ) m_pWidget->Update();
		//シーン切り替え.
		ChangeScene();
	} else {
		m_pConfigWidget->Update();
		if( m_pConfigWidget->IsEndConfig()== true ){
			m_pConfigWidget->OffVolumeSeting();
			m_IsNowConfig = false;
			m_IsChangeScene = false;
			m_IsDecision = false;
		}
	}
}

//============================.
//	描画関数.
//============================.
void CTitle::Render()
{
	// タイトルでは影やエフェクトを描画させる予定はないので.
	//	G-Bufferのみ描画させる.
	//--------------------------------------------.
	// 描画パス1.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();

	// タイトルUIの描画.
	if ( m_pWidget == nullptr ) return;
	m_pWidget->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.
	//	Bloomは使用しないので、無効にする.
	CSceneTexRenderer::Render( false );

	// 設定UIの描画.
	if( m_IsNowConfig == true ) m_pConfigWidget->Render();
}

//============================.
	// シーン切り替え関数.
//============================.
void CTitle::ChangeScene()
{
	// ボタンが押された.
	if( CInput::IsMomentPress( EKeyBind::Decision ) == true ){
		if (m_IsChangeScene == true) return;
		switch (m_pWidget->GetSelectState())
		{
		case CTitleWidget::ESelectState::Start:
			CSoundManager::PlaySE("Determination");
//			CFade::SetFadeIn();
			CSceneTransition::SetSrcTexture( CSceneTexRenderer::GetGBuffer()[0] );
			CSceneTransition::SetTexture();
//			CSceneTransition::SetFadeIn();
			CSoundManager::FadeOutBGM("TitleBGM");
			break;
		case CTitleWidget::ESelectState::Config:
			CSoundManager::PlaySE("Determination");
			m_IsNowConfig = true;
			break;
		case CTitleWidget::ESelectState::End:
			CSoundManager::PlaySE("CancelDetermination");
			CFade::SetFadeIn();
			CSoundManager::FadeOutBGM("TitleBGM");
			break;
		default:
			break;
		}
		m_IsChangeScene = true;
		m_IsDecision = true;
	}
	if( m_IsChangeScene == true ){
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while( CSoundManager::StopBGMThread("TitleBGM") == false);
		m_pSceneManager->NextSceneMove();
	}

	if (m_IsChangeScene == false) return;
	// フェードイン状態かつフェード中なら処理しない.
	if (CFade::GetFadeState() != CFade::EFadeState::In) return;
	if(CFade::GetIsFade() == true) return;
	// どのシーンに遷移するか.
	switch (m_pWidget->GetSelectState())
	{
	case CTitleWidget::ESelectState::Start:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while( CSoundManager::StopBGMThread("TitleBGM") == false);
		m_pSceneManager->NextSceneMove();
		break;
	case CTitleWidget::ESelectState::Config:
		break;
	case CTitleWidget::ESelectState::End:
		if (CSoundManager::GetBGMVolume("TitleBGM") > 0.0f) return;
		while (CSoundManager::StopBGMThread("TitleBGM") == false);
		// ウィンドウを閉じる.
		m_pSceneManager->EndGameClose();
		break;
	default:
		break;
	}
}

