#include "InvisibleWallShader.h"
#include "..\D3DX\D3DX11.h"
#include "..\..\Camera\CameraManager\CameraManager.h"

namespace
{
	const char* SHADER_NAME		= "Data\\Shader\\InvisibleWall.hlsl";	// �V�F�[�_�[��.
	const char* TEXTURE_NAME	= "Data\\Mesh\\shield.png";				// �ǂ̃e�N�X�`��.
};

CInvisibleWallShader::CInvisibleWallShader()
	: m_pVertexShader	( nullptr )
	, m_pPixelShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pTexture		( nullptr )
	, m_pMaskTexture	( nullptr )
	, m_pSampleLinear	( nullptr )
	, m_pConstantBuffer	( nullptr )
	, m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
{
}

CInvisibleWallShader::~CInvisibleWallShader()
{
	Release();
}

// ������.
HRESULT CInvisibleWallShader::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const D3DXVECTOR3& vMaxPoint,
	const D3DXVECTOR3& vMinPoint )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( FAILED( CreateShader() ))		return E_FAIL;
	if( FAILED( InitSample() ))			return E_FAIL;
	if( FAILED( CreateModel( vMaxPoint, vMinPoint) ))	return E_FAIL;
	if( FAILED( InitTexture( TEXTURE_NAME ) )) return E_FAIL;

	D3D11_BUFFER_DESC cb;

	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= sizeof(C_BUFFER);
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;

	return m_pDevice11->CreateBuffer( &cb, nullptr, &m_pConstantBuffer );

	return S_OK;
}

// ���.
void CInvisibleWallShader::Release()
{
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pMaskTexture );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}

// �`��.
void CInvisibleWallShader::Render()
{
	// ���[���h�s��.
	D3DXMATRIX mWorld, mWVP, mVPT;
	D3DXMATRIX mRot, mTran, mScale;

	// ��].
	D3DXMATRIX mYaw, mPitch, mRoll;
	D3DXMatrixRotationX( &mPitch, m_vRot.x );
	D3DXMatrixRotationY( &mYaw, m_vRot.y );
	D3DXMatrixRotationZ( &mRoll, m_vRot.z );
	mRot = mYaw * mPitch * mRoll;
	// ���s�ړ�.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );
	// �g��k���s��쐬.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );
	// ���[���h�s��
	mWorld = mScale * mRot * mTran;
	mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();


	D3DXMATRIX mT = 
	{
		0.5f,  0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f
	};
	mVPT = CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix() * mT;

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map( 
		m_pConstantBuffer, 
		0, 
		D3D11_MAP_WRITE_DISCARD, 
		0, 
		&pData ))){

		C_BUFFER cb;	// �R���X�^���g�o�b�t�@.
		cb.mWVP = mWVP;
		cb.mW = mWorld;
		cb.mVPT = mVPT;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP ); // �s���]�u����.
		D3DXMatrixTranspose( &cb.mW, &cb.mW ); // �s���]�u����.
		D3DXMatrixTranspose( &cb.mVPT, &cb.mVPT ); // �s���]�u����.

		cb.Color = {};
		cb.PlayerPos = { m_PlayerPosition, 0.0f };

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		// �o�b�t�@���̃f�[�^�̏��������I������Unmap.
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );// �s�N�Z���V�F�[�_.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.

	m_pContext11->PSSetShaderResources( 0, 1, &m_pTexture );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pMaskTexture );
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// �T���v���̃Z�b�g.

	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	SetBlend( true );
	m_pContext11->DrawIndexed( 36, 0, 0 );
	SetBlend( false );
}

