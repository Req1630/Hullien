#include "..\EventList.h"

#include "..\..\Common\D3DX\D3DX11.h"
#include "..\..\Common\Shader\ShadowMap\ShadowMap.h"
#include "..\..\Common\PeraRenderer\PeraRenderer.h"

/************************************
*	�C�x���g�V�[���Ǘ��N���X.
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

// �X�V�֐�.
void CEventManager::Update()
{
	if( m_pEventBase == nullptr ) return;

	if (m_IsLoadEnd == false)
	{
		// �ǂݍ��݂��I�����Ă��Ȃ���΁A�ǂݍ��݂��s��.
		if (m_pPeraRenderer->Init(nullptr, nullptr) == E_FAIL) return;
		m_IsLoadEnd = m_pEventBase->Load();
	}
	else
	{
		// �ǂݍ��݂��I��������.
		m_IsEventEnd = m_pEventBase->GetIsEventEnd();
		m_pEventBase->Update();		//�C�x���g�̍X�V.
		// �C�x���g���I�����Ă��Ȃ���Ε`�悷��.
		if (m_IsEventEnd == false)
		{
			ModelRender();	// �C�x���g�̕`��.
		}
	}
}

// ���̃C�x���g�Ɉړ�.
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

// ���f���̕`��.
void CEventManager::ModelRender()
{
	//--------------------------------------------.
// �`��p�X1.
//--------------------------------------------.
// �[�x�e�N�X�`���ɉe�p�̐[�x����������.

	CShadowMap::SetRenderPass(0);
	m_pEventBase->Render();

	//--------------------------------------------.
	// �`��p�X2.
	//--------------------------------------------.
	// G-Buffer��color, normal, depth����������.

	CShadowMap::SetRenderPass(1);
	CDirectX11::SetGBuufer();
	m_pEventBase->Render();

	//--------------------------------------------.
	// �ŏI�`��.
	//--------------------------------------------.
	// G-Buffer���g�p���āA��ʂɕ`�悷��.

	CDirectX11::SetBackBuffer();
	m_pPeraRenderer->Render(CDirectX11::GetGBuffer());

}
