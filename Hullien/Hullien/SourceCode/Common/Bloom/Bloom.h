#ifndef BLOOM_H
#define BLOOM_H

#include "..\Common.h"

class CBloom : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader/Bloom.hlsl";
	const D3DXVECTOR4 CLEAR_BACK_COLOR = { 0.0f, 0.0f, 0.0f, 0.0f };
	// �R���X�^���g�o�b�t�@.
	struct C_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				// ���[���h�s��.
		ALIGN16 D3DXVECTOR2	vViewPort;		// UV���W.
		ALIGN16 D3DXVECTOR2 vPixelSize;		// �s�N�Z���̃T�C�Y.
		ALIGN16 D3DXVECTOR2	vRatio;			// ��ʂ̔䗦.
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2 Tex;	// �e�N�X�`�����W.
	};
public:
	const int SAMPLE_BLUR_MAX = 6;

public:
	CBloom();
	virtual ~CBloom();

	// ������.
	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// ���.
	void Release();

	// ���x���T���v�����O����.
	void Sampling( ID3D11ShaderResourceView* tex );

	// �o�b�t�@�̃N���A.
	void ClearBuffer();

	// �u���[�e�N�X�`���̎擾.
	std::vector<ID3D11ShaderResourceView*> GetBlurTex(){ return m_pBlurBufferSRV; }

private:
	// �u���[�p�̍쐬.
	HRESULT InitBlurTex();
	// �V�F�[�_�쐬.
	HRESULT CreateShader();
	// �T���v���̍쐬.
	HRESULT InitSample();
	// �R���X�^���g�o�b�t�@�̍쐬.
	HRESULT CreateConstantBuffer();
	// ���f���쐬.
	HRESULT CreateModel();

	// �o�b�t�@�̍쐬.
	HRESULT CreateBufferTex(
		const D3D11_TEXTURE2D_DESC	texDesc,
		ID3D11RenderTargetView**	ppRTV,
		ID3D11ShaderResourceView**	ppSRV,
		ID3D11Texture2D**			ppTex );
private:

	std::vector<ID3D11RenderTargetView*>	m_pBlurBufferRTV;	// �u���[�p�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[.
	std::vector<ID3D11ShaderResourceView*>	m_pBlurBufferSRV;	// �u���[�p�o�b�t�@�̃X�e���V���r���[.
	std::vector<ID3D11Texture2D*>			m_pBlurBufferTex;	// �u���[�p�o�b�t�@�̃e�N�X�`���[2D.

	ID3D11VertexShader*		m_pVertexShader;		// ���_�V�F�[�_�[.
	ID3D11PixelShader*		m_pPixelShader;			// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*		m_pVertexLayout;		// ���_���C�A�E�g.
	ID3D11SamplerState*		m_pSampleLinear;		// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	std::vector<ID3D11Buffer*> m_pConstantBuffer;	// �R���X�^���g�o�b�t�@.
	std::vector<ID3D11Buffer*> m_pVertexBuffer;		// ���_�o�b�t�@.

	UINT					m_WndWidth;			// �E�B���h�E��.
	UINT					m_WndHeight;		// �E�B���h�E����.
};

#endif	// #ifndef BLOOM_H.