// �V�F�[�_�쐬.
HRESULT CInvisibleWallShader::CreateShader()
{
	ID3DBlob* pCompileVS = nullptr;
	ID3DBlob* pCompilePS = nullptr;
	ID3DBlob* pErrorMsg = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG
	//----------------------------.
	// ���_�V�F�[�_�[.
	//----------------------------.
	if( FAILED(
		D3DX10CompileFromFile( 
			SHADER_NAME, 
			nullptr, 
			nullptr, 
			"VS_Main",
			"vs_5_0", 
			uCompileFlag,
			0,
			nullptr,
			&pCompileVS, 
			&pErrorMsg,
			nullptr ))){
		ERROR_MESSAGE( "vs hlsl Loading Failure." );
		return E_FAIL;
	}
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompileVS->GetBufferPointer(),
			pCompileVS->GetBufferSize(),
			nullptr,
			&m_pVertexShader ))){
		ERROR_MESSAGE( "vs hlsl Creating Failure." );
		return E_FAIL;
	}
	//----------------------------.
	// �s�N�Z���V�F�[�_�[.
	//----------------------------.
	if( FAILED(
		D3DX10CompileFromFile( 
			SHADER_NAME, 
			nullptr, 
			nullptr, 
			"PS_Main", 
			"ps_5_0", 
			uCompileFlag, 
			0,
			nullptr,
			&pCompilePS,
			&pErrorMsg,
			nullptr ))){
		ERROR_MESSAGE( (char*)pErrorMsg->GetBufferPointer()  );
		return E_FAIL;
	}
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompilePS->GetBufferPointer(),
			pCompilePS->GetBufferSize(),
			nullptr,
			&m_pPixelShader ))){
		ERROR_MESSAGE( "ps hlsl Creating Failure." );
		return E_FAIL;
	}
	//----------------------------.
	//	���_�C���v�b�g.
	//----------------------------.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int numElements = sizeof(layout)/sizeof(layout[0]);	// �v�f���Z�o.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompileVS->GetBufferPointer(),
			pCompileVS->GetBufferSize(),
			&m_pVertexLayout ))){
		ERROR_MESSAGE( "vs layout Creating Failure." );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompileVS );
	SAFE_RELEASE( pCompilePS );

	return S_OK;
}

// �T���v���̍쐬.
HRESULT CInvisibleWallShader::InitSample()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter				= D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.AddressU			= D3D11_TEXTURE_ADDRESS_MIRROR;		// UV�l��[0,1]�𒴂�����,[0,1]�ɐݒ肷��.
	samDesc.AddressV			= D3D11_TEXTURE_ADDRESS_MIRROR;
	samDesc.AddressW			= D3D11_TEXTURE_ADDRESS_MIRROR;
	samDesc.ComparisonFunc		= D3D11_COMPARISON_NEVER;
	samDesc.MipLODBias			= 0;
	samDesc.MaxAnisotropy		= 1;
	samDesc.ComparisonFunc		= D3D11_COMPARISON_NEVER;
	samDesc.BorderColor[ 0 ]	= 0.0f;
	samDesc.BorderColor[ 1 ]	= 0.0f;
	samDesc.BorderColor[ 2 ]	= 0.0f;
	samDesc.BorderColor[ 3 ]	= 0.0f;
	samDesc.MinLOD				= -D3D11_FLOAT32_MAX;
	samDesc.MaxLOD				= D3D11_FLOAT32_MAX;
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) ){
		ERROR_MESSAGE( "SamplerState creation failed" );
		return E_FAIL;
	}
	return S_OK;
}

