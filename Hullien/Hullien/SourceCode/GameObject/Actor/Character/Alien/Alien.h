#ifndef ALIEN_H
#define ALIEN_H

#include "..\Character.h"

class CAlien : public CCharacter
{
	const float ROTATIONAL_SPEED = 0.05f;	// ��]���x.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));
public:
	CAlien();
	virtual ~CAlien();

	// ������W�̐ݒ�.
	virtual void SetOpponentPos( CActor& actor ) override;
protected:
	// �ړ��x�N�g���ݒ�֐�.
	void SetMoveVector( const D3DXVECTOR3& targetPos );
	// �ړI�̍��W�։�].
	void TargetRotation();

protected:
	D3DXVECTOR3	m_GirlPosition;		// ���̎q�̍��W.
	D3DXVECTOR3	m_TargetRotation;	// �ڕW�̉�]���.
};

#endif	// #ifndef ALIEN_H.