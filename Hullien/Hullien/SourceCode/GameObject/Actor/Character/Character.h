#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter();

	// �ړ��֐�.
	virtual void Move() = 0;

protected:
	float		m_Life;			// �̗�.
	D3DXVECTOR3	m_MoveVector;	// �ړ��x�N�g��.
};

#endif	// #ifndef CHARACTER_H.