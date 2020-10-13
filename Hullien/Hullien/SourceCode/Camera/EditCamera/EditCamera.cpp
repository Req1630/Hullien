#include "EditCamera.h"
#include "..\..\Utility\Mouse\Mouse.h"

namespace
{
	const float CAMERA_MOVE_SPEED				= 5.0f;		// カメラの移動速度.
	const float CAMERA_POS_CAMERA_LOOK_DISTANCE = 15.0f;	// カメラと視点の距離.
	const float MOUSE_MOVE_SPEED				= 0.02f;	// マウスの移動速度.
	const float RADIAN_THRESHOLD_X_MAX			= 360.0f;	// ラジアンXの最大しきい値.
	const float RADIAN_THRESHOLD_X_MIN			= 0.0f;		// ラジアンXの最小しきい値.
	const float RADIAN_THRESHOLD_Y_MAX			= 170.0f;	// ラジアンYの最大しきい値.
	const float RADIAN_THRESHOLD_Y_MIN			= -170.0f;	// ラジアンYの最小しきい値.
}

CEditCamera::CEditCamera()
	: m_hWnd	( nullptr )
	, m_pMouse	( nullptr )
	, m_Radian	( 0.0f, 0.0f )
{
	m_pMouse = std::make_unique<CMouse>();
}

CEditCamera::~CEditCamera()
{
}

// 更新関数.
void CEditCamera::Updata()
{
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll( &mRot, m_Radian.x, 0.0f, 0.0f );
	// 軸ベクトルを用意.
	D3DXVECTOR3 vecAxisX( CAMERA_MOVE_SPEED, 0.0f, 0.0f );	// X軸.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, CAMERA_MOVE_SPEED );	// Z軸.
	// X軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisX, &vecAxisX, &mRot );
	// Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &mRot );

	// 前進.
	if( GetAsyncKeyState('W') & 0x8000 ) m_vPosition += vecAxisZ;
	// 後退.
	if( GetAsyncKeyState('S') & 0x8000 ) m_vPosition -= vecAxisZ;
	// 右に移動.
	if( GetAsyncKeyState('D') & 0x8000 ) m_vPosition += vecAxisX;
	// 左に移動.
	if( GetAsyncKeyState('A') & 0x8000 ) m_vPosition -= vecAxisX;
	// 上昇.
	if( GetAsyncKeyState('Q') & 0x8000 ) m_vPosition.y += CAMERA_MOVE_SPEED;
	// 下降.
	if( GetAsyncKeyState('E') & 0x8000 ) m_vPosition.y -= CAMERA_MOVE_SPEED;

	MouseUpdate();

	// カメラ位置を算出.
	m_vLookPosition.x = m_vPosition.x + ( sinf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_vLookPosition.y = m_vPosition.y + ( sinf(m_Radian.y) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
	m_vLookPosition.z = m_vPosition.z + ( cosf(m_Radian.x) * CAMERA_POS_CAMERA_LOOK_DISTANCE );
}

// マウスの更新.
void CEditCamera::MouseUpdate()
{
	m_pMouse->UpdateMouse( m_hWnd );	// マウスの更新.
	// マウスが画面外なら終了.
	if( m_pMouse->IsScreenMiddle() == false ) return;
	if(!( GetAsyncKeyState(VK_LBUTTON) & 0x8000 )) return;

	// マウスの現在の座標と過去の座標を引いた値を算出.
	float xSub = m_pMouse->GetPosisionX() - m_pMouse->GetOldPosisionX();
	float ySub = m_pMouse->GetPosisionY() - m_pMouse->GetOldPosisionY();

	if( xSub < 0.0f ) m_Radian.x -= MOUSE_MOVE_SPEED;
	if( xSub > 0.0f ) m_Radian.x += MOUSE_MOVE_SPEED;
	if( ySub > 0.0f ) m_Radian.y -= MOUSE_MOVE_SPEED;
	if( ySub < 0.0f ) m_Radian.y += MOUSE_MOVE_SPEED;
	if( m_Radian.x > static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MAX)) )
		m_Radian.x = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MIN));
	if( m_Radian.x < static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MIN)) ) 
		m_Radian.x = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_X_MAX));
	if( m_Radian.y > static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MAX)) )
		m_Radian.y = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MAX));
	if( m_Radian.y < static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MIN)) )
		m_Radian.y = static_cast<float>(D3DXToRadian(RADIAN_THRESHOLD_Y_MIN));
}