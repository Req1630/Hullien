#include "Cursor.h"
#include "..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

/********************************************
*	カーソルクラス.
**/
CCursor::CCursor()
	:	CCursor( false )
{
}

CCursor::CCursor( const bool& isBlue )
	:	m_pSelectSprite(nullptr)
	,	m_vOldPosition(D3DXVECTOR3( 0.0f, 0.0f ,0.0f ))
	,	m_Acceleration( 0.0f )
	,	m_TargetWidth(0.0f)
	,	m_ScaleCount(0.0f)
	,	m_IsSetting(false)
	,	m_IsBlueSelect(isBlue)
{
	m_vScale.x = 0.0f;
}

CCursor::~CCursor()
{
}

// 初期化関数.
bool CCursor::Init()
{
	if ( SpriteSetting() == false ) return false;
	return true;
}

// 更新関数.
void CCursor::Update()
{
	if (m_pSprite == nullptr) return;
	MoveScale();	// スケールの動き.
}

// 描画関数.
void CCursor::Render()
{
	if (m_pSprite == nullptr) return;

	if( m_IsSetting == true ){
		m_vPosition.x -= m_TargetWidth*0.5f+ADJ_POSITOIN_X_MAIN;
		m_IsSetting = false;
	}
	m_pSprite->SetPosition( m_vPosition );
	m_pSprite->SetScale( m_vScale );

	m_pSprite->SetDeprh( false );
	m_pSprite->SetBlend( true );
	m_pSprite->RenderUI();
	m_pSprite->SetBlend( false );
	m_pSprite->SetDeprh( true );

	m_SlectPosition = m_vPosition;
	m_SlectPosition.x += ADJ_POSITOIN_X_SIDE;
	m_pSelectSprite->SetScale( {SCALE_MAX, m_vScale.y, SCALE_MAX} );
	m_pSelectSprite->SetPosition( m_SlectPosition );
	m_pSelectSprite->SetDeprh( false );
	m_pSelectSprite->SetBlend( true );
	m_pSelectSprite->RenderUI();
	m_pSelectSprite->SetBlend( false );
	m_pSelectSprite->SetDeprh( true );
}

// スプライト設定関数.
bool CCursor::SpriteSetting()
{
	const char* spriteName = m_IsBlueSelect == false ? SPRITE_BLACK_NAME : SPRITE_BLUE_NAME;
	const char* spriteName2 = m_IsBlueSelect == false ? SPRITE_BLACK_NAME2 : SPRITE_BLUE_NAME2;
	if (m_pSprite != nullptr) return true;
	m_pSprite = CSpriteResource::GetSprite(spriteName);
	if (m_pSprite == nullptr) return false;
	m_pSelectSprite = CSpriteResource::GetSprite(spriteName2);
	if (m_pSelectSprite == nullptr) return false;
	m_SlectPosition = m_pSelectSprite->GetRenderPos();
	return true;
}

// スケール動き関数.
void CCursor::MoveScale()
{
	// 移動前の座標と現在の座標が違う場合.
	if (m_vPosition != m_vOldPosition)
	{
		// 数値を初期化.
		m_vScale.x = 0.0f;
		m_vScale.y = SCALE_MAX;
		m_Acceleration = 0.0f;
		m_ScaleCount = 0.0f;
	}

	// 拡大値が標準ならば処理しない.
	if (m_vScale.x >= SCALE_MAX){
		m_vScale.y = SCALE_MAX + sinf(static_cast<float>(D3DX_PI*D3DXToRadian(m_ScaleCount)))*0.05f;
		m_ScaleCount++;
		if( m_ScaleCount >= 360.0f ) m_ScaleCount = 0.0f;
		return;
	}
	// 拡大.
	IncreaseScale();
}

// 拡大関数.
void CCursor::IncreaseScale()
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

