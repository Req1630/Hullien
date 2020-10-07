#include "Trajectory.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\D3DX\D3DX11.h"

#include <vector>

CTrajectory::CTrajectory()
	: m_pLaserTexture		( nullptr )
	, m_pLaserHeadTexture	( nullptr )
	, m_pSampleLinear		( nullptr )
	, m_pVertexBuffer		( nullptr )
	, m_pHeadVertexbuffer	( nullptr )
	, m_pNoAlphaBlend		( nullptr )
	, m_pAlphaToCoverage	( nullptr )
	, m_VertexCount			( 0 )
	, m_HeadPosition		{ 0.0f, 0.0f, 0.0f }
{
}

CTrajectory::~CTrajectory()
{
}

// ������.
HRESULT CTrajectory::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = CDirectX11::GetDevice();
	m_pContext11 = CDirectX11::GetContext();

	if( FAILED( InitShader() ))		return E_FAIL;
	if( FAILED( CreateTexture() ))	return E_FAIL;
	if( FAILED( CreateSample() ))	return E_FAIL;
	if( FAILED( InitBlend() ))		return E_FAIL;
	return S_OK;
}

// ���.
HRESULT CTrajectory::Release()
{
	SAFE_RELEASE( m_pNoAlphaBlend );
	SAFE_RELEASE( m_pAlphaToCoverage );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pLaserHeadTexture );
	SAFE_RELEASE( m_pLaserTexture );
	SAFE_RELEASE( m_pHeadVertexbuffer )
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	return S_OK;
}

// �`��.
void CTrajectory::Render()
{
	SetCoverage( true );
	//////////////////////////////////////////////.
	// ���̕`��.
	//////////////////////////////////////////////.
	// ���[���h�s��.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );	// �s��̏�����.
	// ���̍��W��ݒ�.
	D3DXMatrixTranslation( &mWorld, m_HeadPosition.x, m_HeadPosition.y, m_HeadPosition.z );
	// �r���{�[�h�ŕ`��.
	D3DXMATRIX CancelRotation = CCameraManager::GetViewMatrix();
	CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f; // xyz��0�ɂ���.
	// CancelRotation�̋t�s������߂�.
	D3DXMatrixInverse( &CancelRotation, nullptr, &CancelRotation );
	mWorld = CancelRotation * mWorld;
	D3DXMATRIX mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// �R���X�^���g�o�b�t�@.
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	if( SUCCEEDED( m_pContext11->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		// ���[���h�A�r���[�A�v���W�F�N�V�����s���n��.
		cb.mWVP = mWVP;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );// �s���]�u����.

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	// �R���X�^���g�o�b�t�@�̃Z�b�g.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.

	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );				// �T���v���̃Z�b�g.
	m_pContext11->PSSetShaderResources( 0, 1, &m_pLaserHeadTexture );	// �e�N�X�`���̃Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	UINT stride = sizeof(VERTEX);	// �f�[�^�̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pHeadVertexbuffer, &stride, &offset );
	m_pContext11->Draw( 4, 0 );	// �|�����_4��.

	//////////////////////////////////////////////.
	// ���[�U�[�̕`��.
	//////////////////////////////////////////////.
	D3DXMatrixIdentity( &mWorld );	// �s��̏�����.
	// WVP�̍쐬.
	mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();
	// �R���X�^���g�o�b�t�@�̐ݒ�.
	if( SUCCEEDED( m_pContext11->Map( m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		// ���[���h�A�r���[�A�v���W�F�N�V�����s���n��.
		cb.mWVP = mWVP;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);// �s���]�u����.

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}
	
	m_pContext11->PSSetShaderResources( 0, 1, &m_pLaserTexture );	// �e�N�X�`���̃Z�b�g.

	stride = sizeof(VERTEX); // �f�[�^�̊Ԋu.
	offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pContext11->Draw( m_VertexCount, 0 );

	SetCoverage( false );
}

// ���_�o�b�t�@�̎擾.
void CTrajectory::CreateVertexBuffer( const std::vector<D3DXVECTOR3>& height )
{
	m_HeadPosition = height.back();		// ���̍��W���擾����.
	m_VertexCount = height.size()*2;
	std::vector<VERTEX> vertex( m_VertexCount );	// ���_�o�b�t�@�̐ݒ�.
	float x = 0.0f;
	for( int i = 0; i < static_cast<int>(height.size()); i++ ){
		// �_����J�����ւ̒P�ʃx�N�g�������߂�.
		D3DXVECTOR3 z = CCameraManager::GetPosition() - height[i];
		// �_�̒P�ʐڐ��x�N�g�������߂�
		D3DXVECTOR3 t;
		if( i == 0 ){
			t = height[i + 1] - height[i];
		} else if( i == height.size() - 1 ){
			t = height[i] - height[i - 1];
		} else {
			t = height[i + 1] - height[i - 1];
		}
		// �X�g���b�v���_�����߂�
		D3DXVECTOR3 cross;
		D3DXVec3Cross( &cross, &t, &z );
		D3DXVec3Normalize( &cross, &cross );
		vertex[i*2].Pos		= height[i] - cross * 1.0f;
		vertex[i*2+1].Pos	= height[i] + cross * 1.0f;

		// UV���W�����߂�.
		vertex[i*2].Tex		= { x / static_cast<float>(m_VertexCount/2), 1.0f };
		vertex[i*2+1].Tex	= { x / static_cast<float>(m_VertexCount/2), 0.0f };
		x += x <= 0.0f ? 2.0f : 1.0f;
	}
	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(VERTEX) * vertex.size();	// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;	// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;	// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;	// �\���̂̃T�C�Y(���g�p).
	
	// �T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &vertex[0];	// �|���̒��_���Z�b�g.
	
	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, 
		&InitData,
		&m_pVertexBuffer ))){
		ERROR_MESSAGE( "���_�ޯ̧�쐬���s" );
		return;
	}
}

