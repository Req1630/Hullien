#ifndef FONT_H
#define FONT_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

class CFont : public CCommon
{
	const float FONT_SIZE = 25.0f;	// �t�H���g�̑傫��.
	const D3DXVECTOR2 TEXTUR_UV_POS = { 1.0f, 1.0f };	// �e�N�X�`��UV���W.

public:
	CFont();
	virtual ~CFont();

	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	void Render( const std::string& text );

private:
	// �e�N�X�`���̕`��.
	void RenderFont( ID3D11Texture2D* texture );
	// �o�[�e�b�N�X�o�b�t�@�̍쐬.
	HRESULT CreateVerTexBuffer();
	// �T���v���[�̍쐬.
	HRESULT CreateSampleLinear();
	// ���\�[�X�r���[�̍쐬.
	HRESULT CreateResourceView( ID3D11Texture2D* texture );
	// ���[���h�}�g���b�N�X�̍쐬.
	D3DXMATRIX CreateWorldMatrix();

private:
	std::unique_ptr<CSpriteShader>	m_pShader;			// �X�v���C�g�V�F�[�_�[.
	ID3D11Buffer*					m_pVertexBuffer;	// �o�[�e�b�N�X�o�b�t�@.
	ID3D11SamplerState*				m_pSampleLinear;	// �T���v���[.
	ID3D11ShaderResourceView*		m_pResourceView;	// ���\�[�X�r���[.
};

#endif	// #ifndef FONT_H.