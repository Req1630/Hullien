#ifndef INVISIBLE_WALL_EDIT_H
#define INVISIBLE_WALL_EDIT_H

#include "..\EditBase.h"

class CInvisibleWallEdit : public CEditBase
{
public:
	CInvisibleWallEdit();
	virtual ~CInvisibleWallEdit();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;
};

#endif	// #ifndef INVISIBLE_WALL_EDIT_H.