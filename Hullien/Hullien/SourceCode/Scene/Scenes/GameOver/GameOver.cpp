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
//	�Ǎ��֐�.
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
//	�X�V�֐�.
//============================.
void CGameOver::Update()
{
	if (CFade::GetFadeState() == CFade::EFadeState::Out
		&& CFade::GetIsFade() == true) return;
	m_pGameOverWidget->Update();


	// �ȉ��V�[���J��.
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
//	�`��֐�.
//============================.
void CGameOver::Render()
{
	// �Q�[���I�[�o�[�ł͉e��G�t�F�N�g��`�悳����\��͂Ȃ��̂�.
	//	G-Buffer�̂ݕ`�悳����.
	//--------------------------------------------.
	// �`��p�X1.
	//--------------------------------------------.
	// G-Buffer��color, normal, depth����������.
	CSceneTexRenderer::SetRenderPass( CSceneTexRenderer::ERenderPass::GBuffer );
	CSceneTexRenderer::SetGBuffer();

	m_pGameOverWidget->Render();

	//--------------------------------------------.
	// �ŏI�`��.
	//--------------------------------------------.
	// G-Buffer���g�p���āA��ʂɕ`�悷��.
	//	Bloom�͎g�p���Ȃ��̂ŁA�����ɂ���.
	CSceneTexRenderer::Render( false );
}