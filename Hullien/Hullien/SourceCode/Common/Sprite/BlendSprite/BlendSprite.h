#ifndef BLEND_SPRITE_UI_H
#define BLEND_SPRITE_UI_H

#include "..\..\Common.h"
#include "..\..\Shader\SpriteShader\SpriteShader.h"
#include "..\SpriteState.h"

enum enBLEND_MODE
{
	EBLEND_MODE_None = -1,

	EBLEND_MODE_Normal,
	EBLEND_MODE_HardLight,

	EBLEND_MODE_Max,
} typedef EBLEND_MODE;

class CBlendSprite : public CCommon
{
	const char* SHADER_NAME = "Data\\Shader\\BlendSprite.hlsl";

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
	struct stPSShaderName
	{
		EBLEND_MODE enBlendMode;
		const char* entryName;

	} typedef SPSShaderName;
public:
	CBlendSprite();
	CBlendSprite(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	virtual ~CBlendSprite();

	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);
	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	// ���.
	void Release();

	// �����_�����O.
	// 3D��Ԃɔz�u.
	void Render(const bool& isBillboard = false);
	// UI�Ƃ��Ĕz�u.
	void RenderUI();
	// �A�j���[�V�����ԍ��̐ݒ�.
	void SetAnimNumber(const int& animNumber);
	// �t���[���^�C���̐ݒ�(���t���[���ŃA�j���[�V�������邩).
	void SetFrameTime(const int& frameTime) { m_pSpriteData->FrameTime = frameTime; }
	// �A�j���[�V�������Đ�����.
	void PlayAnimation() { m_pSpriteData->IsAnimation = true; }
	// �\���摜�T�C�Y�̎擾.
	D3DXVECTOR2 GetSpriteSize() { return { m_pSpriteData->SState.Disp.w, m_pSpriteData->SState.Disp.h }; }
	// �`����W�̎擾.
	D3DXVECTOR3 GetRenderPos() { return m_pSpriteData->SState.vPos; }
	// �X�v���C�g���̐ݒ�.
	void SetSpriteData( const std::shared_ptr<SSpriteData>& pSpriteData )
	{
		m_pSpriteData = pSpriteData;
		m_vPos = m_pSpriteData->SState.vPos;
	}
	// �X�v���C�g���̎擾.
	std::shared_ptr<SSpriteData> GetSpriteData(){ return m_pSpriteData; }

	// �K��F�̃e�N�X�`�����̃Z�b�g.
	void SetDestTexture( ID3D11ShaderResourceView* pDestTexture ){ m_pDestTexture = pDestTexture; }
private:
	// �V�F�[�_�[�̍쐬.
	HRESULT InitShader();
	// ���f���쐬.
	HRESULT InitModel();
	// �e�N�X�`���쐬.
	HRESULT CreateTexture( const char* fileName, ID3D11ShaderResourceView** pTexture );
	// �T���v���̍쐬.
	HRESULT InitSample();

	D3DXMATRIX CreateWorldMatrix();
	// UV���W�ɕϊ�.
	D3DXVECTOR2 ConvertIntoUV();
	// �A�j���[�V�����̍X�V.
	void AnimUpdate();

	// ���_���̍쐬.
	void CreateVERTEX(
		const float& w, const float& h,
		const float& u, const float& v);

	// �e�N�X�`���̔䗦���擾.
	int myGcd(int t, int t2)
	{
		if (t2 == 0) return t;
		return myGcd(t2, t % t2);
	}

private:
	std::shared_ptr<SSpriteData>	m_pSpriteData;	// �X�v���C�g���.

	ID3D11VertexShader*				m_pVertexShader;	// ���_�V�F�[�_�[ 3D�`��p�Ɏg�p.
	ID3D11VertexShader*				m_pVertexShaderUI;	// ���_�V�F�[�_�[ UD�`��p�Ɏg�p.
	std::vector<ID3D11PixelShader*>	m_pPixelShaderList;	// �s�N�Z���V�F�[�_�[.
	ID3D11SamplerState*				m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	ID3D11Buffer*					m_pConstantBuffer;	// �R���X�^���g�o�b�t�@.
	ID3D11ShaderResourceView*		m_pDestTexture;		// �K��F��SRV.
	CSpriteShader::VERTEX m_VerticesUI[4];	// ���_�쐬�p.
};

#endif	// #ifndef BLEND_SPRITE_UI_H.