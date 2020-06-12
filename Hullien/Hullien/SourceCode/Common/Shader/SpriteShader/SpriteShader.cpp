#include "SpriteShader.h"

CSpriteShader::CSpriteShader()
	: m_pVertexShaderUI	( nullptr )
{
}

CSpriteShader::~CSpriteShader()
{
}

//-------------------.
// ������.
//-------------------.
HRESULT CSpriteShader::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( FAILED( InitShader())) return E_FAIL;

	return S_OK;
}

//-------------------.
// ���.
//-------------------.
HRESULT CSpriteShader::Release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pVertexShaderUI );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pConstantBuffer );

	m_pDevice11 = nullptr;
	m_pContext11 = nullptr;

	return S_OK;
}

//--------------------------------.
// �R���X�^���g�o�b�t�@�Ƀf�[�^��n��.
//--------------------------------.
void CSpriteShader::SetConstantBufferData( const D3DXMATRIX& mWVP, const float& fAlpha, const D3DXVECTOR2& texPos )
{
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// �R���X�^���g�o�b�t�@.

					// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map( 
		m_pConstantBuffer, 
		0, 
		D3D11_MAP_WRITE_DISCARD, 
		0, 
		&pData ))){

		//ܰ��ލs���n��.
		cb.mWVP = mWVP;
		cb.mW = mWVP;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );//�s���]�u����.
		D3DXMatrixTranspose( &cb.mW, &cb.mW );//�s���]�u����.
											  // �r���[�|�[�g�̕�,������n��.
		cb.fViewPortWidth	= static_cast<float>(WND_W);
		cb.fViewPortHeight	= static_cast<float>(WND_H);

		// �A���t�@�l��n��.
		cb.vColor.w = fAlpha;

		// �e�N�X�`�����W.
		cb.vUV.x = texPos.x;
		cb.vUV.y = texPos.y;

		memcpy_s( 
			pData.pData, 
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb) );

		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}
}

//--------------------------------.
// �e��V�F�[�_�̐ݒ�.
//--------------------------------.
void CSpriteShader::ShaderSet( ID3D11Buffer* pVertexBuffer )
{
	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.

																// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.

																	// ���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(VERTEX); // �f�[�^�̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
}

//--------------------------------.
// �e��UI�p�̃V�F�[�_�̐ݒ�.
//--------------------------------.
void CSpriteShader::ShaderUISet( ID3D11Buffer* pVertexBuffer )
{
	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShaderUI, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	// �s�N�Z���V�F�[�_.

																// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.

																	// ���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(VERTEX); // �f�[�^�̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
}

//--------------------------------.
// �V�F�[�_�[�쐬.
//--------------------------------.
HRESULT CSpriteShader::InitShader()
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



	// HLSL����o�[�e�b�N�X�V�F�[�_�[�̃u���u���쐬.
	if( FAILED(
		CShaderBase::InitShader(
			SHADER_NAME,		// �V�F�[�_�[�t�@�C����.
			"VS_MainUI",		// �V�F�[�_�[�G���g���[�|�C���g.
			"vs_5_0",			// �V�F�[�_�[���f��.
			uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
			&pCompiledShader,	// �u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors ))){		// �G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		ERROR_MESSAGE( "hlsl read failure" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	// ��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if( FAILED( CShaderBase::CreateVertexShader( pCompiledShader, &m_pVertexShaderUI ))){
		ERROR_MESSAGE( "VertexShader creation failed" );
		return E_FAIL;
	}



	// ���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{  
		CShaderBase::GetPositionInputElement(),
		CShaderBase::GetTexcoordInputElement(),
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