// ���f���쐬.
HRESULT CInvisibleWallShader::CreateModel( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint )
{
	//�����(�l�p�`)�̒��_���쐬.
	VERTEX vertices[] =
	{
		//���_���W(x,y,z).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //���_�P(���㉜).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //���_�Q(�E�㉜).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //���_�R(������).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //���_�S(�E����).

		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //���_�P(�����O).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //���_�Q(������O).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //���_�R(�E���O).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z ), //���_�S(�E����O).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z ), //���_�P(�����O).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //���_�Q(���㉜).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //���_�R(������O).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //���_�S(������).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //���_�P(�E���O).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //���_�Q(�E����O).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //���_�R(�E�㉜).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //���_�S(�E����).

		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y,-vMinPoint.z ), //���_�P(����).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y, vMaxPoint.z ), //���_�Q(����).
		D3DXVECTOR3(-vMinPoint.x,-vMinPoint.y,-vMinPoint.z ), //���_�R(�E��).
		D3DXVECTOR3( vMaxPoint.x,-vMinPoint.y, vMaxPoint.z ), //���_�S(�E��).

		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y,-vMinPoint.z ), //���_�P(����).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y, vMaxPoint.z ), //���_�Q(����).
		D3DXVECTOR3( vMaxPoint.x, vMaxPoint.y,-vMinPoint.z ), //���_�R(�E��).
		D3DXVECTOR3(-vMinPoint.x, vMaxPoint.y, vMaxPoint.z )  //���_�S(�E��).
	};
	WORD indexList[]{
		0,  1,  2,     1,  0,  3,
		4,  5,  6,     5,  4,  7,
		8,  9,  10,    9,  8,  11,
		12, 13, 14,    13, 12, 15,
		16, 17, 18,    17, 16, 19,
		20, 21, 22,    21, 20, 23,
	};

	// �ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices)/sizeof(vertices[0]);
	UINT uIndexMax = sizeof(indexList)/sizeof(indexList[0]);

	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(VERTEX)*uVerMax;	// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;						// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;						// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;						// �\���̂̃T�C�Y(���g�p).

	// �T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	// �|���̒��_���Z�b�g.

	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, 
		&InitData, 
		&m_pVertexBuffer ))){
		ERROR_MESSAGE( "���_�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	D3D11_BUFFER_DESC id;
	id.Usage				= D3D11_USAGE_DEFAULT;	   // �g�p���@(�f�t�H���g).
	id.ByteWidth			= sizeof(WORD)*uIndexMax;  // ���_�̃T�C�Y.
	id.BindFlags			= D3D11_BIND_INDEX_BUFFER; // ���_�o�b�t�@�Ƃ��Ĉ���.
	id.CPUAccessFlags		= 0;					   // CPU����̓A�N�Z�X���Ȃ�.
	id.MiscFlags			= 0;					   // ���̑��̃t���O(���g�p).
	id.StructureByteStride	= 0;					   // �\���̂̃T�C�Y(���g�p).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem			= indexList;
	IndexData.SysMemPitch		= 0;
	IndexData.SysMemSlicePitch	= 0;

	if( FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &m_pIndexBuffer ))){
		_ASSERT_EXPR( false, L"�C���f�b�N�X�ޯ̧�쐬���s" );
		return E_FAIL;
	}
	return S_OK;
}

// �e�N�X�`���̍쐬.
HRESULT CInvisibleWallShader::InitTexture( const char* filename )
{
	// �}�X�N�p�̃e�N�X�`���쐬.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,	// ���\�[�X���g�p����f�o�C�X�̃|�C���^.
			filename,		// �t�@�C����.
			nullptr,
			nullptr,
			&m_pTexture,	// (out)�e�N�X�`��.
			nullptr ))) {
		std::string err = filename;
		err += " : �e�N�X�`���ǂݍ��ݎ��s";
		ERROR_MESSAGE(err);
		return E_FAIL;
	}
	// �}�X�N�p�̃e�N�X�`���쐬.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,	// ���\�[�X���g�p����f�o�C�X�̃|�C���^.
			"Data\\Mesh\\shieldMask.png",		// �t�@�C����.
			nullptr,
			nullptr,
			&m_pMaskTexture,	// (out)�e�N�X�`��.
			nullptr ))) {
		std::string err = filename;
		err += " : �e�N�X�`���ǂݍ��ݎ��s";
		ERROR_MESSAGE(err);
		return E_FAIL;
	}
	return S_OK;
}
