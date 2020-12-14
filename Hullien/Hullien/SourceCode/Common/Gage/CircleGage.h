#ifndef CIRCLE_GAGE_H
#define CIRCLE_GAGE_H

#include "..\Common.h"

class CCircleGage : public CCommon
{
	const char* SHADER_NAME			= "Data\\Shader\\CircleGage.hlsl";
	const char* MASK_TEXTURE_NAME	= "Data\\Mesh\\gage.png";
	const char* TEXTURE_NAME		= "Data\\Mesh\\Fog.png";

	struct CBUFFER_PER_FRAME
	{
		ALIGN16 D3DXMATRIX	mW;				// ���[���h�s��.
		ALIGN16 D3DXVECTOR2	vViewPort;		// �r���[�|�[�g.
		ALIGN16 D3DXVECTOR2	vCenterPos;		// �~�̒��S���W : 0~1.
		ALIGN16 D3DXVECTOR2	vStartVecotr;	// �~�̊J�n�x�N�g��.
		ALIGN16 float		Value;			// �l.
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};
public:
	CCircleGage();
	virtual ~CCircleGage();

	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// ���.
	void Release();

	// �����_�����O.
	void Render();
	// �l�̐ݒ�.
	inline void SetValue( const float& value ){ m_Value = value; }

private:
	// ���f��(�|���S��)�̍쐬.
	HRESULT InitModel();
	// �e�N�X�`���̍쐬.
	HRESULT InitTexture();
	// �V�F�[�_�[�̍쐬.
	HRESULT InitShader();
	// �T���v���̍쐬.
	HRESULT InitSampler();
	// �R���X�^���g�o�b�t�@�쐬�֐�.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );

private:
	ID3D11VertexShader*			m_pVertexShader;		// ���_�V�F�[�_�[.
	ID3D11PixelShader*			m_pPixelShader;			// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*			m_pVertexLayout;		// ���_���C�A�E�g.
	ID3D11Buffer*				m_pConstantBufferFrame;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*				m_pVertexBuffer;		// ���_�o�b�t�@.

	ID3D11SamplerState*			m_pSampleLinear;		// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	ID3D11ShaderResourceView*	m_pMaskTexture;			// �}�X�N�e�N�X�`��.
	ID3D11ShaderResourceView*	m_pTexture;				// �e�N�X�`��.

	float						m_Value;
};

#endif	// #ifndef CIRCLE_GAGE_H.