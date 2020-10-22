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
	m_pVolumeConfig->Update();	// ���ʐݒ�UI�̍X�V.
}

// �`��֐�.
void CConfigWidget::Render()
{
	m_pVolumeConfig->Render();	// ���ʐݒ�UI�̕`��.
}

// �X�v���C�g�ݒ�֐�.
bool CConfigWidget::SpriteSetting()
{
	return true;
}