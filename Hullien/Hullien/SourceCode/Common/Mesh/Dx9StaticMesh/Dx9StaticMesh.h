#ifndef DX9_STATIC_MESH_H
#define DX9_STATIC_MESH_H

#include "..\..\Common.h"
#include "..\MeshStruct.h"
#include "..\..\Fog\Fog.h"

// �X�^�e�B�b�N���b�V���N���X.
class CDX9StaticMesh : public CCommon
{
public:
	CDX9StaticMesh( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName )
		: CDX9StaticMesh()
	{
		Init( hWnd, pDevice11, pContext11, pDevice9, fileName );
	}

	CDX9StaticMesh();	// �R���X�g���N�^.
	virtual ~CDX9StaticMesh();	// �f�X�g���N�^.

						// ������.
	HRESULT Init(
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );

	// ����֐�.
	void Release();

	// �����_�����O�p.
	void Render( const bool& isTrans = false );

	// ���b�V�����擾.
	LPD3DXMESH GetMesh() const { return m_pMeshForRay; }

	// �e��`�悷�邩.
	void SetIsShadow( const bool isShadow ){ m_IsShadow = isShadow; }

private:
	// ���b�V���ǂݍ���.
	HRESULT LoadXMesh(const char* fileName);

	// �V�F�[�_�쐬.
	HRESULT InitShader();

private:
	// ���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	// ���_���W.
		D3DXVECTOR3	Normal;	// �@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W.
	};
	// �}�e���A���\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	// �A���r�G���g.
		D3DXVECTOR4	Diffuse;	// �f�B�q���[�Y.
		D3DXVECTOR4	Specular;	// �X�y�L����.
		char szTextureName[64];	// �e�N�X�`���t�@�C����.
		ID3D11ShaderResourceView* pTexture;	// �e�N�X�`��.
		DWORD dwNumFace;		// ���̃}�e���A���̃|���S����.
								// �R���X�g���N�^.
		MY_MATERIAL()
			: Ambient(),Diffuse(),Specular(),szTextureName()
			, pTexture(nullptr),dwNumFace(0)
		{
		}
		// �f�X�g���N�^.
		~MY_MATERIAL() 
		{
			SAFE_RELEASE(pTexture);
		}
	};

	HWND				m_hWnd;		//����޳�����.

									//Dx9.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9���޲���޼ު��.

	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pCBufferPerMesh;		//�ݽ����ޯ̧(ү����).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//�ݽ����ޯ̧(��رٖ�).
	ID3D11Buffer*			m_pCBufferPerFrame;		//�ݽ����ޯ̧(�ڰі�).

	ID3D11Buffer*				m_pVertexBuffer;	//���_�ޯ̧.
	ID3D11Buffer**				m_ppIndexBuffer;	//���ޯ���ޯ̧.
	ID3D11SamplerState*			m_pSampleLinear;	//�����:ø����Ɋe��̨����������.
	ID3D11SamplerState*			m_pToonSampleLinear;
	ID3D11SamplerState*			m_pShadowMapSampler;// �V���h�E�}�b�v�p�T���v���[.

	LPD3DXMESH			m_pMesh;		//ү����޼ު��.
	LPD3DXMESH			m_pMeshForRay;	//ڲ��ү���p.
	DWORD				m_NumMaterials;	//��رِ�.

	ID3D11ShaderResourceView*	m_pToonTexture;	// �g�D�[���e�N�X�`��.
	ID3D11ShaderResourceView*	m_pFogTexture;	// �t�H�O�e�N�X�`��.

	MY_MATERIAL*	m_pMaterials;	//��رٍ\����.
	DWORD			m_NumAttr;		//������.
	DWORD			m_AttrID[300];	//����ID ��300�����܂�.

	bool			m_EnableTexture;//ø�������.
	bool			m_IsShadow;

	//�����ݸފ֐�(�׽���ł̂ݎg�p����).
	void RenderMesh(
		D3DXMATRIX& mWorld, 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj,
		const bool& isTrans );
	// �e�̕`��.
	bool ShadowRender( const D3DXMATRIX& mWorld );
	// �������̕`��,
	bool TranslucentRender( const D3DXMATRIX& mWorld );
};

#endif	// #ifndef DX9_STATIC_MESH_H.