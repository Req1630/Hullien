#ifndef TRANSITION_H
#define TRANSITION_H

#include "..\Common.h"

class CTransition : public CCommon
{
	const char* SHADER_NAME		= "Data\\Shader\\Transition.hlsl";
	struct CBUFFER_PER_INIT
	{
		ALIGN16 D3DXMATRIX	mW;		// ���[���h�s��.
		ALIGN16 D3DXVECTOR2 vViewPort;
	};
	struct CBUFFER_PER_FRAME
	{
		ALIGN16 D3DXMATRIX	mW;		// ���[���h�s��.
		ALIGN16 float Value;
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};
public:
	CTransition();
	virtual ~CTransition();

	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* ruleSpritePath,
		const float& width,
		const float& height );

	// ���.
	void Release();

	// �����_�����O.
	void Render();

	// �e�N�X�`���̐ݒ�.
	void SetTexture( ID3D11ShaderResourceView* pTexture ){ m_pTexture = pTexture; }
	void SetDestTexture( ID3D11ShaderResourceView* pTexture ){ m_pDestTexture = pTexture; }
	// �l�̐ݒ�.
	void SetValue( const float& value ){ m_Value = value; }

private:
	// ���f��(�|���S��)�̍쐬.
	HRESULT InitModel( const float& width, const float& height );
	// �e�N�X�`���̍쐬.
	HRESULT InitTexture( const char* filename );
	// �V�F�[�_�[�̍쐬.
	HRESULT InitShader();
	// �T���v���̍쐬.
	HRESULT InitSampler();
	// �R���X�^���g�o�b�t�@�쐬�֐�.
	HRESULT CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size );

	// �R���X�^���g�o�b�t�@�̐ݒ�.
	void SetConstantBufferInit();

private:
	ID3D11VertexShader*			m_pVertexShader;	// ���_�V�F�[�_�[.
	ID3D11PixelShader*			m_pPixelShader;		// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*			m_pVertexLayout;	// ���_���C�A�E�g.
	ID3D11Buffer*				m_pConstantBufferInit;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*				m_pConstantBufferFrame;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*				m_pVertexBuffer;	// ���_�o�b�t�@.

	ID3D11SamplerState*			m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	ID3D11ShaderResourceView*	m_pMaskTexture;		// �}�X�N�e�N�X�`��.
	ID3D11ShaderResourceView*	m_pTexture;			// �e�N�X�`��.
	ID3D11ShaderResourceView*	m_pDestTexture;			// �e�N�X�`��.

	float m_Value;
};

#endif	// #ifndef TRANSITION_H.