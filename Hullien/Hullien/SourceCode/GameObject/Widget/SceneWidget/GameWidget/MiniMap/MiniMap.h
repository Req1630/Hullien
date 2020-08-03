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
public:
	CMiniMap();
	virtual ~CMiniMap();

	// �������֐�.
	virtual bool Init() override;

	// �X�V�֐�.
	virtual void Update() override;
	void SetPosition(CGameActorManager* pObj);

	// �`��֐�.
	virtual void Render() override;
	void Render(const D3DXMATRIX& view, const D3DXMATRIX& proj);

private:
	// �����X�v���C�g�ݒ�֐�.
	bool InitSpriteSetting();

private:
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;

	std::vector<std::shared_ptr<CSprite>> m_pSprite;					
};

#endif	//#ifndef MINIMAP_H.
