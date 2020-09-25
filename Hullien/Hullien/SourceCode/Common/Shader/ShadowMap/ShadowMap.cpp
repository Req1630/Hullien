#include "ShadowMap.h"
#include "..\..\D3DX\D3DX11.h"
#include "..\..\..\Light\LightManager\LightManager.h"

CShadowMap::CShadowMap()
	: m_pVertexSkinShader	( nullptr )
	, m_pVertexSkinLayout	( nullptr )
{
}

CShadowMap::~CShadowMap()
{
	GetInstance()->Release();
}

CShadowMap* CShadowMap::GetInstance()
{
	static std::unique_ptr<CShadowMap> pInstance = 
		std::make_unique<CShadowMap>();
	return pInstance.get();
}

// ������.
HRESULT CShadowMap::Init()
{
	GetInstance()->Init( CDirectX11::GetDevice(), CDirectX11::GetContext() );
	return S_OK;
}

// ���.
HRESULT CShadowMap::ReleaseShader()
{
	GetInstance()->Release();
	return S_OK;
}

void CShadowMap::SetConstantBufferData( const D3DXMATRIX& mLWVP, const bool isSkin )
{
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// �R���X�^���g�o�b�t�@.

	GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexLayout );

	// �g�p����V�F�[�_�̃Z�b�g.
	GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	if( isSkin == true ){
		GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexSkinShader, nullptr, 0 );	// ���_�V�F�[�_.
		GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexSkinLayout );
	}

	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( GetInstance()->m_pContext11->Map( 
		GetInstance()->m_pConstantBuffer, 
		0, 
		D3D11_MAP_WRITE_DISCARD, 
		0, 
		&pData ))){

		cb.mLightWVP = mLWVP;
		//���s��̌v�Z���@��DirectX��GPU�ňقȂ邽�ߓ]�u���K�v.
		D3DXMatrixTranspose( &cb.mLightWVP, &cb.mLightWVP ); // �s���]�u����.

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		GetInstance()->m_pContext11->Unmap( GetInstance()->m_pConstantBuffer, 0 );
	}

	// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	GetInstance()->m_pContext11->VSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// ���_�V�F�[�_.
	GetInstance()->m_pContext11->PSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.
}

//-------------------------------------.
// �e��V�F�[�_�̐ݒ�.
//-------------------------------------.
void CShadowMap::ShaderSet( ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer )
{
	// �g�p����V�F�[�_�̃Z�b�g.
	GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.

																							// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	GetInstance()->m_pContext11->VSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// ���_�V�F�[�_.
	GetInstance()->m_pContext11->PSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.

																									// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexLayout );
}
// ������.
HRESULT CShadowMap::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( FAILED( InitShader())) return E_FAIL;
	return S_OK;
}

// ���.
HRESULT CShadowMap::Release()
{
	SAFE_RELEASE( m_pVertexSkinShader );
	SAFE_RELEASE( m_pVertexSkinLayout );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	return S_OK;
}

// �V�F�[�_�[�쐬.
HRESULT CShadowMap::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSL����o�[�e�b�N�X�V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
			"VS_Main",			// �V�F�[�_�[�G���g���[�|�C���g.
			"vs_5_0",			// �V�F�[�_�[���f��.
			uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
			&pCompiledShader,	// �u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors ))){		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE( "hlsl read failure" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// ��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if( FAILED( CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexShader ))){
		ERROR_MESSAGE( "VertexShader creation failed" );
		return E_FAIL;
	}

	// ���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{  
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	// ���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED(
		CShaderBase::CreateInputLayout(
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "Vertex input layout creation failed" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// HLSL����o�[�e�b�N�X�V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
			"VS_SkinMain",			// �V�F�[�_�[�G���g���[�|�C���g.
			"vs_5_0",			// �V�F�[�_�[���f��.
			uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
			&pCompiledShader,	// �u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors ))){		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE( "hlsl read failure" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// ��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if( FAILED( CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexSkinShader ))){
		ERROR_MESSAGE( "VertexShader creation failed" );
		return E_FAIL;
	}

	// ���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC skinLayout[] = 
	{  
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONE_INDEX",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONE_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// ���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	numElements = sizeof( skinLayout ) / sizeof( skinLayout[0] );

	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED(
		CShaderBase::CreateInputLayout(
			skinLayout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "Vertex input layout creation failed" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	//HLSL�����߸�ټ���ނ�����ނ��쐬.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
			"PS_Main",			// �V�F�[�_�[�G���g���[�|�C���g.
			"ps_5_0",			// �V�F�[�_�[���f��.
			uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
			&pCompiledShader,	// �u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors ))){		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE( "hlsl read failure" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬��������ނ���u�߸�ټ���ށv���쐬.
	if( FAILED( CShaderBase::CreatePixelShader( pCompiledShader, &m_pPixelShader ))){
		ERROR_MESSAGE( "VertexShader creation failed" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// �V�F�[�_�[�ɓ���̐��l�𑗂�o�b�t�@.
	// �R���X�^���g�o�b�t�@�̍쐬.
	if( FAILED(
		CShaderBase::CreateConstantBuffer( &m_pConstantBuffer, sizeof(C_BUFFER) ))){
		ERROR_MESSAGE( "Instant buffer creation failed" );
		return E_FAIL;
	}

	return S_OK;
}