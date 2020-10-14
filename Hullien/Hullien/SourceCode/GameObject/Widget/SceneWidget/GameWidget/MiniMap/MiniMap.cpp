#include "MiniMap.h"

#include "..\..\..\..\..\Common\Sprite\CSprite.h"
#include "..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\Actor\ActorManager\GameActorManager.h"
#include "..\..\..\..\..\Common\DebugText\DebugText.h"

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
	auto List = pObj->GetObjPositionList();
	SpriteSetting( List );

	// �A�C�R���ʒu�̍X�V.
	int objCount = 1;	// �}�b�v���n�͊܂߂Ȃ��̂Ŕz��ԍ���1����.
	for (const auto& obj : pObj->GetObjPositionList())
	{
		// None�Ȃ�Ώ������Ȃ�.
		if (obj.first == EObjectTag::None) continue;
		// �^�O���̍X�V.
		if (obj.first != m_IconList[objCount].EObjTag) 
		{ 
			m_IconList[objCount].EObjTag = obj.first; 
			m_IconList[objCount].AnimNumber = SetAnimNumber(obj.first);
		}

		// �ʒu���̍X�V.
		if (m_IconList[objCount].Pos == obj.second) return;
		m_IconList[objCount].Pos.x = m_IconList[MAP_BACK].pSprite->GetRenderPos().x - (obj.second.x * 0.25f);
		m_IconList[objCount].Pos.y = m_IconList[MAP_BACK].pSprite->GetRenderPos().y + (obj.second.z * 0.25f);
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
		l.pSprite->SetAnimNumber(l.AnimNumber );
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
	if(m_ObjPosListCount == objList.size()) return;
	// �J�E���g���I�u�W�F�N�g���𒴂�����.
	// �I�u�W�F�N�g�����܂Ō��炷.
	if (m_ObjPosListCount > objList.size())
	{
		m_ObjPosListCount = objList.size();
		m_IconList.resize(objList.size());
		m_IconList.shrink_to_fit();
		return;
	}

	// ���X�g�𑝂₷.
	for (auto obj = objList.begin() + m_ObjPosListCount; obj < objList.end(); obj++)
	{
		m_ObjPosListCount++;
		ObjSpriteSetting(SPRITE_DEFAULT_ICON, obj->first);
	}
}

// �I�u�W�F�N�g�̃X�v���C�g�ݒ�֐�.
void CMiniMap::ObjSpriteSetting(const char* spriteName, const EObjectTag& tag)
{
	SIconInfo info;
	// �ǂݍ��ރX�v���C�g�̐ݒ�.
	info.pSprite = CSpriteResource::GetSprite( spriteName );
	// �����ʒu�̐ݒ�.
	info.Pos = m_IconList[MAP_BACK].pSprite->GetRenderPos();
	// �^�O�̐ݒ�.
	info.EObjTag = tag;
	// �A�j���[�V�����ԍ��̐ݒ�.
	info.AnimNumber = SetAnimNumber( tag );
	// ���X�g�𑝂₷.
	m_IconList.emplace_back( info );
}

// �A�j���[�V�����ԍ��̐ݒ�֐�.
int CMiniMap::SetAnimNumber(const EObjectTag& tag)
{
	int animNumber = 0;
	switch (tag)
	{
	case EObjectTag::Player:
		animNumber = 0;
		break;
	case EObjectTag::Girl:
		animNumber = 1;
		break;
	case EObjectTag::Alien_A:
	case EObjectTag::Alien_B:
	case EObjectTag::Alien_C:
		animNumber = 2;
		break;
	case EObjectTag::Alien_D:
		animNumber = 3;
		break;
	case EObjectTag::SPEffectTimeItem:
	case EObjectTag::AttackUpItem:
	case EObjectTag::LifeRecoveryItem:
	case EObjectTag::MoveSpeedUpItem:
		animNumber = 4;
		break;

	default:
		break;
	}

	return animNumber;
}

