/**
* @file FontCreate.h.
* @brief Fontテクスチャ作成クラス.
* @author 福田玲也.
*/
#ifndef FONT_CREATE_H
#define FONT_CREATE_H

#include "..\Common.h"
#include <Shlwapi.h>

/**********************************:
*	フォントテクスチャ作成クラス.
**/
class CFontCreate
{
public:
	CFontCreate( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	~CFontCreate();

	// フォント画像の作成.
	HRESULT CreateFontTexture2D( const char* c, ID3D11ShaderResourceView** textur2D );

private:
	CFontCreate();
	// フォントを利用可能にする.
	int FontAvailable();
	// Textur2D_Descを作成.
	D3D11_TEXTURE2D_DESC CreateDesc( UINT width, UINT height );

private:
	ID3D11Device*			m_pDevice11;	// デバイスオブジェクト.
	ID3D11DeviceContext*	m_pContext11;	// デバイスコンテキスト.
};

#endif	// #ifndef FONT_CREATE_H.