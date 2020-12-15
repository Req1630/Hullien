#include "CircleGage.h"
#include <algorithm>

CCircleGage::CCircleGage()
	: m_pVertexShader			( nullptr )
	, m_pPixelShader			( nullptr )
	, m_pVertexLayout			( nullptr )
	, m_pConstantBufferFrame	( nullptr )
	, m_pVertexBuffer			( nullptr )
	, m_pSampleLinear			( nullptr )
	, m_pTexture				( nullptr )
	, m_pMaskTexture			( nullptr )
	, m_Value					( 0.0f )
{
}

CCircleGage::~CCircleGage()
{
	Release();
}

// ������.
HRESULT CCircleGage::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( m_pDevice11 == nullptr ) return E_FAIL;
	if( m_pContext11 == nullptr ) return E_FAIL;

	if( FAILED( InitTexture() ))	return E_FAIL;
	if( FAILED( InitModel() ))		return E_FAIL;
	if( FAILED( InitShader() ))		return E_FAIL;
	if( FAILED( InitSampler() ))	return E_FAIL;
	if( FAILED( CreateCBuffer( &m_pConstantBufferFrame, sizeof(CBUFFER_PER_FRAME) ) )) return E_FAIL;

	return S_OK;
}

// ���.
void CCircleGage::Release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBufferFrame );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pMaskTexture );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;
}

// �����_�����O.
void CCircleGage::Render()
{
	if( GetAsyncKeyState('K') & 0x8000 ) m_Value += 0.01f;
	if( GetAsyncKeyState('L') & 0x8000 ) m_Value -= 0.01f;
	if( m_Value > 1.0f ) m_Value = 1.0f;
	if( m_Value < 0.0f ) m_Value = 0.0f;

	//�@���[���h�s��, �X�P�[���s��, ��]�s��, ���s�ړ��s��.
	D3DXMATRIX mWVP, mScale, mRot, mTran;

	// �g��k���s��쐬.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// ��]�s����쐬.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.y, m_vRot.x, m_vRot.z );
	// ���s�ړ��s��.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );

	// ���[���h�s��쐬.
	mWVP = mScale * mRot * mTran;

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	CBUFFER_PER_FRAME cb;	// �R���X�^���g�o�b�t�@.

	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBufferFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ))){

		// ���[���h�s���n��.
		cb.mW	= mWVP;
		D3DXMatrixTranspose( &cb.mW, &cb.mW ); // �s���]�u����.

		// �r���[�|�[�g�̕�,������n��.
		cb.vViewPort.x	= static_cast<float>(WND_W);
		cb.vViewPort.y	= static_cast<float>(WND_H);

		// �~�̒��S���W.
		// 0~1�܂łȂ̂ŁA0.5�����S.
		cb.vCenterPos = { 0.5f, 0.5f };

		// �~�̊J�n�x�N�g��.
		// 2D�Ȃ̂ŁA��������}�C�i�X.
		cb.vStartVecotr = { 0.0, -1.0f };

		// �l 0~1 ��radian�Ƃ��ēn��.
		cb.Value	= static_cast<float>( (D3DX_PI*2.0)*std::clamp(m_Value, 0.0f, 1.0f) );

		memcpy_s( pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb) );

		m_pContext11->Unmap( m_pConstantBufferFrame, 0 );
	}

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.

	// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBufferFrame );	// �s�N�Z���V�F�[�_�[.

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(VERTEX); // �f�[�^�̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	// �v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// �e�N�X�`�����V�F�[�_�[�ɓn��.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pTexture );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pMaskTexture );

	// �����_�����O.
	SetDeprh( false );
	SetBlend( true );
	m_pContext11->Draw( 4, 0 );
	SetBlend( false );
	SetDeprh( true );
}

// ���f��(�|���S��)�̍쐬.
HRESULT CCircleGage::InitModel()
{
	// �ǂݍ��񂾃e�N�X�`���̃T�C�Y���擾����.
	ID3D11Texture2D *pTextureInterface = nullptr;
	m_pTexture->GetResource((ID3D11Resource**)&pTextureInterface);
	D3D11_TEXTURE2D_DESC desc = {};
	if( pTextureInterface != nullptr ){
		pTextureInterface->GetDesc( &desc );
	}
	float w = static_cast<float>(desc.Width)/2;
	float h = static_cast<float>(desc.Height)/2;
	
	// �|��(�l�p�`)�̒��_���쐬.
	VERTEX vertices[] =
	{
		// �|���S���̒��S�𒸓_�Ƃ���.
		// ���_���W(x,y,z)					UV���W(u,v)
		D3DXVECTOR3(-w,    h, 0.0f), D3DXVECTOR2(0.0f, 1.0f),	//���_�P(����).
		D3DXVECTOR3(-w, -h, 0.0f), D3DXVECTOR2(0.0f, 0.0f),	//���_�Q(����).
		D3DXVECTOR3(   w,    h, 0.0f), D3DXVECTOR2(1.0f, 1.0f),	//���_�R(�E��).
		D3DXVECTOR3(   w, -h, 0.0f), D3DXVECTOR2(1.0f, 0.0f)	//���_�S(�E��).
	};

	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(VERTEX) * 4;		// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;						// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;						// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;						// �\���̂̃T�C�Y(���g�p).

	// �T�u���\�[�X�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	// �T�u���\�[�X�\����.
	InitData.pSysMem = vertices;	// �|���̒��_���Z�b�g.

	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ))) {
		_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
		return E_FAIL;
	}

	return S_OK;
}

