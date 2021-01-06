#include "ConfigCursor.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Sprite\BlendSprite\BlendSprite.h"

/********************************************
*	カーソルクラス.
**/
CConfigCursor::CConfigCursor()
	:	m_pBlendSprite(nullptr)
	,	m_vOldPosition(D3DXVECTOR3( 0.0f, 0.0f ,0.0f ))
	,	m_Acceleration( 0.0f )
	,	m_TargetWidth(0.0f)
	,	m_IsSetting(false)
{
	m_pBlendSprite = std::make_unique<CBlendSprite>();
}

CConfigCursor::~CConfigCursor()
{
}

// 初期化関数.
bool CConfigCursor::Init()
{
	if ( SpriteSetting() == false ) return false;
	if( FAILED( m_pBlendSprite->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() ))) return false;
	m_pBlendSprite->SetSpriteData( m_pSprite->GetSpriteData() );
	return true;
}

// 更新関数.
void CConfigCursor::Update()
{
	if (m_pSprite == nullptr) return;
//	MoveScale();	// スケールの動き.
}

// 描画関数.
void CConfigCursor::Render()
{
	if (m_pSprite == nullptr) return;

	m_vPosition.x += ADJ_POSITOIN_X_MAIN;
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetScale( m_vScale );

	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );
}

// スプライト設定関数.
bool CConfigCursor::SpriteSetting()
{
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(SPRITE_NAME);
	if (m_pSprite == nullptr) return false;
	return true;
}

// スケール動き関数.
void CConfigCursor::MoveScale()
{
	// 移動前の座標と現在の座標が違う場合.
	if (m_vPosition != m_vOldPosition)
	{
		// 数値を初期化.
		m_vScale.x = 0.0f;
		m_Acceleration = 0.0f;
	}

	// 拡大値が標準ならば処理しない.
	if (m_vScale.x >= SCALE_MAX) return;
	// 拡大.
	IncreaseScale();
}

// 拡大関数.
void CConfigCursor::IncreaseScale()
{
	m_vOldPosition = m_vPosition;	// 現在地を保持する.

	m_vScale.x += SCALE_SPEED - m_Acceleration;
	m_Acceleration += ACC_SPEED;

	// 拡大値が標準値になった時.
	if ( m_vScale.x >= SCALE_MAX)
	{
		m_vScale.x = SCALE_MAX;
		m_Acceleration = 0.0f;	// 加速度の初期化.
	}
}
