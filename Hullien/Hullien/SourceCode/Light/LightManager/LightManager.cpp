#include "LightManager.h"
#include "..\LightBase.h"

CLightManager::CLightManager()
	: m_pLight	( nullptr )
{
}
CLightManager::~CLightManager()
{
}

//-----------------------------------.
// ���C�g�N���X�̐ݒ�.
//-----------------------------------.
void CLightManager::SetLgiht( std::shared_ptr<CLightBase> pLight )
{
	GetInstance()->m_pLight = pLight;
}

//-----------------------------------.
// ���W�̎擾.
//-----------------------------------.
D3DXVECTOR3 CLightManager::GetPosition()
{
	if( GetInstance()->m_pLight == nullptr ) 
		return { 0.0f, 5.0f, 0.0f };
	return GetInstance()->m_pLight->GetPosition();
}

//-----------------------------------.
// �����̎擾.
//-----------------------------------.
D3DXVECTOR3 CLightManager::GetDirection()
{
	if( GetInstance()->m_pLight == nullptr ) 
		return { 1.5f, 1.0f, -1.0f };
	return GetInstance()->m_pLight->GetDirection();
}

//-----------------------------------.
// ��]�s��̎擾.
//-----------------------------------.
D3DXMATRIX CLightManager::GetRorarionMatrix()
{
	if( GetInstance()->m_pLight == nullptr ){
		D3DXMATRIX mRot;
		D3DXMatrixIdentity( &mRot );
		return mRot;
	}
	return GetInstance()->m_pLight->GetRorarionMatrix();
}

//-----------------------------------.
// �����̎擾.
//-----------------------------------.
float CLightManager::GetIntensity()
{ 
	if( GetInstance()->m_pLight == nullptr ) return 1.0f;
	return GetInstance()->m_pLight->GetIntensity(); 
}

//-----------------------------------.
// �C���X�^���X�̎擾.
//-----------------------------------.
CLightManager* CLightManager::GetInstance()
{
	static std::unique_ptr<CLightManager> pInstance =
		std::make_unique<CLightManager>();
	return pInstance.get();
};