#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\GameWidget.h"
#include <vector>

class CGameActorManager;	//�A�N�^�Ǘ��N���X.

/**********************************
*	�~�j�}�b�v�N���X.
**/
class CMiniMap : public CGameWidget
{
private:
	typedef std::vector<std::pair<EObjectTag, D3DXVECTOR3>> OBJLIST;

	const char* SPRITE_MAP_BACK = "minimapsize";		//�~�j�}�b�v�w�i�X�v���C�g��.
	const char* SPRITE_DEFAULT_ICON = "mapiconsize";	//�W���A�C�R���X�v���C�g��.
	const int	 MAP_BACK = 0;										//�~�j�}�b�v�w�i�z��ԍ�.

public:
	CMiniMap();
	virtual ~CMiniMap();

	// �������֐�.
	virtual bool Init() override;

	// �X�V�֐�.
	virtual void Update() override;
	// �I�u�W�F�N�g�ʒu�ݒ�֐�.
	void SetObjPosition(CGameActorManager* pObj);

	// �`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;
	//�X�v���C�g�ݒ�֐�.
	void SpriteSetting(OBJLIST objList);
	// �I�u�W�F�N�g�̃X�v���C�g�ݒ�֐�.
	void ObjSpriteSetting(const char* spriteName);

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;
	std::vector<D3DXVECTOR3>	m_vPosition;
	size_t m_ObjPosListCount;

};

#endif	//#ifndef MINIMAP_H.
