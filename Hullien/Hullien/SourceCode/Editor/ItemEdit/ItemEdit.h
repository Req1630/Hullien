#ifndef ITEM_EDIT_H
#define ITEM_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Item\ItemBase.h"
#include "..\..\GameObject\Actor\Item\EachItemEffectStruct.h"
#include <vector>
#include <string>

class CItemEdit : public CEditBase
{
	const char* ITEM_PARAM_FILE_PATH = "Data\\GameParam\\Item\\ItemParam.bin";
	const char* EACH_ITEM_EFFECT_FILE_PATH = "Data\\GameParam\\Item\\EachItemEffect.bin";
	const std::vector<std::string> TAG_LIST =
	{
		"Parameter",
		u8"�񕜃A�C�e��",
		u8"����\�̓A�C�e��",
		u8"�U����UP�A�C�e��",
		u8"�ړ����xUP�A�C�e��"
	};

	const char* ATTACK_MODEL_NAME	= "item attack";	// ���f����.
	const char* CURE_MODEL_NAME		= "item cure";		// ���f����.
	const char* SPEED_MODEL_NAME	= "item speed";		// ���f����.
	const char* ABILITY_MODEL_NAME	= "item ability";	// ���f����.

public:
	CItemEdit();
	virtual ~CItemEdit();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// ���f���̕`��.
	virtual void ModelRender() override;

private:
	// �e�^�O�̕`��.
	void TagRender( const int& index );
	// �p�����[�^�̕`��.
	void ParamRender();

	// �t�@�C���̓ǂݍ���.
	bool FileReading();
	// ���f���̎擾.
	bool GetModel();

private:
	std::vector<std::shared_ptr<CDX9StaticMesh>>	m_pStaticMeshs;
	CItemBase::SParameter m_Prameter;
	SEachItemEffect	m_EachItemEffect;
	float	m_RotY;
	int		m_ItemNo;
};

#endif	// #ifndef ITEM_EDIT_H.