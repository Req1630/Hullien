#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/**********************************
*	�~�j�}�b�v�N���X.
**/
CMiniMap::CMiniMap()
	: m_mView					()
	, m_mProj					()
	, m_pSprite					()
	, m_ObjPosListCount	( 0 )

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
	SpriteSetting( pObj );

	for (const auto& obj : pObj->GetObjPositionList())
	{
		switch (obj.first)
		{
		case EObjectTag::Player:
			m_vPosition[1].x = m_pSprite[0]->GetRenderPos().x - obj.second.x;
			m_vPosition[1].y = m_pSprite[0]->GetRenderPos().y + obj.second.z;
			break;
		case EObjectTag::Girl:
			m_vPosition[2].x = m_pSprite[0]->GetRenderPos().x - obj.second.x;
			m_vPosition[2].y = m_pSprite[0]->GetRenderPos().y + obj.second.z;
			break;
		case EObjectTag::Alien_B:
		case EObjectTag::Alien_A:
			break;
		default:
			break;
		}
	}
}

// �`��֐�.
void CMiniMap::Render()
{
	for ( size_t sprite = 0; sprite < m_pSprite.size(); sprite++ )
	{
		m_pSprite[sprite]->SetPosition( m_vPosition[sprite] );
		m_pSprite[sprite]->SetDeprh( false );
		m_pSprite[sprite]->RenderUI();
		m_pSprite[sprite]->SetDeprh( true );
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
		m_vPosition.emplace_back( m_pSprite[sprite]->GetRenderPos() );
		if( m_pSprite[sprite] == nullptr ) return false;
	}

	return true;
}

// �X�v���C�g�ݒ�֐�.
void CMiniMap::SpriteSetting(CGameActorManager * pObj)
{
	std::vector<std::pair<EObjectTag, D3DXVECTOR3>> v = pObj->GetObjPositionList();

	if (m_ObjPosListCount > pObj->GetObjPositionList().size()) return;

	for (std::vector<std::pair<EObjectTag, D3DXVECTOR3>>::const_iterator obj = v.begin() + m_ObjPosListCount; obj < v.end(); obj++)
	{
		m_ObjPosListCount++;
		switch (obj->first)
		{
		case EObjectTag::Player:
			m_pSprite.emplace_back(CSpriteResource::GetSprite("mapiconsize"));
			m_vPosition.emplace_back(D3DXVECTOR3(
				m_pSprite[0]->GetRenderPos().x - obj->second.x,
				m_pSprite[0]->GetRenderPos().y + obj->second.z,
				0.0f
			));
			break;
		case EObjectTag::Girl:
			m_pSprite.emplace_back(CSpriteResource::GetSprite("mapiconsize"));
			m_vPosition.emplace_back(D3DXVECTOR3(
				m_pSprite[0]->GetRenderPos().x - obj->second.x,
				m_pSprite[0]->GetRenderPos().y + obj->second.z,
				0.0f
			));
			break;
		case EObjectTag::Alien_B:
		case EObjectTag::Alien_A:
			break;

		default:
			break;
		}
	}
}
