#ifndef FOG_H
#define FOG_H

#include "..\..\Global.h"

class CFog
{
public:
	CFog();
	~CFog();

	static CFog* GetInstance()
	{
		static std::unique_ptr<CFog> pInstance = std::make_unique<CFog>();
		return pInstance.get();
	}

	// �X�V.
	static void Update();
	// �t�H�O�̃e�N�X�`�����W�擾.
	static D3DXVECTOR4 GetFogTex() { return GetInstance()->m_FogTex; }

private:
	D3DXVECTOR4 m_FogTex;	// �t�H�O�̃e�N�X�`�����W.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CFog( const CFog & )				= delete;
	CFog( CFog && )						= delete;
	CFog& operator = ( const CFog & )	= delete;
	CFog& operator = ( CFog && )		= delete;
};

#endif	// #ifndef FOG_H.