#ifndef GAGE_H
#define GAGE_H

#include "..\Common.h"

class CGage : public CCommon
{
	const char* SHADER_NAME		= "Data\\Shader\\Gage.hlsl";
	const char* TEXTURE_NAME	= "Data\\Mesh\\gage.png";

	struct CBUFFER_PER_FRAME
	{
		ALIGN16 D3DXMATRIX	mW;				// ���[���h�s��.
		ALIGN16 D3DXVECTOR4	vColor;
		ALIGN16 D3DXVECTOR2	vViewPort;
		ALIGN16 float		ImageWidth;
		ALIGN16 float		DispPos;
		ALIGN16 float		Value;
		ALIGN16 float		RangeValue;
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};
public:
	CGage();
	virtual ~CGage();

	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// ���.
	void Release();

	// �����_�����O.
	void Render();

private:
	// ���f��(�|���S��)�̍쐬.
	HRESULT InitModel();
	// �e�N�X�`���̍쐬.
	HRESULT InitTexture( const char* filename );
	// �V�F�[�_�[�̍쐬.
	HRESULT InitShader();
	// �T���v���̍쐬.
	HRESULT InitSampler();
	// �R���X�^���g�o�b�t�@�쐬�֐�.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );

private:
	ID3D11VertexShader*			m_pVertexShader;	// ���_�V�F�[�_�[.
	ID3D11PixelShader*			m_pPixelShader;		// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*			m_pVertexLayout;	// ���_���C�A�E�g.
	ID3D11Buffer*				m_pConstantBufferFrame;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*				m_pVertexBuffer;	// ���_�o�b�t�@.

	ID3D11SamplerState*			m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	ID3D11ShaderResourceView*	m_pTexture;			// �e�N�X�`��.
};

#endif	// #ifndef TRANSITION_H.