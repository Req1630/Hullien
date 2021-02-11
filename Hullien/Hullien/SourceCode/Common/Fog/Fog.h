/**
* @file Fog.h.
* @brief �t�H�O�̏��������Ă���.
* @author ���c���.
*/
#ifndef FOG_H
#define FOG_H

#include "..\..\Global.h"

//------------------------------.
// �t�H�O�̍\����.
//------------------------------.
// �t�H�O�̏�� : 16byte�ɂ��邽�߂ɒ����ϐ��ǉ�.
struct stFogState
{
	float MinHeight;	// �t�H�O�̍ŏ�����.
	float MaxHeight;	// �t�H�O�̍ő卂��.
	float TexScale;		// �t�H�O�̃e�N�Z���X�P�[���l.
	stFogState()
		: MinHeight	( 0.0f )
		, MaxHeight ( 10.0f )
		, TexScale	( 0.03f )
	{}
private:
	float MemoryAdj;	// �����������p 16byte�ɐݒ肷�邽��.
} typedef SFogState;

// �t�H�O�̏�� : �R���X�^���g�o�b�t�@�ɓn���悤.
struct stFog
{
	D3DXVECTOR4 Tex;	// �t�H�O�̃e�N�X�`�����W.
	D3DXVECTOR4 Color;	// �t�H�O�̐F.
	SFogState	State;	// �t�H�O�̃X�e�[�^�X.

	stFog()
		: Tex	{ 0.0f, 0.0f, 0.5f, 0.5f }
		, Color	{ 1.0f, 1.0f, 1.0f, 1.0f }
		, State	()
	{}
} typedef SFog;

class CFog
{
public:
	CFog();
	~CFog();

	// �C���X�^���X�̎擾.
	static CFog* GetInstance();

	// �X�V.
	static void Update();
	// �t�H�O�̃e�N�X�`�����W�擾.
	static SFog GetFog() { return GetInstance()->m_Fog; }

private:
	SFog m_Fog;	// �t�H�O�̃e�N�X�`�����W.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CFog( const CFog & )				= delete;
	CFog( CFog && )						= delete;
	CFog& operator = ( const CFog & )	= delete;
	CFog& operator = ( CFog && )		= delete;
};

#endif	// #ifndef FOG_H.