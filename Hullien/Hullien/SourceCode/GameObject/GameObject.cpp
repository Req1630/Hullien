#include "GameObject.h"
#include "..\Camera\CameraManager\CameraManager.h"
#include "..\Common\SceneTexRenderer\SceneTexRenderer.h"


/****************************************
*	ゲームオブジェクト元クラス.
**/
CGameObject::CGameObject()
	: m_ObjectTag	( EObjectTag::None )
	, m_vPosition	( 0.0f, 0.0f, 0.0f )
	, m_vRotation	( 0.0f, 0.0f, 0.0f )
	, m_vScale		( 1.0f, 1.0f, 1.0f )
{
}

CGameObject::~CGameObject()
{
}

//-----------------------------.
// 画面の外に出ているか.
//-----------------------------.
bool CGameObject::IsDisplayOut( const float& adjSize )
{
	const float wnd_w = static_cast<float>(CDirectX11::GetWndWidth());
	const float wnd_h = static_cast<float>(CDirectX11::GetWndHeight());
	// ビューポート行列.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity( &mViewport );	// 単位行列作成.
	mViewport._11 = wnd_w *  0.5f;
	mViewport._22 = wnd_h * -0.5f;
	mViewport._41 = wnd_w *  0.5f;
	mViewport._42 = wnd_h *  0.5f;

	// ワールド, ビュー, プロジェ, ビューポートの計算.
	D3DXMATRIX m = CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix() * mViewport;
	D3DXVECTOR3 screenPos = { 0.0f, 0.0f, 0.0f };	// スクリーン座標.
	// D3DXVECTOR3と掛け合わせ.
	D3DXVec3TransformCoord( &screenPos, &m_vPosition, &m );

	if( -adjSize < screenPos.x && screenPos.x < wnd_w + adjSize &&
		-adjSize < screenPos.y && screenPos.y < wnd_h + adjSize ) return false;	// 画面内.
	// 画面外でもシャドウは描画する.
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return false;
	
	return true;	// 画面外.
}