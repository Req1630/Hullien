#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"

class CExplosion : public CActor
{
public:
	// �����p�����[�^�[.
	struct stExplosionParam
	{
		float		ExplosionSpeed;		// �������x.
		float		AttackPower;		// ������.
		float		SphereMaxRadius;	// �X�t�B�A�̍ő唼�a.
		D3DXVECTOR3	SphereAdjPos;		// �X�t�B�A�̒����p���W.

		stExplosionParam()
			: ExplosionSpeed	( 0.0f )
			, AttackPower		( 0.0f )
			, SphereMaxRadius	( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		{}
	} typedef SExplosionParam;


public:
	CExplosion();
	virtual ~CExplosion();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& pActor ) override;

	// �p�����[�^�[�̐ݒ�.
	void SetExplosionParam( const SExplosionParam& param ){ m_Param = param; }

private:
	// �����蔻��̐ݒ�.
	bool ColliderSetting();

private:
	SExplosionParam	m_Param;	// �����p�����[�^�[.
	float m_CollSphereRadius;	// �����蔻��̔��a.
#if _DEBUG
	float m_ResizeCollTime;		// �����蔻��̔��a�̃��T�C�Y���鎞��(�f�o�b�O�p).
#endif	// #if _DEBUG.
};

#endif	// #ifndef EXPLOSION_H.