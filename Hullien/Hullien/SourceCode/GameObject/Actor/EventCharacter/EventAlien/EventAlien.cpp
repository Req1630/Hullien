#include "EventAlien.h"
#include "..\..\..\..\Collider\CollsionManager\CollsionManager.h"

/****************************************
*	イベント用宇宙人クラス.
**/
CEventAlien::CEventAlien()
	: m_TargetRotation				( 0.0f, 0.0f, 0.0f )
	, m_pAbductUFOPosition		( nullptr )
	, m_BeforeMoveingPosition	( 0.0f, 0.0f, 0.0f )
	, m_Parameter						()
	, m_NowState						( EEventAlienState::None )
	, m_NowMoveState				( EMoveState::None )
	, m_ModelAlpha					( 0.0f )
	, m_WaitCount						( 0 )
{
}

CEventAlien::~CEventAlien()
{
}

// 相手座標の設定.
void CEventAlien::SetTargetPos(CEventActor & actor)
{
	SetGirlPos(actor);
}

// 現在の状態の更新関数.
void CEventAlien::CurrentStateUpdate()
{
	switch (m_NowState)
	{
	case EEventAlienState::Spawn:
		this->Spawning();
		break;
	case EEventAlienState::Move:
		this->Move();
		break;
	case EEventAlienState::Abduct:
		this->Abduct();
		break;
	case EEventAlienState::Escape:
		this->Escape();
		break;
	case EEventAlienState::BlowAway:
		this->BlowAway();
		break;
	case EEventAlienState::Wait:
		this->Wait();
		break;
	default:
		break;
	}
}

// 女の子の座標を設定.
void CEventAlien::SetGirlPos(CEventActor& actor)
{
	if (m_NowMoveState == EMoveState::Move) return;
	// 女の子じゃなければ終了.
	if (actor.GetObjectTag() != EObjectTag::Girl) return;
	m_vDestination = actor.GetPosition();	// 女の子の座標を取得.

}

// 移動ベクトル設定関数.
void CEventAlien::SetMoveVector(const D3DXVECTOR3 & targetPos)
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
void CEventAlien::TargetRotation()
{
	if (m_NowMoveState != EMoveState::Rotation) return;

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
void CEventAlien::VectorMove(const float& moveSpeed)
{
	if (m_NowMoveState != EMoveState::Move) return;

	float lenght = D3DXVec3Length(&D3DXVECTOR3(m_vDestination - m_vPosition));

	m_vPosition.x -= sinf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;
	m_vPosition.z -= cosf(m_vRotation.y + static_cast<float>(D3DX_PI)) * moveSpeed;

	float researchLengh = D3DXVec3Length(&D3DXVECTOR3(m_BeforeMoveingPosition - m_vPosition));
	if (researchLengh >= m_Parameter.ResearchLenght) m_NowMoveState = EMoveState::Rotation;

	if (lenght >= 1.0f) return;

	m_NowMoveState = EMoveState::Wait;
}

// 待機関数.
void CEventAlien::WaitMove()
{
	if (m_NowMoveState != EMoveState::Wait) return;
	m_WaitCount++;	// 待機カウント加算.
	if (m_WaitCount < m_Parameter.WaitTime*FPS) return;
	m_NowMoveState = EMoveState::Rotation;	// 移動状態を回転する.
	m_WaitCount = 0;	// 待機カウントの初期化.
}

// スポーン中.
void CEventAlien::Spawning()
{
	// モデルのスケール値を足していく.

	// モデルのアルファ値を足していく.
	m_ModelAlpha += m_Parameter.ModelAlphaAddValue;
	if (m_ModelAlpha < MODEL_ALPHA_MAX) return;
	m_NowState = EEventAlienState::Move;
	m_NowMoveState = EMoveState::Rotation;
}

// 移動.
void CEventAlien::Move()
{
	TargetRotation();			// 回転.
	CEventAlien::VectorMove(m_MoveSpeed);	// 移動.
	CEventAlien::WaitMove();							// 待機.

	if (m_NowState == EEventAlienState::Abduct) return;
	m_NowState = EEventAlienState::Escape;
	m_NowMoveState = EMoveState::Rotation;	// 移動状態を回転する.
}

// 拐う.
void CEventAlien::Abduct()
{
	if (m_IsBarrierHit == true) return;

	SetMoveVector(*m_pAbductUFOPosition);
	m_vDestination = *m_pAbductUFOPosition;

	TargetRotation();
	CEventAlien::VectorMove(m_MoveSpeed);
}

// 逃げる.
void CEventAlien::Escape()
{
	if (m_IsBarrierHit == true) return;

	SetMoveVector(*m_pAbductUFOPosition);
	m_vDestination = *m_pAbductUFOPosition;
	TargetRotation();
	CEventAlien::VectorMove(m_MoveSpeed);
}

// 吹き飛ぶ.
void CEventAlien::BlowAway()
{
}

// 待機.
void CEventAlien::Wait()
{
}

// 女の子との当たり判定.
void CEventAlien::GirlCollision(CEventActor* pActor)
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if (pActor->GetObjectTag() != EObjectTag::Girl) return;
	if (m_IsBarrierHit == true) return;
	if (m_NowMoveState == EMoveState::Attack) return;	// 攻撃状態は終了.
	if (m_NowState == EEventAlienState::Spawn) return;	// スポーン状態なら終了.

	bool isAbduct = false;
	if (m_NowState == EEventAlienState::Abduct) {
		isAbduct = true;
	}
	else {
		isAbduct = true;
	}

	if (isAbduct == false) return;
	// 球体の当たり判定.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) return;
	pActor->SetTargetPos(*this);

	if (m_NowState == EEventAlienState::Abduct) return;
	m_NowState = EEventAlienState::Abduct;
	m_NowMoveState = EMoveState::Rotation;
}

// バリアとの当たり判定.
void CEventAlien::BarrierCollision(CEventActor* pActor)
{
	// オブジェクトのタグが女の子じゃなければ終了.
	if (pActor->GetObjectTag() != EObjectTag::Bariier) return;
	const float moveSpeed = -2.0f;
	// 球体の当たり判定.
	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) {
		m_MoveSpeed = m_Parameter.MoveSpeed;
		m_IsBarrierHit = false;

	}
	else {
		m_MoveSpeed = moveSpeed;
		m_IsBarrierHit = true;
		m_NowState = EEventAlienState::Move;
		m_NowMoveState = EMoveState::Move;
	}
}
