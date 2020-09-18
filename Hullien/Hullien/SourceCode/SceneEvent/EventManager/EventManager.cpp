#include "..\EventList.h"

#include "..\..\Common\D3DX\D3DX11.h"
#include "..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\Common\PeraRenderer\PeraRenderer.h"

/************************************
*	イベントシーン管理クラス.
**/
CEventManager::CEventManager()
	: m_pEventBase	( nullptr )
	, m_pPeraRenderer(nullptr)
	, m_NowEvent		( EEvent::Start )
	, m_NextEvent		( EEvent::Start )
	, m_IsLoadEnd		( false )
	, m_IsSkip			( false )
	, m_IsGameOver	( false )
	, m_IsEventEnd	( false ) 
{
	NextEventMove();
	m_pPeraRenderer = std::make_unique<CPeraRenderer>();
}

CEventManager::~CEventManager()
{
	m_pPeraRenderer->Release();
}

// 更新関数.
void CEventManager::Update()
{
	if( m_pEventBase == nullptr ) return;

	if (m_IsLoadEnd == false)
	{
		// 読み込みが終了していなければ、読み込みを行う.
		if (m_pPeraRenderer->Init(nullptr, nullptr) == E_FAIL) return;
		m_IsLoadEnd = m_pEventBase->Load();
	}
	else
	{
		// 読み込みが終了したら.
		m_IsEventEnd = m_pEventBase->GetIsEventEnd();
		m_pEventBase->Update();		//イベントの更新.
		// イベントが終了していなければ描画する.
		if (m_IsEventEnd == false)
		{
			ModelRender();	// イベントの描画.
		}
	}
}

// 次のイベントに移動.
void CEventManager::NextEventMove()
{
	m_IsLoadEnd = false;
	switch (m_NextEvent)
	{
	case EEvent::GameStart:
		m_pEventBase		= std::make_shared<CGameStartEvent>();
		m_IsGameOver	= false;
		m_IsEventEnd		= false;
		m_NowEvent		= m_NextEvent;
		m_NextEvent		= EEvent::GameClear;
		break;
	case EEvent::GameClear:
		m_pEventBase = std::make_shared<CGameClearEvent>();
		m_IsEventEnd = false;
		m_NowEvent = m_NextEvent;
		m_NextEvent = EEvent::GameStart;
		break;
	case EEvent::GameOver:
		m_pEventBase = std::make_shared<CGameOverEvent>();
		m_IsEventEnd = false;
		m_NowEvent = m_NextEvent;
		m_NextEvent = EEvent::GameStart;
		break;
	default:
		break;
	}
}

void CEventManager::EventRetry()
{
	m_IsLoadEnd = false;
	switch (m_NowEvent)
	{
	case EEvent::GameStart:
		m_pEventBase = std::make_shared<CGameStartEvent>();
		m_IsGameOver = false;
		m_IsEventEnd = false;
		break;
	case EEvent::GameClear:
		m_pEventBase = std::make_shared<CGameClearEvent>();
		m_IsEventEnd = false;
		break;
	case EEvent::GameOver:
		m_pEventBase = std::make_shared<CGameOverEvent>();
		m_IsEventEnd = false;
		break;
	default:
		break;
	}
}

// モデルの描画.
void CEventManager::ModelRender()
{
	//--------------------------------------------.
// 描画パス1.
//--------------------------------------------.
// 深度テクスチャに影用の深度を書き込む.

	CShadowMap::SetRenderPass(0);
	m_pEventBase->Render();

	//--------------------------------------------.
	// 描画パス2.
	//--------------------------------------------.
	// G-Bufferにcolor, normal, depthを書き込む.

	CShadowMap::SetRenderPass(1);
	CDirectX11::SetGBuufer();
	m_pEventBase->Render();

	//--------------------------------------------.
	// 最終描画.
	//--------------------------------------------.
	// G-Bufferを使用して、画面に描画する.

	CDirectX11::SetBackBuffer();
	m_pPeraRenderer->Render(CDirectX11::GetGBuffer());

}
