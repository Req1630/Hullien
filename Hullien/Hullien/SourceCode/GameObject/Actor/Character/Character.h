#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"
#include "..\Item\ItemList.h"

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

protected:
	float	m_Life;			// �̗�.
	float	m_MoveSpeed;	// �ړ����x.
	int		m_HasAnyItem;	// �ǂ̃A�C�e���������Ă��邩.
};

#endif	// #ifndef CHARACTER_H.