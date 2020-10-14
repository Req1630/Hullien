#ifndef BOX_STRUCT_H
#define BOX_STRUCT_H

#include "..\..\Global.h"

// �{�b�N�X�̍\����.
struct stBoxWall
{
	D3DXVECTOR3 Length;			// �{�b�N�X�̒���x,y,z�̐�Βl.
	D3DXVECTOR2	MaxPosition;	// �ő���W : y��z���W�Ƃ��Ďg�p.
	D3DXVECTOR2	MinPosition;	// �ŏ����W : y��z���W�Ƃ��Ďg�p.
	stBoxWall()
		: Length		( 0.0f, 0.0f, 0.0f )
		, MaxPosition	( 0.0f, 0.0f )
		, MinPosition	( 0.0f, 0.0f )
	{}
} typedef SBoxWall;

#endif	// #ifndef BOX_STRUCT_H.