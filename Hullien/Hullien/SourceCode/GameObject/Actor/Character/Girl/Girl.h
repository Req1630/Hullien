#ifndef GIRL_H
#define GIRL_H

#include "..\Character.h"

class CGirl : public CCharacter
{
	const char* MODEL_NAME = "Towa_s";

	// �v���C���[�p�����[�^.
	struct stGirlParam : public stParameter
	{
		D3DXVECTOR3 SphereAdjPos;	// �X�t�B�A�̒������W.
		float		SphereAdjRadius;			// �X�t�B�A�̒������a.

		stGirlParam()
			: SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.7f )
		{}
	} typedef SGirlParam;
public:
	CGirl();
	~CGirl();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision(CActor* pActor) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos(CActor& actor) override;

private:
	// �ړ��֐�.
	virtual void Move() override;
	// �����蔻��̍쐬.
	bool  ColliderSetting();

private:
	SGirlParam m_Parameter;
};

#endif	// #ifndef GIRL_H.