#ifndef MOTHER_SHIP_UFO_H
#define MOTHER_SHIP_UFO_H

#include "..\GameObject.h"

class CActor;
class CCollisionManager;	// �����蔻��N���X.

class CMotherShipUFO : public CGameObject
{
public:
	// �}�U�[�V�b�v�̃p�����[�^.
	struct stMotherShipUFOParam
	{
		D3DXVECTOR3 Position;			// ���W.
		float		AddPosYPower;		// �����グ���.
		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		CollisionRadius;	// �����蔻��̔��a.

		stMotherShipUFOParam()
			: Position			( 0.0f, 0.0f, 0.0f )
			, AddPosYPower		( 0.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, CollisionRadius	( 1.0f )
		{}

	} typedef SMotherShipUFOParam;

private:
	const char* MODEL_NAME = "ufo";	// ���f����.
	const D3DXVECTOR3 INIT_POS = { 30.0f, 10.0f, 0.0f };
	const float COLLISION_RADIUS = 5.0f;
	const float ADD_POS_POWER = 0.1f;
//	const SMotherShipUFOParam*	pPRAMETER;	// �p�����[�^(�O������|�C���^�Ŏ󂯎��).

public:
	CMotherShipUFO();
	virtual ~CMotherShipUFO();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��.
	void Collision( CActor* pActor );
	// �p�����[�^�̐ݒ�.
	void SetParameter( const SMotherShipUFOParam& param );

private:
	// ���f���̎擾.
	bool GetModel();
	// �����蔻��̐ݒ�.
	bool CollisionSetting();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// �X�^�e�B�b�N���b�V��.
	std::shared_ptr<CCollisionManager>	m_pCollManager;	// �����蔻��N���X.
	SMotherShipUFOParam	m_Param;
};

#endif	// #ifndef MOTHER_SHIP_UFO_H.