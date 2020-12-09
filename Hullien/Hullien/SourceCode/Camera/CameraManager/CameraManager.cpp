#include "CameraManager.h"
#include "..\Camera.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

//---------------------------.
// �J�����̐ݒ�.
//---------------------------.
void CCameraManager::SetCamera( std::shared_ptr<CCamera> pCamera )
{
	GetInstance()->m_pCamera = pCamera;
}

//---------------------------.
// �X�V�֐�.
//---------------------------.
void CCameraManager::Update()
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->Update();
}

//---------------------------.
// �r���[�s��A�v���W�F�N�V�����s��̏�����.
//---------------------------.
void CCameraManager::InitViewProj()
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->InitViewProj();
}

//---------------------------.
// �r���[�s��擾�֐�.
//---------------------------.
D3DXMATRIX CCameraManager::GetViewMatrix()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetViewMatrix();
}

//---------------------------.
// �v���W�F�N�V�����s��擾�֐�.
//---------------------------.
D3DXMATRIX CCameraManager::GetProjMatrix()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetProjMatrix();
}

//---------------------------.
// �J�������W�擾�֐�.
//---------------------------.
D3DXVECTOR3 CCameraManager::GetPosition()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetPosition();
}

//---------------------------.
// �J�����������W�擾�֐�.
//---------------------------.
D3DXVECTOR3 CCameraManager::GetLookPosition()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetLookPosition();
}

// ���W�̐ݒ�.
void CCameraManager::SetPosition( const D3DXVECTOR3& pos )
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->SetPosition( pos );
}

// ���_���W�̐ݒ�.
void CCameraManager::SetLookPosition( const D3DXVECTOR3& pos )
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->SetLookPosition( pos );
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CCameraManager* CCameraManager::GetInstance()
{
	static std::unique_ptr<CCameraManager> pInstance =
		std::make_unique<CCameraManager>();

	return pInstance.get();
}