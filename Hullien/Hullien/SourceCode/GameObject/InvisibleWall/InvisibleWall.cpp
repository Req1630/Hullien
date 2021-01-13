#include "InvisibleWall.h"
#include "..\..\Collider\CollsionManager\CollsionManager.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Common\InvisibleWall\InvisibleWallShader.h"

CInvisibleWall::CInvisibleWall()
	: m_BoxWall					()
	, m_pInvisibleWallShader	( nullptr )
#ifdef _DEBUG
	, m_pCollision				( nullptr )
#endif	// #ifdef _DEBUG.
{
	m_vPosition.y = POSITION_HEIGHT;
	m_pInvisibleWallShader = std::make_unique<CInvisibleWallShader>();
}

CInvisibleWall::~CInvisibleWall()
{
}

// 初期化関数.
bool CInvisibleWall::Init()
{
	if( InitParam()		== false ) return false;
	if( InitCollision()	== false ) return false;
	const float ADJ_LENGHT = 2.0f;
	if( FAILED( m_pInvisibleWallShader->Init( 
		CDirectX11::GetDevice(),
		CDirectX11::GetContext(),
		D3DXVECTOR3(m_BoxWall.MaxPosition.x+ADJ_LENGHT, m_BoxWall.Length.y, m_BoxWall.MaxPosition.y+ADJ_LENGHT),
		D3DXVECTOR3(m_BoxWall.MaxPosition.x+ADJ_LENGHT, m_BoxWall.Length.y, m_BoxWall.MaxPosition.y+ADJ_LENGHT) ))) return false;
	return true;
}

// 更新関数.
void CInvisibleWall::Update()
{
}

// 描画関数.
void CInvisibleWall::Render()
{
	m_pInvisibleWallShader->SetPlayerPos( {m_PlayerPosition.x, m_PlayerPosition.y+5.0f, m_PlayerPosition.z} );
	m_pInvisibleWallShader->Render();
#ifdef _DEBUG
	if( m_pCollision == nullptr ) return;
	m_pCollision->DebugRender();
#endif	// #ifdef _DEBUG.
}

// ボックスの設定.
void CInvisibleWall::SetBoxWall( const SBoxWall& boxWall )
{ 
	m_BoxWall = boxWall;
	// ボックスの半分の値を最大、最小として計算.
	m_BoxWall.MaxPosition = { m_BoxWall.Length.x * HALF, m_BoxWall.Length.z * HALF };
	m_BoxWall.MinPosition = { m_BoxWall.Length.x *-HALF, m_BoxWall.Length.z *-HALF };
	InitCollision();
}

// 座標設定関数.
void CInvisibleWall::SetPosition( const D3DXVECTOR3& vPos )
{ 
	m_PlayerPosition = vPos;
}

// パラメータの初期化.
bool CInvisibleWall::InitParam()
{
	if( CFileManager::BinaryReading( INVISIBLE_WALL_PARAM_FILE_NAME, m_BoxWall ) == false ) return false;

	// ボックスの半分の値を最大、最小として計算.
	m_BoxWall.MaxPosition = { m_BoxWall.Length.x * HALF, m_BoxWall.Length.z * HALF };
	m_BoxWall.MinPosition = { m_BoxWall.Length.x *-HALF, m_BoxWall.Length.z *-HALF };
	return true;
}

// 当たり判定の初期化.
bool CInvisibleWall::InitCollision()
{
#ifdef _DEBUG
	if( m_pCollision == nullptr ){
		m_pCollision = std::make_unique<CCollisionManager>();
	}
	if( FAILED( m_pCollision->InitBox(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{ 0.0f, 0.0f, 0.0f },
		m_BoxWall.Length ))) return false;
#endif	// #ifdef _DEBUG.
	return true;
}