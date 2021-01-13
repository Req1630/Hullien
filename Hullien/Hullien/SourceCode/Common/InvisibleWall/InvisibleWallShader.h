#ifndef INVISIBLE_WALL_SHADER_H
#define INVISIBLE_WALL_SHADER_H

#include "..\Common.h"

class CInvisibleWallShader : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader\\InvisibleWall.hlsl";
	const char* TEXTURE_NAME = "Data\\Mesh\\shield.png";
	const D3DXVECTOR4 CLEAR_BACK_COLOR = { 0.0f, 0.0f, 0.0f, 0.0f };
	// �R���X�^���g�o�b�t�@.
	struct C_BUFFER
	{
		D3DXMATRIX	mW;			// ���[���h�s��.
		D3DXMATRIX	mWVP;		// ���[���h,�r���[,�v���W�F�N�V�����̍����s��.
		D3DXMATRIX	mVPT;		// ���[���h,�r���[,�v���W�F�N�V�����̍����s��.
		D3DXVECTOR4	PlayerPos;	// �v���C���[�ʒu.
		D3DXVECTOR4	Color;	// �F.
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
	};

	enum enBlurSmpleDir
	{
		EBlurSmpleDir_Width,	// ������.
		EBlurSmpleDir_Height,	// ��������.

		EBlurSmpleDir_Final = EBlurSmpleDir_Height,

		EBlurSmpleDir_Max,
	} typedef EBlurSmpleDir;

public:
	CInvisibleWallShader();
	virtual ~CInvisibleWallShader();

	// ������.
	HRESULT Init( 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11,
		const D3DXVECTOR3& vMaxPoint, 
		const D3DXVECTOR3& vMinPoint );
	// ���.
	void Release();

	// �`��.
	void Render();

	// �v���C���[���W�̐ݒ�.
	inline void SetPlayerPos( const D3DXVECTOR3& pos ){ m_PlayerPosition = pos; }

private:
	// �V�F�[�_�쐬.
	HRESULT CreateShader();
	// �T���v���̍쐬.
	HRESULT InitSample();
	// ���f���쐬.
	HRESULT CreateModel( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint );
	// �e�N�X�`���̍쐬.
	HRESULT InitTexture( const char* filename );

private:
	ID3D11VertexShader*	m_pVertexShader;	// ���_�V�F�[�_�[.
	ID3D11PixelShader*	m_pPixelShader;		// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*	m_pVertexLayout;	// ���_���C�A�E�g.
	ID3D11ShaderResourceView*	m_pTexture;	// �e�N�X�`��.
	ID3D11SamplerState*	m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	ID3D11Buffer*		m_pConstantBuffer;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*		m_pVertexBuffer;	// ���_�o�b�t�@.
	ID3D11Buffer*		m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@.
	D3DXVECTOR3			m_PlayerPosition;	// �v���C���[���W.
};

#endif	// #ifndef INVISIBLE_WALL_SHADER_H.