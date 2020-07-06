#ifndef SPRITE_SHADER_H
#define SPRITE_SHADER_H

#include "..\ShaderBase.h"

class CSpriteShader : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\Sprite.hlsl";

public:
	// �R���X�^���g�o�b�t�@�̃A�v�����̒�`.
	//���V�F�[�_�[���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	struct C_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				// ���[���h�s��.
		ALIGN16 D3DXMATRIX	mWVP;
		ALIGN16 D3DXVECTOR4 vColor;			// �J���[(RGBA�̌^�ɍ��킹��).
		ALIGN16 D3DXVECTOR2 vUV;			// UV���W.
		ALIGN16 D3DXVECTOR2 vViewPort;
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};

public:
	CSpriteShader();
	virtual ~CSpriteShader();

	// ������.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// ���.
	virtual HRESULT Release() override;

	// �R���X�^���g�o�b�t�@�Ƀf�[�^��n��.
	void SetConstantBufferData( 
		const D3DXMATRIX& mWVP, 
		const D3DXVECTOR4& color, 
		const D3DXVECTOR2& texPos );

	// �e��V�F�[�_�̐ݒ�.
	void ShaderSet( ID3D11Buffer* pVertexBuffer );
	// �e��UI�p�̃V�F�[�_�̐ݒ�.
	void ShaderUISet( ID3D11Buffer* pVertexBuffer );

private:
	// �V�F�[�_�[�쐬.
	virtual HRESULT InitShader() override;

private:
	ID3D11VertexShader*		m_pVertexShaderUI;	// ���_�V�F�[�_�[ UI�p�Ɏg�p.
};

#endif	// #ifndef SPRITE_SHADER_H.