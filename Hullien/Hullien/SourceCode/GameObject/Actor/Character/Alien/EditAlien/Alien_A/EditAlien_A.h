#ifndef EDIT_ALIEN_A_H
#define EDIT_ALIEN_A_H

#include "..\EditAlien.h"

class CEditAlienA : public CEditAlien
{
	const char* MODEL_NAME = "a_s";
public:
	CEditAlienA();
	virtual	~CEditAlienA();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
};

#endif	// #ifndef EDIT_ALIEN_A_H.
