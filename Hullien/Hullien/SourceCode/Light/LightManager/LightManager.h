#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "..\..\Global.h"

class CLightBase;

class CLightManager
{
public:
	CLightManager();
	~CLightManager();

	// ���C�g�N���X�̐ݒ�.
	static void SetLgiht( std::shared_ptr<CLightBase> pLight );

	// �J�X�P�[�h���̃V���h�E�s����擾����.
	static D3DXMATRIX* GetShadowVP();
	// �J�X�P�[�h�������W���擾.
	static float* GetSpritPos();

	// ���W�̎擾.
	static D3DXVECTOR3 GetPosition();
	// �����̎擾.
	static D3DXVECTOR3 GetDirection();
	// ��]�s��̎擾.
	static D3DXMATRIX GetRorarionMatrix();
	// �����̎擾.
	static float GetIntensity();

private:
	std::shared_ptr<CLightBase> m_pLight;

private:
	// �C���X�^���X�̎擾.
	static CLightManager* GetInstance();
};

#endif	// #ifndef LIGHT_MANAGER_H.