#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"

class CRotLookAtCenter;

class CPlayer : public CCharacter
{
	const char* MODEL_NAME = "Sayaka_s";	// ���f����.
public:
	CPlayer();
	virtual ~CPlayer();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// ����֐�.
	void Controller();
	// �ړ��֐�.
	virtual void Move() override;

private:
	std::shared_ptr<CDX9SkinMesh>		m_pSkinMesh;	// �X�L�����b�V��.
	std::shared_ptr<CRotLookAtCenter>	m_pCamera;		// �J�����N���X.
};

#endif	// #ifndef PLAYER_H.