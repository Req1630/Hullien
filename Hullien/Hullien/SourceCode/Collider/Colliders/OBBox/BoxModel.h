/**
* @file BoxModel.h.
* @brief �{�b�N�X�̃��f����`�悷��N���X.
* @author ���c���.
*/
#ifndef BOX_MODEL_H
#define BOX_MODEL_H

#include "..\..\..\Common\Common.h"

class CSimpleShader;

/****************************************
*	�{�b�N�X�̃��f����`�悷��N���X.
**/
class CBoxModel : public CCommon
{
private: //------- �萔 -------.
	inline static const D3DXVECTOR4 MODEL_COLOR			= { 0.0f, 0.0f, 1.0f, 1.0f };	// ���f���̐F.
	inline static const D3DXVECTOR4 MODEL_CHANGE_COLOR	= { 1.0f, 0.0f, 0.0f, 1.0f };	// ���f���̐F.

public: //------- �֐� -------.
	CBoxModel();
	virtual ~CBoxModel();

	// ������.
	HRESULT Init( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint );
	// �`��.
	void Render();
	// �F��ς���.
	void ChangeColor( const bool& changed ){ m_IsColorChanged = changed; }

private: //------- �֐� -------.
		 // WVP Matrix�̍쐬.
	D3DXMATRIX CreateWVPMatrix( D3DXMATRIX& mWVP );
	// ���f���̍쐬.
	HRESULT InitModel( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint );

private: //------- �ϐ� -------.
	std::unique_ptr<CSimpleShader>	m_pSimpleShader;	// �V���v���V�F�[�_.
	ID3D11Buffer*					m_pVertexBuffer;	// ���_�o�b�t�@.
	ID3D11Buffer*					m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@.

	bool m_IsColorChanged;	// �F���ς�������ǂ���.
};

#endif	// #ifndef BOX_MODEL_H.