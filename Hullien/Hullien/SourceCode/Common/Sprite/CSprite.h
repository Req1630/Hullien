#ifndef CSPRITE_UI_H
#define CSPRITE_UI_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"
#include "SpriteState.h"

class CSprite : public CCommon
{
public:
	CSprite();
	CSprite(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	virtual ~CSprite();

	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	// ���.
	void Release();

	// �����_�����O.
	// 3D��Ԃɔz�u.
	void Render( const bool& isBillboard = false );
	// UI�Ƃ��Ĕz�u.
	void RenderUI();
	// �A�j���[�V�����ԍ��̐ݒ�.
	void SetAnimNumber( const int& animNumber );
	// �t���[���^�C���̐ݒ�(���t���[���ŃA�j���[�V�������邩).
	void SetFrameTime( const int& frameTime ) { m_pSpriteData->FrameTime = frameTime; }
	// �A�j���[�V�������Đ�����.
	void PlayAnimation() {m_pSpriteData->IsAnimation = true; }
	// �\���摜�T�C�Y�̎擾.
	D3DXVECTOR2 GetSpriteSize() { return { m_pSpriteData->SState.Disp.w, m_pSpriteData->SState.Disp.h }; }
	// �`����W�̎擾.
	D3DXVECTOR3 GetRenderPos() { return m_pSpriteData->SState.vPos; }
	// �X�v���C�g���̐ݒ�.
	void SetSpriteData( const std::shared_ptr<SSpriteData>& pSpriteData ){ m_pSpriteData = pSpriteData; }
	// �X�v���C�g���̎擾.
	std::shared_ptr<SSpriteData> GetSpriteData(){ return m_pSpriteData; }
private:
	// ���f���쐬.
	HRESULT InitModel();
	// �e�N�X�`���쐬.
	HRESULT CreateTexture( const char* fileName, ID3D11ShaderResourceView** pTexture );
	// �T���v���̍쐬.
	HRESULT InitSample();

	// world�s��̍쐬.
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
	int myGcd( int t, int t2 )
	{
		if (t2 == 0) return t;
		return myGcd(t2, t % t2);
	}
private:
	std::unique_ptr<CSpriteShader>	m_pSpriteShader;	// �X�v���C�g�V�F�[�_�[.
	std::shared_ptr<SSpriteData>	m_pSpriteData;		// �X�v���C�g���.
	ID3D11SamplerState*				m_pSampleLinear;	// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	CSpriteShader::VERTEX m_VerticesUI[4];
};

#endif	// #ifndef CSPRITE_UI_H.