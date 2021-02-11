/**
* @file Font.h.
* @brief Font�e�N�X�`���`��N���X.
* @author ���c���.
*/
#ifndef FONT_H
#define FONT_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

/****************************************
*	�t�H���g�e�N�X�`���`��N���X.
**/
class CFont : public CCommon
{
public:
	CFont();
	virtual ~CFont();

	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	void RenderUI( const std::string& text );
	void Render( const std::string& text, const bool& isBillboard = false );

private:
	// �e�N�X�`���̕`��.
	void RenderFontUI( const char* c );
	// �e�N�X�`���̕`��.
	void RenderFont( const char* c, const bool& isBillboard  );
	// �o�[�e�b�N�X�o�b�t�@�̍쐬.
	HRESULT CreateVerTexBuffer();
	// �T���v���[�̍쐬.
	HRESULT CreateSampleLinear();
	// ���[���h�}�g���b�N�X�̍쐬.
	D3DXMATRIX CreateWorldMatrix();
	// WVP�̍쐬.
	D3DXMATRIX CreateWVPMatrix();

private:
	std::unique_ptr<CSpriteShader>	m_pShader;			// �X�v���C�g�V�F�[�_�[.
	ID3D11Buffer*					m_pVertexBuffer;	// �o�[�e�b�N�X�o�b�t�@.
	ID3D11Buffer*					m_pVertexBufferUI;	// ���_�o�b�t�@.
	ID3D11SamplerState*				m_pSampleLinear;	// �T���v���[.
};

#endif	// #ifndef FONT_H.