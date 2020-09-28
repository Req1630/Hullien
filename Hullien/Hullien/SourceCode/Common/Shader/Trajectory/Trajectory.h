#ifndef TRAJECTOTY_H
#define TRAJECTOTY_H

#include "..\ShaderBase.h"
#include <list>

class CTrajectory : public CShaderBase
{
	const char* SHADER_NAME = "Data\\Shader\\Trajectory.hlsl";
	const char* IMAGE_NAME = "Data\\Mesh\\Barrier\\laser.png";
public:
	// �R���X�^���g�o�b�t�@�̃A�v�����̒�`.
	//���V�F�[�_�[���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;
	};
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};
public:
	CTrajectory();
	virtual ~CTrajectory();

	// ������.
	virtual HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) override;
	// ���.
	virtual HRESULT Release() override;

	// �`��.
	void Render();
	// ���_�o�b�t�@�̍쐬.
	void CreateVertexBuffer( 
		const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& height,
		const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& width );

private:
	// �V�F�[�_�[�쐬.
	virtual HRESULT InitShader() override;
	// �e�N�X�`���̍쐬.
	HRESULT CreateTexture();
	// �T���v���̍쐬.
	HRESULT CreateSample();
	// ���_�o�b�t�@�̍쐬.
	void CreateVertexBuffer( 
		const std::list<std::pair<D3DXVECTOR3,D3DXVECTOR3>>& vertexPoint,
		ID3D11Buffer** ppHeightVertexBuffer);

private:
	ID3D11ShaderResourceView*	m_pTexture;			// �e�N�X�`��.
	ID3D11SamplerState*			m_pSampleLinear;	// �T���v��.
	ID3D11Buffer*	m_pHeightVertexBuffer;	// �������_�o�b�t�@.
	ID3D11Buffer*	m_pWidthVertexBuffer;	// �����_�o�b�t�@.
	int m_VertexCount;					// ���_��.
};

#endif	// #ifndef TRAJECTOTY_H.