#ifndef TRAJECTOTY_H
#define TRAJECTOTY_H

#include "..\ShaderBase.h"
#include <vector>

class CTrajectory : public CShaderBase
{
	// �V�F�[�_�[��.
	const char* SHADER_NAME = "Data\\Shader\\Trajectory.hlsl";
	// �e�N�X�`����.
	const char* IMAGE_NAME = "Data\\Mesh\\Laser\\laser.png";
	// ���̃r���{�[�h�p�e�N�X�`����.
	const char* HEAD_IMAGE_NAME = "Data\\Mesh\\Laser\\LaserHead.png";
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
	void CreateVertexBuffer( const std::vector<D3DXVECTOR3>& height );

private:
	// �V�F�[�_�[�쐬.
	virtual HRESULT InitShader() override;
	// �e�N�X�`���̍쐬.
	HRESULT CreateTexture();
	// �T���v���̍쐬.
	HRESULT CreateSample();
	// �u�����h�쐬.
	HRESULT InitBlend();
	// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
	void SetCoverage( bool EnableCoverage );

private:
	ID3D11ShaderResourceView*	m_pLaserTexture;		// ���[�U�[�e�N�X�`��.
	ID3D11ShaderResourceView*	m_pLaserHeadTexture;	// ���[�U�[�̓��e�N�X�`��.
	ID3D11SamplerState*			m_pSampleLinear;		// �T���v��.
	ID3D11Buffer*				m_pVertexBuffer;		// ���_�o�b�t�@.
	ID3D11Buffer*				m_pHeadVertexbuffer;	// ���̒��_�o�b�t�@.
	ID3D11BlendState*			m_pNoAlphaBlend;		// �A���t�@�u�����h����.
	ID3D11BlendState*			m_pAlphaToCoverage;		// �A���t�@�J�o���[�W�L��.
	int							m_VertexCount;			// ���_��.
	D3DXVECTOR3					m_HeadPosition;			// ���̍��W.
};

#endif	// #ifndef TRAJECTOTY_H.