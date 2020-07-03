#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "..\Actor.h"

class CExplosion : public CActor
{
public:
	// �����p�����[�^�[.
	struct stExplosionParam
	{
		float		ExplosionTime;		// ��������.
		float		AttackPower;		// ������.
		D3DXVECTOR3	SphereAdjPos;		// �X�t�B�A�̒����p���W.
		float		SphereAdjRadius;	// �X�t�B�A�̒����p���a.

		stExplosionParam()
			: ExplosionTime		( 0.0f )
			, AttackPower		( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
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

	// �p�����[�^�[�̐ݒ�.
	void SetExplosionParam( const SExplosionParam& param ){ m_Param = param; }

private:
	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// ���f���̎擾(�Ȃ��Ȃ��ăG�t�F�N�g�ɂȂ�).
	bool GetModel( const char* modelName );

private:
	SExplosionParam	m_Param;		// �����p�����[�^�[.
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
};

#endif	// #ifndef EXPLOSION_H.