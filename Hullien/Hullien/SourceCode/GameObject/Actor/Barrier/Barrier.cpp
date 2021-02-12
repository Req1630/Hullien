#include "Barrier.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\XAudio2\SoundManager.h"

#include "..\..\..\Common\Effect\EffectManager.h"

namespace
{
	const char* EFFECT_NAME							= "barrier_effect";	// エフェクトの名前.

	const float COLLISION_SPHERE_RDIUS_MAX			= 20.0f;	// 当たり判定の最大半径.
	const float COLLISION_SPHERE_RDIUS_ADD_VALUE	= 1.0f;		// 当たり判定サイズ加算値.
	const float ACTIVE_TIME							= 5.0f;		// 動作時間.
	const float EFFECT_SCALE						= 0.3f;		// エフェクトのサイズ.
	const float EFFECT_ADJ_POSITION_Y				= 0.001f;	// エフェクトの調整用座標.
};

CBarrier::CBarrier()
	: m_pEffect				( nullptr )
	, m_IsEffectPlay		( false )
	, m_IsActive			( false )
	, m_ActiveCount			( 0.0f )
	, m_CollSphereRadius	( 0.0f )
#if _DEBUG
	, m_ResizeCollTime		( 0.0f )
#endif	// #if _DEBUG.
{
	m_ObjectTag = EObjectTag::Bariier;
	m_pEffect = std::make_shared<CEffectManager>();

}

CBarrier::~CBarrier()
{
}

// 初期化関数.
bool CBarrier::Init()
{
	if( m_IsActive == true ) return false;
	if( ColliderSetting() == false ) return false;
	if (m_pEffect->SetEffect(EFFECT_NAME) == false) return false;
	m_IsActive			= true;
	m_CollSphereRadius	= 0.0f;
	m_ActiveCount		= 0.0f;
	m_IsEffectPlay = false;

	return true;
}

// 更新関数.
void CBarrier::Update()
{
	if( m_IsActive == false ) return;	// 動作してなければ終了.

	if( m_CollSphereRadius <= COLLISION_SPHERE_RDIUS_MAX ){
		CSoundManager::NoMultipleSEPlay("BarrierInvocating");
		m_CollSphereRadius += COLLISION_SPHERE_RDIUS_ADD_VALUE;	// 当たり半径の加算.
		return;
	}

	m_ActiveCount++;	// 動作カウントの加算.
	if( m_ActiveCount < ACTIVE_TIME*FPS ) return;
	m_IsActive = false;	// 動作しない.
	m_CollSphereRadius	= 0.0f;
	m_ActiveCount		= 0.0f;
	// 当たり判定のサイズを初期化.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// 描画関数.
void CBarrier::Render()
{
	if( m_IsActive == false ) return;		// 動作してなければ終了.
	if( m_pCollManager == nullptr ) return;

	//m_StaticMesh->SetPosition( m_vPosition );
	//m_StaticMesh->SetRotation( m_vRotation );
	//m_StaticMesh->SetScale( m_CollSphereRadius );
	//m_StaticMesh->SetColor( { 0.2f, 0.2f, 1.0f, 0.3f } );
	//m_StaticMesh->SetBlend( true );
	//m_StaticMesh->Render( true );
	//m_StaticMesh->SetBlend( false );

#if _DEBUG
	m_pCollManager->DebugRender();
	m_ResizeCollTime++;
	if( m_ResizeCollTime < 15.0f ) return;
	m_ResizeCollTime = 0.0f;
#endif	// #if _DEBUG.
	// 当たり判定のサイズを変更.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// エフェクト描画関数.
void CBarrier::EffectRender()
{
	if( m_IsActive == false ) return;		// 動作してなければ終了.
	if( m_pCollManager == nullptr ) return;

	// エフェクトを描画.
	m_pEffect->SetLocation( m_vPosition );
	m_pEffect->SetScale( EFFECT_SCALE );
	m_pEffect->Render();

#if _DEBUG
	m_pCollManager->DebugRender();
	m_ResizeCollTime++;
	if( m_ResizeCollTime < 15.0f ) return;
	m_ResizeCollTime = 0.0f;
#endif	// #if _DEBUG.
	// 当たり判定のサイズを変更.
	m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_CollSphereRadius,
		{ 0.0f, 0.0f, 0.0f },
		m_CollSphereRadius );
}

// 当たり判定関数.
void CBarrier::Collision( CActor* pActor )
{
	if( m_IsActive == false ) return;
	if( pActor == nullptr ) return;
	if( m_pCollManager == nullptr ) return;
	if( m_pCollManager->GetSphere() == nullptr ) return;

	// 対象オブジェクトじゃなければ終了.
	if( ( pActor->GetObjectTag() != EObjectTag::Alien_A ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_B ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_C ) &&
		( pActor->GetObjectTag() != EObjectTag::Alien_D )) return;

	// 球体の当たり判定.
	if( m_pCollManager->IsShereToShere( pActor->GetCollManager() ) == false ) return;
	pActor->SetTargetPos( *this );
	
}

// 相手座標の設定関数.
void CBarrier::SetTargetPos( CActor& pActor )
{
	if( m_IsActive == false ) return;
	m_vPosition = pActor.GetPosition();
	m_vPosition.y += EFFECT_ADJ_POSITION_Y;
	// すでにエフェクトを再生していれば終了.
	if (m_IsEffectPlay == true) return;
	m_pEffect->Play(m_vPosition);	// エフェクトを再生.
	m_IsEffectPlay = true;

}

// 当たり判定の設定.
bool CBarrier::ColliderSetting()
{
	if( m_pCollManager == nullptr ){
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	return true;
}