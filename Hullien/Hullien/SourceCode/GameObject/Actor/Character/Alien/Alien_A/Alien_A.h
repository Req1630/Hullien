#ifndef ALIEN_A_H
#define ALIEN_A_H

#include "..\Alien.h"

class CAlienA : public CAlien
{
	const char* MODEL_NAME = "GhostB_s";
	const char* MODEL_TEMP_NAME = "a_b_csize";	// �����f����.

public:
	CAlienA();
	virtual ~CAlienA();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// �X�|�[��.
	virtual bool Spawn( const stAlienParam& param, const D3DXVECTOR3& spawnPos ) override;

private:
	// �X�|�[��.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct() override;
	// ����.
	virtual void Fright() override;
	// ���S.
	virtual void Death() override;
	// ������.
	virtual void Escape() override;

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

};

#endif	// #ifndef ALIEN_A_H.