// �e�N�X�`���̍쐬.
HRESULT CCircleGage::InitTexture()
{
	// �e�N�X�`���쐬.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,	// ���\�[�X���g�p����f�o�C�X�̃|�C���^.
			TEXTURE_NAME,	// �t�@�C����.
			nullptr,
			nullptr,
			&m_pTexture,	// (out)�e�N�X�`��.
			nullptr ))){
		std::string err = TEXTURE_NAME;
		err += " : �e�N�X�`���ǂݍ��ݎ��s";
		ERROR_MESSAGE(err);
		return E_FAIL;
	}

	// �}�X�N�p�̃e�N�X�`���쐬.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,		// ���\�[�X���g�p����f�o�C�X�̃|�C���^.
			MASK_TEXTURE_NAME,	// �t�@�C����.
			nullptr,
			nullptr,
			&m_pMaskTexture,	// (out)�e�N�X�`��.
			nullptr ))){
		std::string err = MASK_TEXTURE_NAME;
		err += " : �e�N�X�`���ǂݍ��ݎ��s";
		ERROR_MESSAGE(err);
		return E_FAIL;
	}

	return S_OK;
}


// �V�F�[�_�[�̍쐬.
HRESULT CCircleGage::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG

	// HLSL����o�[�e�b�N�X�V�F�[�_�[�̃u���uނ��쐬.
	if( FAILED( D3DX11CompileFromFile(
		SHADER_NAME, 
		nullptr,
		nullptr,
		"VS_Main",
		"vs_5_0",
		uCompileFlag, 
		0, 
		nullptr, 
		&pCompiledShader, 
		&pErrors, 
		nullptr ))){
		_ASSERT_EXPR( false, L"hlsl�ǂݍ��ݎ��s" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// ��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�[�v���쐬.
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader ))){	//(out)�ްï�������.
		_ASSERT_EXPR(false, L"�ްï������ލ쐬���s");
		return E_FAIL;
	}

	// ���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	// ���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = 0;

	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	numElements = sizeof(tmp) / sizeof(tmp[0]);	//�v�f���Z�o.
	memcpy_s( layout, sizeof(layout),
		tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);


	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ))){	//(out)���_�C���v�b�g���C�A�E�g.
		_ASSERT_EXPR( false, L"���_���߯�ڲ��č쐬���s" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	// HLSL����s�N�Z���V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr, 
			nullptr, 
			"PS_Main", 
			"ps_5_0",
			uCompileFlag, 
			0, 
			nullptr, 
			&pCompiledShader, 
			&pErrors, 
			nullptr ))){
		_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// ��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�[�v���쐬.
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader ))){	// (out)�s�N�Z���V�F�[�_�[.
		_ASSERT_EXPR( false, L"�߸�ټ���ލ쐬���s" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	return S_OK;
}

// �T���v���̍쐬.
HRESULT CCircleGage::InitSampler()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ���j�A�t�B���^(���`���).
														// POINT:���������e��.
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// ���b�s���O���[�h(WRAP:�J��Ԃ�).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// MIRROR: ���]�J��Ԃ�.
	// CLAMP : �[�̖͗l�������L�΂�.
	// BORDER: �ʓr���E�F�����߂�.
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, 
		&m_pSampleLinear ))) {
		_ASSERT_EXPR( false, L"����׍쐬���s" );
		return E_FAIL;
	}
	return S_OK;
}

// �R���X�^���g�o�b�t�@�쐬�֐�.
HRESULT CCircleGage::CreateCBuffer( ID3D11Buffer** pConstantBuffer, UINT size )
{
	D3D11_BUFFER_DESC cb;

	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= size;
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;
	if( FAILED( m_pDevice11->CreateBuffer(
		&cb, 
		nullptr,
		pConstantBuffer ))){
		return E_FAIL;
	}
	return S_OK;
}