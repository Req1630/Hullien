#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

/**********************************
*	�~�j�}�b�v�N���X.
**/
CMiniMap::CMiniMap()
	: m_pSprite ()
{
}

CMiniMap::~CMiniMap()
{
}

// �������֐�.
bool CMiniMap::Init()
{
	// �X�v���C�g�ǂݍ���.
	if ( SpriteSetting() == false ) return false;
	return true;
}

// �X�V�֐�.
void CMiniMap::Update()
{
}

// �`��֐�.
void CMiniMap::Render()
{
	for ( const auto& s : m_pSprite )
	{
		s->SetDeprh( false );
		s->RenderUI();
		s->SetDeprh( true );
	}
}

// �X�v���C�g�ݒ�֐�.
bool CMiniMap::SpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	const char* spriteName[] = 
	{
		"minimapsize",
		"mapiconsize",
	};
	const int spriteMax = sizeof( spriteName ) / sizeof( spriteName[0] );

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back( CSpriteResource::GetSprite( spriteName[sprite] ) );
		if( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}
