#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/**********************************
*	�~�j�}�b�v�N���X.
**/
CMiniMap::CMiniMap()
	: m_mView	()
	, m_mProj	()
	, m_pSprite ()
{
}

CMiniMap::~CMiniMap()
{
}

// �������֐�.
bool CMiniMap::Init()
{
	// �����X�v���C�g�ǂݍ���.
	if (InitSpriteSetting() == false ) return false;

	return true;
}

// �X�V�֐�.
void CMiniMap::Update()
{
}

void CMiniMap::SetPosition(CGameActorManager* pObj)
{
	m_pSprite.resize(pObj->GetObjPositionList().size());


	for (auto& obj : pObj->GetObjPositionList())
	{ 
		switch (obj.first)
		{
		case EObjectTag::Girl:
			m_pSprite.emplace_back(CSpriteResource::GetSprite("mapiconsize"));
			m_vPosition.x = m_pSprite[0]->GetRenderPos().x - obj.second.x;
			m_vPosition.y = m_pSprite[0]->GetRenderPos().y + obj.second.z;
			m_pSprite[m_pSprite.size() - 1]->SetPosition(m_vPosition);
			break;
		case EObjectTag::Player:
			m_pSprite.emplace_back(CSpriteResource::GetSprite("mapiconsize"));
			m_vPosition.x = m_pSprite[0]->GetRenderPos().x - obj.second.x;
			m_vPosition.y = m_pSprite[0]->GetRenderPos().y + obj.second.z;
			m_pSprite[m_pSprite.size() -1]->SetPosition(m_vPosition);
			break;
		default:
			break;
		}
	}
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

// �`��֐�.
void CMiniMap::Render(const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	m_mView = view;
	m_mProj = proj;

	Render();
}

// �X�v���C�g�ݒ�֐�.
bool CMiniMap::InitSpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	const char* spriteName[] = 
	{
		"minimapsize",
	};
	const int spriteMax = sizeof( spriteName ) / sizeof( spriteName[0] );

	for (int sprite = 0; sprite < spriteMax; sprite++)
	{
		m_pSprite.emplace_back( CSpriteResource::GetSprite( spriteName[sprite] ) );
		if( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}