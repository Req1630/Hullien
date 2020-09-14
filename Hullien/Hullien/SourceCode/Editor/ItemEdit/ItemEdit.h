#ifndef ITEM_EDIT_H
#define ITEM_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\GameObject\Actor\Item\ItemBase.h"
#include "..\..\GameObject\Actor\Item\EachItemEffectStruct.h"
#include <vector>
#include <string>

class CItemEdit
{
	const ImVec2 WINDOW_SIZE		= { 510.0f, WND_H-70.0f };	// �E�B���h�E�T�C�Y.
	const ImVec2 RENDER_POSITION	= { 0.0f, 70.0f };	// �`����W.
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

public:
	CItemEdit();
	~CItemEdit();

	// �������֐�.
	bool Init();
	// �`��֐�.
	void Render();

private:
	// �e�^�O�̕`��.
	void TagRender( const int& index );
	// �p�����[�^�̕`��.
	void ParamRender();

	// �t�@�C���̓ǂݍ���.
	bool FileReading();

private:
	CItemBase::SParameter m_Prameter;
	SEachItemEffect	m_EachItemEffect;
};

#endif	// #ifndef ITEM_EDIT_H.