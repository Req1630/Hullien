#ifndef ITEM_H
#define ITEM_H

#include <unordered_map>
#include "..\..\..\Global.h"

// �A�C�e�����X�g.
enum  class enItemList
{
	None,

	LifeRecovery,	// �̗͉�.
	SPEffectTime,	// ����\�͉񕜑��xUP.
	AttackUp,		// �U����UP.
	MoveSpeedUp,	// �ړ����xUP.

	Max,

} typedef EItemList;

typedef std::unordered_map<EItemList, D3DXVECTOR3>	DropItemList;	// ���Ƃ��A�C�e���̃��X�g.


#endif	// #ifndef ITEM_H.