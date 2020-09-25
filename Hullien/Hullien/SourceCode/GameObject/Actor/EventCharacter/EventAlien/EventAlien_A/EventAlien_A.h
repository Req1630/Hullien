#ifndef EVENTALIEN_A_H
#define EVENTALIEN_A_H

#include "..\EventAlien.h"

/*************************************
*	�C�x���g�p�F���lA.
**/
class CEventAlienA : public CEventAlien
{
	const char* MODEL_NAME = "GhostB_s";
	const char* MODEL_TEMP_NAME = "a";	// �����f����.

public:
	CEventAlienA();
	virtual ~CEventAlienA();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision(CEventActor* pActor) override;
	// �X�|�[��.
	virtual bool Spawn(const D3DXVECTOR3& spawnPos) override;

private:
	// �X�|�[��.
	virtual void Spawning() override;
	// �ړ�.
	virtual void Move() override;
	// ����.
	virtual void Abduct() override;
	// ������.
	virtual void Escape() override;
	// �������.
	virtual void BlowAway() override;
	// �ҋ@.
	virtual void	Wait() override;

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

};

#endif	//#ifndef EVENTALIEN_A_H.