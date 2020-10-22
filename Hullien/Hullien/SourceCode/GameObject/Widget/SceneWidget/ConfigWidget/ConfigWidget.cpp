#include "ConfigWidget.h"
#include "..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Utility\XInput\XInput.h"
#include "..\..\..\..\XAudio2\SoundManager.h"
#include "VolumeConifgWidget/VolumeConifgWidget.h"

/********************************************
*	�ݒ�UI�N���X.
**/
CConfigWidget::CConfigWidget()
	: m_pVolumeConfig	( nullptr )
	, m_NowConfigState	( EConfigState_None )
{
	m_pVolumeConfig = std::make_unique<CVolumeConfigWidget>();
}

CConfigWidget::~CConfigWidget()
{
}

// �������֐�.
bool CConfigWidget::Init()
{
	if( m_pVolumeConfig->Init() == false ) return false;
	return true;
}

// �X�V�֐�.
void CConfigWidget::Update()
{
	if( GetAsyncKeyState('B') & 0x0001 ){
		m_NowConfigState = EConfigState_Volume;
		m_pVolumeConfig->OnVolumeSeting();
	}
	if( GetAsyncKeyState('V') & 0x0001 ){
		m_NowConfigState = EConfigState_None;
		m_pVolumeConfig->OffVolumeSeting();
	}
	switch( m_NowConfigState )
	{
	case EConfigState_Volume:
		m_pVolumeConfig->Update();	// ���ʐݒ�UI�̍X�V.
		break;
	case EConfigState_Key:
		break;
	default:
		break;
	}
}

// �`��֐�.
void CConfigWidget::Render()
{
	switch( m_NowConfigState )
	{
	case EConfigState_Volume:
		m_pVolumeConfig->Render();	// ���ʐݒ�UI�̕`��.
		break;
	case EConfigState_Key:
		break;
	default:
		break;
	}
}

// �X�v���C�g�ݒ�֐�.
bool CConfigWidget::SpriteSetting()
{
	return true;
}