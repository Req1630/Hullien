#include "..\EventList.h"

/************************************
*	�C�x���g�V�[���Ǘ��N���X.
**/
CEventManager::CEventManager()
	: m_pEventBase	( nullptr )
	, m_NowEvent		( EEvent::Start )
	, m_NextEvent		( EEvent::Start )
	, m_IsLoadEnd		( false )
	, m_IsSkip			( false )
	, m_IsGameOver	( false )
	, m_IsEventEnd	( false ) 
{
	NextEventMove();
}

CEventManager::~CEventManager()
{
}

// �X�V�֐�.
void CEventManager::Update()
{
	if( m_pEventBase == nullptr ) return;

	if (m_IsLoadEnd == false)
	{
		// �ǂݍ��݂��I�����Ă��Ȃ���΁A�ǂݍ��݂��s��.
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
			m_pEventBase->Render();	// �C�x���g�̕`��.
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
