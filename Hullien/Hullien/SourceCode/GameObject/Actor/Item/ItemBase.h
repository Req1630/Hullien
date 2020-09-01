#ifndef ITEM_BASE_H
#define ITEM_BASE_H

#include "..\Actor.h"

class CItemBase : public CActor
{
	const float INIT_MODEL_ALPHA			= 1.0f;		// �������f�����ߒl.
	const float DEFAULT_ACCELERATION_VALUE	= -0.05f;	// �f�t�H���g�����l.
	const float DEFAULT_GRAVITY				= 0.001f;	// �f�t�H���g�d��.
public:
	// �p�����[�^.
	struct stParameter
	{
		float	InitAccelerationValue;	// ���������l.
		float	InitGravity;			// �����d��.

		int		BoundCountMax;			// �o�E���h�ő吔.
		float	FinalRenderHeight;		// �ŏI�I�ȕ`��̍���.
		float	FinalMoveSpeed;			// �ŏI�I�Ȉړ����x.
		float	ModelScaleMax;			// ���f���T�C�Y�ő�.
		float	ModelScaleAddValue;		// ���f���T�C�Y���Z�l.
		float	RotationSpeed;			// ��]���x.
		float	ActiveTime;				// �A�N�e�B�u����.
		float	DisappearTime;			// �����鎞��.
		float	FlashingAddValue;		// �_�ŉ��Z�l.
		float	ModelAlphaMax;			// ���f�����ߒl�ő�.
		stParameter()
			: InitAccelerationValue	( 0.0f )
			, InitGravity			( 0.0f )
			, BoundCountMax			( 0 )
			, FinalRenderHeight		( 0.0f )
			, FinalMoveSpeed		( 0.0f )
			, ModelScaleMax			( 0.0f )
			, ModelScaleAddValue	( 0.0f )
			, RotationSpeed			( 0.0f )
			, ActiveTime			( 0.0f )
			, DisappearTime			( 0.0f )
			, FlashingAddValue		( 0.0f )
			, ModelAlphaMax			( 0.0f )
		{}
	} typedef SParameter;

private:
	const SParameter* pPRAMETER;	// �p�����[�^(�O������|�C���^�Ŏ擾����).

protected:
	// ���݂̏��.
	enum class enNowState
	{
		None,

		Drop,			// �o��.
		Active,			// �A�N�e�B�u.
		HitDisappear,	// ���������ۂɏ�����.
		TimeDisappear,	// ���Ԃ��o�������ɏ�����.
		Delete,			// ������.

		Max,
	} typedef ENowState;

public:
	CItemBase();
	CItemBase( const SParameter* pParam );
	virtual ~CItemBase();

	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// �o������.
	void Drop( const D3DXVECTOR3& vPos );

	// ���������ǂ���.
	bool IsDelete(){ return m_NowState == ENowState::Delete; }

protected:
	// ���������ۂ̌��ʂ�^����.
	virtual void GiveEffect( CActor* pActor ) = 0;
	// �o��.
	void Drop();
	// �A�N�e�B�u.
	void Active();
	// ���������ۂɏ����鏈��.
	void HitDisappear();
	// ���Ԃ��o�������ɏ�����.
	void TimeDisappear();

	// ��]�֐�.
	void Rotate();

	// ���f���̎擾.
	bool GetModel( const char* modelName );
	// �����蔻��̐ݒ�.
	bool ColliderSetting();

protected:
	std::shared_ptr<CDX9StaticMesh> m_pStaticMesh;	// �X�^�e�B�b�N���b�V��.
	ENowState m_NowState;	// ���݂̏��.
	float m_Scale;
	float m_ModelAlpha;

	float m_AccelerationValue;	// �����l.
	float m_Gravity;			// �d��.
	int m_BoundCount;			// �o�E���h��.

	int m_ActiveCount;		// �A�N�e�B�u�J�E���g.

	float m_FlashingCount;	// �_�ŃJ�E���g.
	float m_DisappearCount;	// �����鎞��.
	float m_FlashingAccValue;	// �_�ŉ����l.
};

#endif	// #ifndef ITEM_BASE_H.