#include "EventCharacter.h"
#include "..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

/***************************************
*	イベント用キャラクタクラス.
**/
CEventCharacter::CEventCharacter()
	: m_pSkinMesh( nullptr )
#ifdef IS_TEMP_MODEL_RENDER
	, m_pTempStaticMesh( nullptr )
#endif
	, m_MoveVector					( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation				( 0.0f, 0.0f, 0.0f )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter						()
	, m_NowMoveState				( EMoveState::None )
	, m_ModelAlpha					( 0.0f )
{
	m_vPosition.y = INIT_POSITION_ADJ_HEIGHT;
}

CEventCharacter::~CEventCharacter()
{
}

// 移動関数.
void CEventCharacter::Move()
{
	TargetRotation();
	CEventCharacter::VectorMove(m_Parameter.MoveSpeed);
}

// 情報設定関数.
void CEventCharacter::SetOptionalState(SOptionalState state)
{
	m_Parameter.Destination = state.Destination;
	m_Parameter.MoveSpeed = state.MoveSpeed;
	m_Parameter.RotationalSpeed = state.RotationalSpeed;
}

// メッシュの表示関数.
void CEventCharacter::MeshRender()
{
#ifndef IS_TEMP_MODEL_RENDER
	if (m_pSkinMesh == nullptr) return;

	m_pSkinMesh->SetPosition(m_vPosition);
	m_pSkinMesh->SetRotation(m_vRotation);
	m_pSkinMesh->SetScale(m_vSclae);
	m_pSkinMesh->SetAnimSpeed(0.01);
	m_pSkinMesh->Render();
#else
	if (m_pTempStaticMesh == nullptr) return;

	m_pTempStaticMesh->SetPosition(m_vPosition);
	m_pTempStaticMesh->SetRotation(m_vRotation);
	m_pTempStaticMesh->SetScale(m_vSclae);
	m_pTempStaticMesh->Render();
#endif	// #ifdef IS_TEMP_MODEL_RENDER.
}

// モデルの取得関数
bool CEventCharacter::GetModel(const char * modelName)
{
#ifndef IS_TEMP_MODEL_RENDER
	// 既に読み込めていたら終了.
	if (m_pSkinMesh != nullptr) return true;
	// モデルの取得.
	CMeshResorce::GetSkin(m_pSkinMesh, modelName);
	// モデルが読み込めてなければ false.
	if (m_pSkinMesh == nullptr) return false;
	return true;
#else
	// 既に読み込めていたら終了.
	if (m_pTempStaticMesh != nullptr) return true;
	// モデルの取得.
	CMeshResorce::GetStatic(m_pTempStaticMesh, modelName);
	// モデルが読み込めてなければ false.
	if (m_pTempStaticMesh == nullptr) return false;
	return true;
#endif	// #ifndef IS_MODEL_RENDER.
}

// 移動ベクトル設定関数.
void CEventCharacter::SetMoveVector(const D3DXVECTOR3& targetPos)
{
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f(
		targetPos.x - m_vPosition.x,
		targetPos.z - m_vPosition.z);

	// 移動用ベクトルを取得.
	m_MoveVector.x = sinf(m_TargetRotation.y);
	m_MoveVector.z = cosf(m_TargetRotation.y);
}

// 目的の座標へ回転.
void CEventCharacter::TargetRotation()
{
	if (m_NowMoveState != EMoveState::Rotation) return;

	// 目的地のベクトルを用意.
	SetMoveVector( m_Parameter.Destination );
	
	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf(m_vRotation.y);
	myVector.z = cosf(m_vRotation.y);

	// ベクトルの長さを求める.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// 目的のベクトルと、自分のベクトルの外積を求める.
	float cross = myVector.x * m_MoveVector.z - myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x + myVector.z * m_MoveVector.z;
	dot = acosf(dot / (myLenght * targetLenght));

	// 外積が0.0より少なければ 時計回り : 反時計回り に回転する.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// 内積が許容範囲なら.
	if (-TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN) {
		m_vRotation.y = m_TargetRotation.y;	// ターゲットへの回転取得.
		// 移動用ベクトルを取得.
		m_MoveVector.x = sinf(m_vRotation.y);
		m_MoveVector.z = cosf(m_vRotation.y);
		m_BeforeMoveingPosition = m_vPosition;
		m_NowMoveState = EMoveState::Move;
	}
}

// 移動関数.
void CEventCharacter::VectorMove(const float& moveSpeed)
{
	if (m_NowMoveState != EMoveState::Move) return;

	float lenght = D3DXVec3Length(&D3DXVECTOR3(m_Parameter.Destination - m_vPosition));

	m_vPosition.x -= sinf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;
	m_vPosition.z -= cosf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;

	float researchLengh = D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_vPosition));
	if (researchLengh >= m_Parameter.ResearchLenght) m_NowMoveState = EMoveState::Rotation;

	if (lenght >= 1.0f) return;
}

/******************************************************
*	以下情報取得・設定関数.
*/

// X座標位置設定関数.
void CEventCharacter::SetPositionX(const float& vPos_x)
{
	m_vPosition.x = vPos_x;
}

// Y座標位置設定関数.
void CEventCharacter::SetPositionY(const float& vPos_y)
{
	m_vPosition.y = vPos_y;
}

// Z座標位置設定関数.
void CEventCharacter::SetPositionZ(const float& vPos_z)
{
	m_vPosition.z = vPos_z;
}

// 回転値取得関数.
D3DXVECTOR3 CEventCharacter::GetRotation() const
{
	return m_vRotation;
}

// 回転値設定関数.
void CEventCharacter::SetRotation(const D3DXVECTOR3& vRot)
{
	m_vRotation = vRot;
}

// X座標回転値設定関数.
void CEventCharacter::SetRotationX(const float& vRot_x)
{
	m_vRotation.x = vRot_x;
}

// Y座標回転値設定関数.
void CEventCharacter::SetRotationY(const float& vRot_y)
{
	m_vRotation.y = vRot_y;
}

// Z座標回転値設定関数.
void CEventCharacter::SetRotationZ(const float& vRot_z)
{
	m_vRotation.z = vRot_z;
}

// 大きさ取得関数.
D3DXVECTOR3 CEventCharacter::GetScale() const
{
	return m_vSclae;
}

// 大きさ設定関数.
void CEventCharacter::SetScale(const D3DXVECTOR3& vScale)
{
	m_vSclae = vScale;
}

// X座標大きさ設定関数.
void CEventCharacter::SetScaleX(const float & vScale_x)
{
	m_vSclae.x = vScale_x;
}

// Y座標大きさ設定関数.
void CEventCharacter::SetScaleY(const float & vScale_y)
{
	m_vSclae.y = vScale_y;
}

// Z座標大きさ設定関数.
void CEventCharacter::SetScaleZ(const float & vScale_z)
{
	m_vSclae.z = vScale_z;
}
