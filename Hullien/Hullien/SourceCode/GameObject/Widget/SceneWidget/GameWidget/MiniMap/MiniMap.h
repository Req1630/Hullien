#ifndef MINIMAP_H
#define MINIMAP_H

#include "..\GameWidget.h"

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
	//�`��֐�.
	virtual void Render() override;

private:
};

#endif	//#ifndef MINIMAP_H.
