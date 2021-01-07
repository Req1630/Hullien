#include "SceneTransition.h"
#include "..\..\..\Common\Transition\Transition.h"

CSceneTransition::CSceneTransition()
	: m_pTransition		( nullptr )
	, m_pDestTexture	( nullptr )
	, m_pSrcTexture		( nullptr )
	, m_FadeState		( EFadeState::Finish )
	, m_NowAlpha		( 0.0f )
	, m_IsFade			( false )
{
	m_pTransition = std::make_unique<CTransition>();
}

CSceneTransition::~CSceneTransition()
{
}

// 初期化関数.
bool CSceneTransition::Init()
{
	static bool end = false;
	if( end == true ) return true; 
	if( FAILED( GetInstance()->m_pTransition->Init( 
		CDirectX11::GetDevice(), CDirectX11::GetContext(),
		"Data\\Texture\\UI\\Fade\\UFOTransitoinRule.png",
		1280, 720 ))) return false;
	end = true;
	return true;
}

// 更新関数.
void CSceneTransition::Update()
{
	if( m_FadeState != EFadeState::In ) return;
	m_NowAlpha += ALPHA_ADD_VALUE;

	if( m_NowAlpha < ALPHA_MAX ) return;
	m_FadeState = EFadeState::Finish;
}

// 描画関数.
void CSceneTransition::Render()
{
	GetInstance()->Update();
	if( GetInstance()->m_FadeState == EFadeState::Finish ) return;
	GetInstance()->m_pTransition->SetValue( GetInstance()->m_NowAlpha );
//	GetInstance()->m_pTransition->SetTexture( GetInstance()->m_pSrcTexture );
//	GetInstance()->m_pTransition->SetDestTexture( GetInstance()->m_pDestTexture );
	GetInstance()->m_pTransition->Render();
}

// スプライト設定関数.
bool CSceneTransition::SpriteSetting()
{
	return true;
}

// フェードイン設定関数.
void CSceneTransition::SetFadeIn()
{
	GetInstance()->m_NowAlpha = 0.0f;
	GetInstance()->m_FadeState = EFadeState::In;
}

// 基準となるテクスチャの設定.
void CSceneTransition::SetDestTexture( ID3D11ShaderResourceView* pDestTexture )
{
	GetInstance()->m_pTransition->SetDestTexture( pDestTexture );
}

// 合成するテクスチャの設定.
void CSceneTransition::SetSrcTexture( ID3D11ShaderResourceView* pSrcTexture )
{
	GetInstance()->m_pTransition->SetTexture( pSrcTexture );
}