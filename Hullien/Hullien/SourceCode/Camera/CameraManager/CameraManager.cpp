#include "CameraManager.h"
#include "..\Camera.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

//---------------------------.
// カメラの設定.
//---------------------------.
void CCameraManager::SetCamera( std::shared_ptr<CCamera> pCamera )
{
	GetInstance()->m_pCamera = pCamera;
}

//---------------------------.
// 更新関数.
//---------------------------.
void CCameraManager::Update()
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->Update();
}

//---------------------------.
// ビュー行列、プロジェクション行列の初期化.
//---------------------------.
void CCameraManager::InitViewProj()
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->InitViewProj();
}

//---------------------------.
// ビュー行列取得関数.
//---------------------------.
D3DXMATRIX CCameraManager::GetViewMatrix()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetViewMatrix();
}

//---------------------------.
// プロジェクション行列取得関数.
//---------------------------.
D3DXMATRIX CCameraManager::GetProjMatrix()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetProjMatrix();
}

//---------------------------.
// カメラ座標取得関数.
//---------------------------.
D3DXVECTOR3 CCameraManager::GetPosition()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetPosition();
}

//---------------------------.
// カメラ注視座標取得関数.
//---------------------------.
D3DXVECTOR3 CCameraManager::GetLookPosition()
{
	if( GetInstance()->m_pCamera == nullptr ) return {};
	return GetInstance()->m_pCamera->GetLookPosition();
}

// 座標の設定.
void CCameraManager::SetPosition( const D3DXVECTOR3& pos )
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->SetPosition( pos );
}

// 視点座標の設定.
void CCameraManager::SetLookPosition( const D3DXVECTOR3& pos )
{
	if( GetInstance()->m_pCamera == nullptr ) return;
	GetInstance()->m_pCamera->SetLookPosition( pos );
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CCameraManager* CCameraManager::GetInstance()
{
	static std::unique_ptr<CCameraManager> pInstance =
		std::make_unique<CCameraManager>();

	return pInstance.get();
}