// �V�F�[�_�[�쐬.
HRESULT CTrajectory::InitShader()
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
			&pErrors))) {		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE("hlsl read failure");
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// ��L�ō쐬�����u���u����V�F�[�_�[���쐬.
	if( FAILED( CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexShader ))){
		ERROR_MESSAGE("VertexShader creation failed");
		return E_FAIL;
	}

	// ���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		CShaderBase::GetPositionInputElement(),
		CShaderBase::GetTexcoordInputElement(),
	};

	// ���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED(
		CShaderBase::CreateInputLayout(
			layout,
			numElements,
			pCompiledShader,
			&m_pVertexLayout ))){
		ERROR_MESSAGE("Vertex input layout creation failed");
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );


	// HLSL����s�N�Z���V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
			"PS_Main",			// �V�F�[�_�[�G���g���[�|�C���g.
			"ps_5_0",			// �V�F�[�_�[���f��.
			uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
			&pCompiledShader,	// �u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors ))){		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE("hlsl read failure");
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬�����u���u����s�N�Z���V�F�[�_�[���쐬.
	if( FAILED( CShaderBase::CreatePixelShader(pCompiledShader, &m_pPixelShader ))){
		ERROR_MESSAGE("VertexShader creation failed");
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// �V�F�[�_�[�ɓ���̐��l�𑗂�o�b�t�@.
	// �R���X�^���g�o�b�t�@�̍쐬.
	if( FAILED(
		CShaderBase::CreateConstantBuffer( &m_pConstantBuffer, sizeof(C_BUFFER) ))){
		ERROR_MESSAGE("Instant buffer creation failed");
		return E_FAIL;
	}

	return S_OK;
}

// �e�N�X�`���̍쐬.
HRESULT CTrajectory::CreateTexture()
{
	// �e�N�X�`���̍쐬.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,
			IMAGE_NAME,
			nullptr,
			nullptr,
			&m_pLaserTexture,
			nullptr ))){
		_ASSERT_EXPR(false, L"ø����쐬���s");
		return E_FAIL;
	}
	// �e�N�X�`���̍쐬.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,
			HEAD_IMAGE_NAME,
			nullptr,
			nullptr,
			&m_pLaserHeadTexture,
			nullptr ))){
		_ASSERT_EXPR(false, L"ø����쐬���s");
		return E_FAIL;
	}

	const float size = 1.0f;
	// �|��(�l�p�`)�̒��_���쐬.
	VERTEX vertices[] =
	{
		// �|���S���̒��S�𒸓_�Ƃ���.
		// ���_���W(x,y,z)				 UV���W(u,v)
		D3DXVECTOR3(-size, -size, 0.0f), D3DXVECTOR2( 0.0f, 1.0f ),	//���_�P(����).
		D3DXVECTOR3(-size,  size, 0.0f), D3DXVECTOR2( 0.0f, 0.0f ),	//���_�Q(����).
		D3DXVECTOR3( size, -size, 0.0f), D3DXVECTOR2( 1.0f, 1.0f ),	//���_�R(�E��).
		D3DXVECTOR3( size,  size, 0.0f), D3DXVECTOR2( 1.0f, 0.0f )	//���_�S(�E��).
	};
	int vertexSize = sizeof(vertices)/sizeof(vertices[0]);
	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(VERTEX) * vertexSize;	// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;	// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;	// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;	// �\���̂̃T�C�Y(���g�p).
	
	// �T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	// �|���̒��_���Z�b�g.
	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pHeadVertexbuffer ))){
		_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
		return E_FAIL;
	}
	return S_OK;
}

// �T���v���̍쐬.
HRESULT CTrajectory::CreateSample()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))
	{
		ERROR_MESSAGE("SamplerState creation failed");
		return E_FAIL;
	}
	return S_OK;
}

//--------------------------------------------.
// �u�����h�쐬.
//--------------------------------------------.
HRESULT CTrajectory::InitBlend()
{
	// ��̧�����ޗp�����޽ðč\����.
	// pnģ�ٓ��ɱ�̧��񂪂���̂ŁA���߂���悤�������޽ðĂŐݒ肷��.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory( &BlendDesc, sizeof( BlendDesc ) );

	BlendDesc.IndependentBlendEnable				= false;	
	BlendDesc.AlphaToCoverageEnable					= true;
	BlendDesc.RenderTarget[0].BlendEnable			= true;
	BlendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;	
	BlendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	BlendDesc.AlphaToCoverageEnable = true;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pAlphaToCoverage ) ) ){
		ERROR_MESSAGE( "BlendState(AlphaToCoverage) creation failed" );
		return E_FAIL;
	}

	BlendDesc.RenderTarget[0].BlendEnable	= false;	//false:��̧�����ނ��g�p���Ȃ�.
	BlendDesc.AlphaToCoverageEnable			= false;
	if( FAILED( m_pDevice11->CreateBlendState( &BlendDesc, &m_pNoAlphaBlend ) ) ){
		ERROR_MESSAGE( "BlendState(NoAlphaBlend) creation failed" );
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------.
// �A���t�@�J�o���[�W��L��:�����ɐݒ肷��.
//--------------------------------------------.
void CTrajectory::SetCoverage( bool EnableCoverage )
{
	// �u�����h�X�e�[�g�̐ݒ�.
	UINT mask = 0xffffffff;	// �}�X�N�l.
	ID3D11BlendState* blend = 
		EnableCoverage == true ? m_pAlphaToCoverage : m_pNoAlphaBlend;
	m_pContext11->OMSetBlendState( blend, nullptr, mask );
}