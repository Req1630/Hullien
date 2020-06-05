#ifndef LIGHT_BASE_H
#define LIGHT_BASE_H

#include "..\Global.h"

class CLightBase
{
public:
	CLightBase();
	virtual ~CLightBase();


	// ���W�̐ݒ�.
	void SetPosition( const D3DXVECTOR3& pos ){ m_vPosition = pos; }
	// ���W�̎擾.
	D3DXVECTOR3 GetPosition() const { return m_vPosition; }

	// �����̐ݒ�.
	void SetDirection( const D3DXVECTOR3& dir ){ m_vDirection = dir; }
	// �����̎擾.
	D3DXVECTOR3 GetDirection() const { return m_vDirection; }

	// ��]�s��̐ݒ�.
	void SetRorarionMatrix( const D3DXMATRIX& rot ){ m_mRotation = rot; }
	// ��]�s��̎擾.
	D3DXMATRIX GetRorarionMatrix() const { return m_mRotation; }

	// �����̐ݒ�.
	void SetIntensity( const float& intensity ){ m_fIntensity = intensity; }
	// �����̎擾.
	float GetIntensity() const { return m_fIntensity; }
	
protected:
	D3DXVECTOR3	m_vPosition;	// �ʒu.
	D3DXVECTOR3	m_vDirection;	// ����.
	D3DXMATRIX	m_mRotation;	// ��]�s��.
	float		m_fIntensity;	// ���x(���邳).
};

#endif	// #ifndef LIGHT_BASE_H.