#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "..\ShaderBase.h"

class CShadowMap : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\ShadowMap.hlsl";	// �V�F�[�_�[�t�@�C����.
																//���V�F�[�_�[���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	struct C_BUFFER
	{
		D3DXMATRIX	mLightWVP;		// ���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR3	Normal;	//�@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2	Tex;	//ø������W.
	};
public:
	enum enShadowMapRenderPass
	{
		None = -1,

		One,
		Two,

		Max,
	} typedef EShadowMapRenderPass;
public:
	CShadowMap();
	virtual ~CShadowMap();

	// �C���X�^���X�̎擾.
	static CShadowMap* GetInstance();

	// ������.
	static HRESULT Init();
	// ���.
	static HRESULT ReleaseShader();

	// �R���X�^���g�o�b�t�@�Ƀf�[�^��n��.
	static void SetConstantBufferData( const D3DXMATRIX& mLWVP, const bool isSkin = false );
	// �e��V�F�[�_�̐ݒ�.
	static void ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer );

	// ���݂̃p�X��Ԃ��擾.
	static int GetRenderPass(){ return GetInstance()->m_RenderPass; }
	// �����_�����O�p�X��ݒ�.
	static void SetRenderPass( const int& pass ){ GetInstance()->m_RenderPass = pass; }

private:
	// ������.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// ���.
	virtual HRESULT Release() override;
	// �V�F�[�_�[�쐬.
	virtual HRESULT InitShader() override;

private:
	ID3D11VertexShader*	m_pVertexSkinShader;	// �X�L�����b�V���p���_�V�F�[�_�[.
	ID3D11InputLayout*	m_pVertexSkinLayout;	// ���_���C�A�E�g.
	int					m_RenderPass;			// �����_�����O�p�X.
};

#endif	// #ifndef SHADOW_MAP_H.