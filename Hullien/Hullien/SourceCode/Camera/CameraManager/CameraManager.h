#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "..\..\Global.h"

class CCamera;	// �J��r���x�[�X�N���X.

/****************************************
* �J�����Ǘ��N���X.
*	�J�����x�[�X���p�������N���X���Z�b�g���g�p����.
**/
class CCameraManager
{
public:
	CCameraManager();
	~CCameraManager();

	// �J�����̐ݒ�.
	static void SetCamera( std::shared_ptr<CCamera> pCamera );

	// �X�V�֐�.
	static void Update();
	// �r���[�s��A�v���W�F�N�V�����s��̏�����.
	static void InitViewProj();

	// �r���[�s��擾�֐�.
	static D3DXMATRIX GetViewMatrix();
	// �v���W�F�N�V�����s��擾�֐�.
	static D3DXMATRIX GetProjMatrix();
	// �J�������W�擾�֐�.
	static D3DXVECTOR3 GetPosition();
	// �J�����������W�擾�֐�.
	static D3DXVECTOR3 GetLookPosition();

private:
	std::shared_ptr<CCamera> m_pCamera;	// �J�����N���X.

private:
	// �C���X�^���X�̎擾.
	static CCameraManager* GetInstance();
};

#endif	// #ifndef CAMERA_MANAGER_H.