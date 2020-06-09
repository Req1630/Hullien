#ifndef ALIEN_A_H
#define ALIEN_A_H

#include "..\Alien.h"

class CAlienA : public CAlien
{
	const char* MODEL_NAME = "GhostB_s";

public:
	CAlienA();
	virtual ~CAlienA();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// �ړ��֐�.
	virtual void Move() override;
	
};

#endif	// #ifndef ALIEN_A_H.