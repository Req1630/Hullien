#include "Bloom.h"
#include "..\D3DX\D3DX11.h"
#include "..\..\Editor\EditRenderer\EditRenderer.h"
#include "..\..\Utility\ImGuiManager\ImGuiManager.h"

CBloom::CBloom()
	: m_pDownLuminanceRTV	( nullptr )
	, m_pDownLuminanceSRV	( nullptr )
	, m_pDownLuminanceTex	( nullptr )
	, m_pBlurBufferRTV		( SAMPLE_BLUR_MAX )
	, m_pBlurBufferSRV		( SAMPLE_BLUR_MAX )
	, m_pBlurBufferTex		( SAMPLE_BLUR_MAX )
	, m_pVertexShader		( nullptr )
	, m_pPixelShader		( nullptr )
	, m_pVertexLayout		( nullptr )
	, m_pConstantBuffer		( SAMPLE_BLUR_MAX )
	, m_pVertexBuffer		( SAMPLE_BLUR_MAX )
	, m_pSampleLinear		( nullptr )
	, m_WndWidth			( 0 )
	, m_WndHeight			( 0 )
{	  
}

CBloom::~CBloom()
{
}

// ������.
HRESULT CBloom::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;
	m_WndWidth = CDirectX11::GetWndWidth();
	m_WndHeight = CDirectX11::GetWndHeight();

	if( FAILED( InitDownLuminanceTex() ))	return E_FAIL;
	if( FAILED( InitBlurTex() ))			return E_FAIL;
	if( FAILED( CreateShader() ))			return E_FAIL;
	if( FAILED( CreateModel() ))			return E_FAIL;
	if( FAILED( InitSample() ))				return E_FAIL;
	if( FAILED( CreateConstantBuffer() ))	return E_FAIL;

	return S_OK;
}

// ���.
void CBloom::Release()
{
	SAFE_RELEASE( m_pDownLuminanceRTV );
	SAFE_RELEASE( m_pDownLuminanceSRV );
	SAFE_RELEASE( m_pDownLuminanceTex );

	for( auto& rtv : m_pBlurBufferRTV ) SAFE_RELEASE(rtv);
	for( auto& srv : m_pBlurBufferSRV ) SAFE_RELEASE(srv);
	for( auto& tex : m_pBlurBufferTex ) SAFE_RELEASE(tex);

	SAFE_RELEASE( m_pSampleLinear );
	for( auto& b : m_pVertexBuffer ) SAFE_RELEASE(b);
	for( auto& b : m_pConstantBuffer ) SAFE_RELEASE(b);
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );
}

// ���x���T���v�����O����.
void CBloom::Sampling( ID3D11ShaderResourceView* tex )
{
	ID3D11ShaderResourceView* srv = tex;
	int i = -1;
	for( auto& rtv : m_pBlurBufferRTV ){
		// �����_�[�^�[�Q�b�g�̐ݒ�.
		m_pContext11->OMSetRenderTargets( 1, &rtv, CDirectX11::GetDepthSV() );
		// �f�v�X�X�e���V���o�b�t�@.
		m_pContext11->ClearDepthStencilView( CDirectX11::GetDepthSV(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

		// �g�p����V�F�[�_�̃Z�b�g.
		m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
		m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.
		m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );		// �T���v���̃Z�b�g.

		m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer[i+1] );	// ���_�V�F�[�_.
		m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer[i+1] );	// �s�N�Z���V�F�[�_�[.

		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer[i+1], &stride, &offset );
		m_pContext11->IASetInputLayout( m_pVertexLayout );
		m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

		m_pContext11->PSSetShaderResources( 0, 1, &srv );
		m_pContext11->Draw( 4, 0 );

		srv = m_pBlurBufferSRV[i+1];
		i++;
	}

	CEditRenderer::PushRenderProc( 
		[&]()
		{
			ImGui::Image( m_pBlurBufferSRV[0], ImVec2(800, 400) );
			ImGui::Image( m_pBlurBufferSRV[1], ImVec2(800, 400) );
			ImGui::Image( m_pBlurBufferSRV[2], ImVec2(800, 400) );
			ImGui::Image( m_pBlurBufferSRV[3], ImVec2(800, 400) );
			ImGui::Image( m_pBlurBufferSRV[4], ImVec2(800, 400) );
			ImGui::Image( m_pBlurBufferSRV[5], ImVec2(800, 400) );
		});
}

