#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/**********************************
*	�~�j�}�b�v�N���X.
**/
CMiniMap::CMiniMap()
	: m_pSprite					()
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
	if (SpriteSetting() == false ) return false;

	return true;
}

// �X�V�֐�.
void CMiniMap::Update()
{
}

// �I�u�W�F�N�g�ʒu�ݒ�֐�.
void CMiniMap::SetObjPosition(CGameActorManager* pObj)
{
	// �X�v���C�g�̐ݒ�.
	OBJLIST List = pObj->GetObjPositionList();
	SpriteSetting( List );

	// �A�C�R���ʒu�̍X�V.
	int objCount = 0;
	for (const auto& obj : pObj->GetObjPositionList())
	{
		objCount++;
		m_vPosition[objCount].x = m_pSprite[MAP_BACK]->GetRenderPos().x - obj.second.x;
		m_vPosition[objCount].y = m_pSprite[MAP_BACK]->GetRenderPos().y + obj.second.z;
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

// �X�v���C�g�ݒ�֐�.
bool CMiniMap::SpriteSetting()
{
	if ( m_pSprite.size() != 0 ) return true;

	// �}�b�v�w�i�̓ǂݍ���.
	m_pSprite.emplace_back( CSpriteResource::GetSprite(SPRITE_MAP_BACK) );
	m_vPosition.emplace_back( m_pSprite[MAP_BACK]->GetRenderPos() );
	if( m_pSprite[MAP_BACK] == nullptr ) return false;

	return true;
}

// �X�v���C�g�ݒ�֐�.
void CMiniMap::SpriteSetting(OBJLIST objList)
{
	if (m_ObjPosListCount > objList.size()) return;

	for (OBJLIST::const_iterator obj = objList.begin() + m_ObjPosListCount; obj < objList.end(); obj++)
	{
		m_ObjPosListCount++;

		switch (obj->first)
		{
		case EObjectTag::Player:
		case EObjectTag::Girl:
		case EObjectTag::Alien_B:
		case EObjectTag::Alien_A:
		case EObjectTag::Alien_C:
		case EObjectTag::Alien_D:
			ObjSpriteSetting( SPRITE_DEFAULT_ICON );
			break;

		default:
			break;
		}
	}
}

// �I�u�W�F�N�g�̃X�v���C�g�ݒ�֐�.
void CMiniMap::ObjSpriteSetting(const char* spriteName)
{
	// �ǂݍ��ރX�v���C�g�̐ݒ�.
	m_pSprite.emplace_back(CSpriteResource::GetSprite( spriteName ));
	// �����ʒu�̐ݒ�.
	m_vPosition.emplace_back( m_pSprite[MAP_BACK]->GetRenderPos() );
}

