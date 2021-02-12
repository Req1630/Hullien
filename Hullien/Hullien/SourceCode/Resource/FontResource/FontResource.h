/**
* @file FontResource.h.
* @brief �t�H���g���\�[�X�N���X.
* @author ���c���.
*/
#ifndef FONT_RESOURCE_H
#define FONT_RESOURCE_H

#include "..\..\Global.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <filesystem>	// C++17 �K�{.

namespace fs = std::filesystem;

/******************************
*	�t�H���g���\�[�X�N���X.
**/
class CFontResource
{
	const char* TEXT_LIST_PATH = "Data\\TextList\\TextList.txt";
public:
	CFontResource();
	~CFontResource();

	// �C���X�^���X�̎擾.
	static CFontResource* GetInstance();
	// �t�H���g�̓ǂݍ���.
	static void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// �t�H���g�e�N�X�`���̓ǂݍ���.
	static ID3D11ShaderResourceView* GetTexture2D( const std::string key );

private:
	// �t�H���g�̓ǂݍ���.
	void FontLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

private:
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_FontTexturList;
	bool m_HasFinishedLoading;			// �Ǎ����I�������.
};

#endif	// #ifndef FONT_RESOURCE_H.