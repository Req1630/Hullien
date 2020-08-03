#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\GameWidget.h"
#include <vector>

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
	// �`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	bool SpriteSetting();

private:
	std::vector<std::shared_ptr<CSprite>> m_pSprite;	//�X�v���C�g�N���X.


};

#endif	//#ifndef MINIMAP_H.
