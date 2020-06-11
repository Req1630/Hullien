#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"

class CCharacter : public CActor
{
protected:
	// �L�����N�^�[�p�����[�^.
	struct stParameter
	{
		float	Life;		// �̗�.
		float	MoveSpeed;	// �ړ����x.

		stParameter()
			: Life		( 1.0f )
			, MoveSpeed	( 0.5f )
		{}
	} typedef SParameter;

public:
	CCharacter();
	virtual ~CCharacter();

	// �ړ��֐�.
	virtual void Move() = 0;

protected:
	// ���f���̎擾.
	bool GetModel( const char* modelName );
	// �p�����[�^�̐ݒ�.
	template<class inParam>
	bool ParameterSetting( const char* fileName, inParam& param )
	{
		// ���Ƀp�����[�^���ݒ�ς݂Ȃ�I��.
		if( m_HasFinishedParamSetting == true ) return true;

		// �ǂݍ��݂�����ɍs���ĂȂ���ΏI��.
		if( CFileManager::BinaryReading( fileName, param ) == false ) return false;

		m_HasFinishedParamSetting = true;
		return true;
	}

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// �X�L�����b�V��.
	D3DXVECTOR3	m_MoveVector;	// �ړ��x�N�g��.

	bool m_HasFinishedParamSetting;	// �p�����[�^�[�̐ݒ肪�I�������.
};

#endif	// #ifndef CHARACTER_H.