#include "ButtonExplanation.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"

CButtonExp::CButtonExp()
	: CButtonExp	( DEFAULT_SPRITE_RENDER_POS_X, DEFAULT_SPRITE_RENDER_POS_Y )
{
}

CButtonExp::CButtonExp( const float& weight, const float& height )
	: SPRITE_RENDER_POS_X		( weight )
	, SPRITE_RENDER_POS_Y		( height )
	, m_pSprites				()
	, m_PositionList			()
	, m_SpriteSizeList			()
	, m_IsBackString			( false )
	, m_IsCancelStringRender	( true )
{
}

CButtonExp::~CButtonExp()
{
}

// 初期化関数.
bool CButtonExp::Init()
{
	if( SpriteSetting() == false ) return false;

	return true;
}

// 更新関数.
void CButtonExp::Update()
{
}

// 描画関数.
void CButtonExp::Render()
{
	const int spriteEndNo = m_IsCancelStringRender == true ? ESpriteNo_End : ESpriteNo_A;

	for( int i = ESpriteNo_Begin; i < spriteEndNo; i++ ){
		D3DXVECTOR3 renderPos = m_PositionList[i];

		if( m_IsCancelStringRender == false ) renderPos.x += SPRITE_RENDER_WIDTH;
		m_pSprites[i]->SetPosition( renderPos );
		m_pSprites[i]->SetScale( m_SpriteSizeList[i] );
		m_pSprites[i]->SetDeprh( false );
		m_pSprites[i]->SetBlend( true );
		m_pSprites[i]->RenderUI();
		m_pSprites[i]->SetBlend( false );
		m_pSprites[i]->SetDeprh( true );
	}
}

// スプライト設定関数.
bool CButtonExp::SpriteSetting()
{
	const char* spriteName[] =
	{
		SPRITE_L_STICK_NAME,
		SPRITE_SELECT_NAME,
		SPRITE_B_BUTTON_NAME,
		SPRITE_DETERMINATION_NAME,
		SPRITE_A_BUTTON_NAME,
		m_IsBackString == true ? SPRITE_BACK_NAME : SPRITE_CANCEL_NAME,
	};
	int SpriteMax = sizeof(spriteName) / sizeof(spriteName[0]);

	const float positionXList[] =
	{
		SPRITE_RENDER_POS_X+L_STICK_SPRITE_RENDER_POS_X,
		SPRITE_RENDER_POS_X+SELECT_SPRITE_RENDER_POS_X,
		SPRITE_RENDER_POS_X+B_SPRITE_RENDER_POS_X,
		SPRITE_RENDER_POS_X+DET_SPRITE_RENDER_POS_X,
		SPRITE_RENDER_POS_X+A_SPRITE_RENDER_POS_X,
		SPRITE_RENDER_POS_X+CANCEL_SPRITE_RENDER_POS_X,
	};

	const float spriteSizeList[] =
	{
		BUTTON_SPRITE_SIZE,
		CHAR_SPRITE_SIZE,
		BUTTON_SPRITE_SIZE,
		CHAR_SPRITE_SIZE,
		BUTTON_SPRITE_SIZE,
		CHAR_SPRITE_SIZE,
	};

	// メモリの最大値設定.
	m_pSprites.reserve(SpriteMax);
	m_PositionList.reserve(SpriteMax);
	for( int sprite = 0; sprite < SpriteMax; sprite++ ){
		m_pSprites.emplace_back(CSpriteResource::GetSprite(spriteName[sprite]));
		if( m_pSprites[sprite] == nullptr ) return false;
		m_PositionList.emplace_back( D3DXVECTOR3( positionXList[sprite], SPRITE_RENDER_POS_Y, 0.0f ) );
		m_SpriteSizeList.emplace_back( spriteSizeList[sprite] );
	}
	return true;
}