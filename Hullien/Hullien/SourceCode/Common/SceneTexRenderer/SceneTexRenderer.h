#ifndef PERA_RENDERER_H
#define PERA_RENDERER_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

#include <vector>

class CSceneTexRenderer : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader\\SceneTexRenderer.hlsl";
public:
	//======================================
	//	�\����.
	//======================================
	// �ݽ����ޯ̧�̱��ؑ��̒�`.
	// ������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct C_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				// ���[���h�s��.
		ALIGN16 D3DXVECTOR2	vViewPort;		// UV���W.
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR2 Tex;
	};

public:
	CSceneTexRenderer();
	virtual ~CSceneTexRenderer();

	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// ���.
	void Release();

	// �`��֐�.
	void Render( std::vector<ID3D11ShaderResourceView*> gbuffers );

private:
	// �V�F�[�_�쐬.
	HRESULT CreateShader();
	// �T���v���̍쐬.
	HRESULT InitSample();
	// ���f���쐬.
	HRESULT CreateModel();

private:
	ID3D11VertexShader*		m_pVertexShader;	// ���_�V�F�[�_�[.
	ID3D11PixelShader*		m_pPixelShader;		// �s�N�Z���V�F�[�_�[.
	ID3D11InputLayout*		m_pVertexLayout;	// ���_���C�A�E�g.
	ID3D11Buffer*			m_pConstantBuffer;	// �R���X�^���g�o�b�t�@.
	ID3D11Buffer*			m_pVertexBuffer;	// ���_�o�b�t�@.
	ID3D11SamplerState*		m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
};

#endif	// #ifndef PERA_RENDERER_H.