// �P�x�p�̍쐬.
HRESULT CBloom::InitDownLuminanceTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= m_WndWidth;						// ��.
	texDesc.Height				= m_WndHeight;						// ����.
	texDesc.MipLevels			= 1;								// �~�b�v�}�b�v���x��:1.
	texDesc.ArraySize			= 1;								// �z��:1.
	texDesc.Format				= DXGI_FORMAT_R11G11B10_FLOAT;		// 32�r�b�g�t�H�[�}�b�g.
	texDesc.SampleDesc.Count	= 1;								// �}���`�T���v���̐�.
	texDesc.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@:�f�t�H���g.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// �����_�[�^�[�Q�b�g�A�V�F�[�_�[���\�[�X.
	texDesc.CPUAccessFlags		= 0;								// CPU����̓A�N�Z�X���Ȃ�.
	texDesc.MiscFlags			= 0;								// ���̑��̐ݒ�Ȃ�.

	if( FAILED( CreateBufferTex(
		texDesc,
		&m_pDownLuminanceRTV,
		&m_pDownLuminanceSRV,
		&m_pDownLuminanceTex ))) return E_FAIL;

	return S_OK;
}

// �u���[�p�̍쐬.
HRESULT CBloom::InitBlurTex()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width				= m_WndWidth/2;						// ��.
	texDesc.Height				= m_WndHeight/2;					// ����.
	texDesc.MipLevels			= 1;								// �~�b�v�}�b�v���x��:1.
	texDesc.ArraySize			= 1;								// �z��:1.
	texDesc.Format				= DXGI_FORMAT_R11G11B10_FLOAT;		// 32�r�b�g�t�H�[�}�b�g.
	texDesc.SampleDesc.Count	= 1;								// �}���`�T���v���̐�.
	texDesc.SampleDesc.Quality	= 0;								// �}���`�T���v���̃N�I���e�B.
	texDesc.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@:�f�t�H���g.
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;	// �����_�[�^�[�Q�b�g�A�V�F�[�_�[���\�[�X.
	texDesc.CPUAccessFlags		= 0;								// CPU����̓A�N�Z�X���Ȃ�.
	texDesc.MiscFlags			= 0;								// ���̑��̐ݒ�Ȃ�.

	for( int i = 0; i < SAMPLE_BLUR_MAX; i++ ){
		if( FAILED( CreateBufferTex(
			texDesc,
			&m_pBlurBufferRTV[i],
			&m_pBlurBufferSRV[i],
			&m_pBlurBufferTex[i] ))) return E_FAIL;
		// �E�ɃV�t�g���ăe�N�X�`���T�C�Y��i�K�I�ɗ��Ƃ�.
		texDesc.Width	>>= 1;
		texDesc.Height	>>= 1;
	}

	return S_OK;
}

// �V�F�[�_�쐬.
HRESULT CBloom::CreateShader()
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
HRESULT CBloom::InitSample()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter				= D3D11_FILTER_MIN_MAG_MIP_POINT;
	samDesc.AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;		// UV�l��[0,1]�𒴂�����,[0,1]�ɐݒ肷��.
	samDesc.AddressV			= D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW			= D3D11_TEXTURE_ADDRESS_CLAMP;
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

