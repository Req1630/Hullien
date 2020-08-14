#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"

/**********************************
*	�~�j�}�b�v�N���X.
**/
CMiniMap::CMiniMap()
	: m_IconList		()
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
	int objCount = 1;	// �}�b�v���n�͊܂߂Ȃ��̂Ŕz��ԍ���1����.
	for (const auto& obj : pObj->GetObjPositionList())
	{
		// �^�O���̍X�V.
		if (obj.first != m_IconList[objCount].EObjTag) m_IconList[objCount].EObjTag = obj.first;

		// None�Ȃ�Ώ������Ȃ�.
		if (obj.first == EObjectTag::None) continue;
		// �ʒu���̍X�V.
		m_IconList[objCount].Pos.x = m_IconList[MAP_BACK].pSprite->GetRenderPos().x - obj.second.x;
		m_IconList[objCount].Pos.y = m_IconList[MAP_BACK].pSprite->GetRenderPos().y + obj.second.z;
		objCount++;
	}
}

// �`��֐�.
void CMiniMap::Render()
{
	if (m_IconList.size() == 0) return;
	for (const auto& l : m_IconList)
	{
		// �^�O��None�Ȃ�Δ�\��.
		if (l.EObjTag == EObjectTag::None) continue;
		l.pSprite->SetPosition(l.Pos );
		l.pSprite->SetDeprh( false );
		l.pSprite->RenderUI();
		l.pSprite->SetDeprh( true );
	}
}

// �X�v���C�g�ݒ�֐�.
bool CMiniMap::SpriteSetting()
{
	if ( m_IconList.size() != 0 ) return true;

	// �}�b�v�w�i�̓ǂݍ���.
	m_IconList.emplace_back();
	m_IconList[MAP_BACK].pSprite = CSpriteResource::GetSprite(SPRITE_MAP_BACK);
	m_IconList[MAP_BACK].Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	if(m_IconList[MAP_BACK].pSprite == nullptr ) return false;

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
		case EObjectTag::SPEffectTimeItem:
		case EObjectTag::AttackUpItem:
		case EObjectTag::LifeRecoveryItem:
		case EObjectTag::MoveSpeedUpItem:
		case EObjectTag::None:
			ObjSpriteSetting(SPRITE_DEFAULT_ICON, obj->first);
			break;

		default:
			// �^�O��None�Ȃ�I�u�W�F�N�g�����݂��Ȃ��̂Ń��X�g�����炷.
			//m_IconList[m_ObjPosListCount] = m_IconList.back();
			//m_IconList.pop_back();
			break;
		}
	}
}

// �I�u�W�F�N�g�̃X�v���C�g�ݒ�֐�.
void CMiniMap::ObjSpriteSetting(const char* spriteName, const EObjectTag& tag)
{
	SIconInfo info;
	// �ǂݍ��ރX�v���C�g�̐ݒ�.
	info.pSprite = CSpriteResource::GetSprite(spriteName);
	// �����ʒu�̐ݒ�.
	info.Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	// �^�O�̐ݒ�.
	info.EObjTag = tag;
	// ���X�g�𑝂₷.
	m_IconList.emplace_back(info);
}

