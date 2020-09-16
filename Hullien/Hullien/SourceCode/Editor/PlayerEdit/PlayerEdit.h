#ifndef PLAYER_EDIT_H
#define PLAYER_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Player\Player.h"

class CPlayerEdit : public CEditBase
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
public:
	CPlayerEdit();
	virtual ~CPlayerEdit();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;

private:
	std::unique_ptr<CPlayer> m_pPlayer;
};

#endif	// #ifndef PLAYER_EDIT_H.