// �R���X�^���g�o�b�t�@�̍쐬.
HRESULT CBloom::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth			= sizeof(C_BUFFER);
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;

	UINT width = m_WndWidth/2;
	UINT height = m_WndHeight/2;
	for( int i = 0; i < SAMPLE_BLUR_MAX; i++ ){
		if( FAILED(  m_pDevice11->CreateBuffer( &cbDesc, nullptr, &m_pConstantBuffer[i] ))){
			ERROR_MESSAGE( "Buffer creation failed" );
			return E_FAIL;
		}

		// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
		D3D11_MAPPED_SUBRESOURCE pData;

		// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
		if( FAILED( m_pContext11->Map( m_pConstantBuffer[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){
			ERROR_MESSAGE( "ConstantBuffer Map failed" );
			return E_FAIL;
		}

		C_BUFFER cb;	// �R���X�^���g�o�b�t�@.
		D3DXMatrixIdentity( &cb.mW );
		D3DXMatrixTranspose( &cb.mW, &cb.mW );
		// �r���[�|�[�g�̕�,������n��.
		cb.vViewPort.x = static_cast<float>(width);
		cb.vViewPort.y = static_cast<float>(height);

		cb.vPixelSize.x = 1.0f / static_cast<float>(width);
		cb.vPixelSize.y = 1.0f / static_cast<float>(height);

		cb.vRatio.x = m_WndWidth/width;
		cb.vRatio.y = m_WndHeight/height;

		// �������̈���R�s�[.
		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );
		m_pContext11->Unmap( m_pConstantBuffer[i], 0 );

		width	>>= 1;
		height	>>= 1;
	}

	return S_OK;
}

// ���f���쐬.
HRESULT CBloom::CreateModel()
{
	UINT width = m_WndWidth/2;
	UINT height = m_WndHeight/2;

	for( int i = 0; i < SAMPLE_BLUR_MAX; i++ ){
		// �|��(�l�p�`)�̒��_���쐬.
		VERTEX vertices[]=
		{
			// ���_���W(x,y,z)				 
			D3DXVECTOR3( 0.0f,						static_cast<float>(height), 0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
			D3DXVECTOR3( 0.0f,						0.0f, 0.0f ),						D3DXVECTOR2( 0.0f, 0.0f ),
			D3DXVECTOR3( static_cast<float>(width), static_cast<float>(height), 0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
			D3DXVECTOR3( static_cast<float>(width), 0.0f, 0.0f ),						D3DXVECTOR2( 1.0f, 0.0f ),
		};
		// �ő�v�f�����Z�o����.
		UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

		// �o�b�t�@�\����.
		D3D11_BUFFER_DESC bd;
		bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@(�f�t�H���g).
		bd.ByteWidth			= sizeof(VERTEX) * uVerMax;	// ���_�̃T�C�Y.
		bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
		bd.CPUAccessFlags		= 0;	// CPU����̓A�N�Z�X���Ȃ�.
		bd.MiscFlags			= 0;	// ���̑��̃t���O(���g�p).
		bd.StructureByteStride	= 0;	// �\���̂̃T�C�Y(���g�p).

		// �T�u���\�[�X�f�[�^�\����.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;	// �|���̒��_���Z�b�g.

		// ���_�o�b�t�@�̍쐬.
		if( FAILED( m_pDevice11->CreateBuffer(
			&bd, 
			&InitData, 
			&m_pVertexBuffer[i] ))){
			ERROR_MESSAGE( "���_�ޯ̧�쐬���s" );
			return E_FAIL;
		}
		width	>>= 1;
		height	>>= 1;
	}
	return S_OK;
}

// �o�b�t�@�̍쐬.
HRESULT CBloom::CreateBufferTex(
	const D3D11_TEXTURE2D_DESC	texDesc,
	ID3D11RenderTargetView**	ppRTV,
	ID3D11ShaderResourceView**	ppSRV,
	ID3D11Texture2D**			ppTex )
{
	// ���̃e�N�X�`���ɑ΂��ăf�v�X�X�e���V��(DSTex)���쐬.
	if( FAILED( m_pDevice11->CreateTexture2D( &texDesc, nullptr, ppTex )) ){
		_ASSERT_EXPR( false, L"�e�N�X�`���f�X�N�쐬���s" );
		return E_FAIL;
	}
	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = texDesc.Format;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	// RenderTargetView�쐬.
	if( FAILED( m_pDevice11->CreateRenderTargetView( *ppTex, &rtvDesc, ppRTV ) )){
		_ASSERT_EXPR( false, L"RenderTargetView�쐬���s" );
		return E_FAIL;
	}

	// �V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset( &srvDesc, 0, sizeof( srvDesc ) );
	srvDesc.Format              = rtvDesc.Format;
	srvDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	// �e�N�X�`���쐬���Ɠ����t�H�[�}�b�g
	if( FAILED( m_pDevice11->CreateShaderResourceView( *ppTex, &srvDesc, ppSRV ) )){
		_ASSERT_EXPR( false, L"�f�v�X�X�e���V���쐬���s" );
		return E_FAIL;
	}
	return S_OK;
}