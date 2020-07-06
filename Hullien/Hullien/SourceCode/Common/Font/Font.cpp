#include "Font.h"
#include "..\..\Resource\FontResource\FontResource.h"

CFont::CFont()
	: m_pShader			( std::make_unique<CSpriteShader>() )
	, m_pVertexBuffer	( nullptr )
	, m_pSampleLinear	( nullptr )
	, m_pResourceView	( nullptr )
{
}

CFont::~CFont()
{
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pResourceView );
}

HRESULT CFont::Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitPram( pDevice11, pContext11 ))) return E_FAIL;
	if( FAILED( m_pShader->Init( pDevice11, pContext11 ))) return E_FAIL;
	if( FAILED( CreateVerTexBuffer() )) return E_FAIL;
	if( FAILED( CreateSampleLinear() )) return E_FAIL;

	return S_OK;
}

void CFont::Render( const std::string& text )
{
	// �����������[�v.
	for( int i = 0; i < static_cast<int>(text.length()); i++ ){
		std::string f = text.substr( i, 1 );
		if( IsDBCSLeadByte( text[i] ) == TRUE ){
			f = text.substr( i++, 2 );	// �S�p����.
		} else {
			f = text.substr( i, 1 );	// ���p����.
		}
		RenderFont( CFontResource::GetTexture2D(f) );
		m_vPos.x += (FONT_SIZE*2.1f) * m_vScale.x;
	}
}

void CFont::RenderFont( ID3D11Texture2D* texture )
{
	// ���\�[�X�r���[�̍쐬.
	CreateResourceView( texture );

	// �V�F�[�_�[�Ɋe�l��ݒ�.
	m_pShader->SetConstantBufferData( CreateWorldMatrix(), m_vColor, TEXTUR_UV_POS );
	m_pShader->ShaderUISet( m_pVertexBuffer );

	// �v���~�e�B�u�g�|���W�[.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// �e�N�X�`�����V�F�[�_�[�ɓn��.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pResourceView );

	// �`��.
	SetBlend( true );
	SetRasterizerState( enRS_STATE::Back );
	m_pContext11->Draw( 4, 0 );
	SetRasterizerState( enRS_STATE::None );
	SetBlend( false );
}

//---------------------------------.
// �o�[�e�b�N�X�o�b�t�@�̍쐬.
//---------------------------------.
HRESULT CFont::CreateVerTexBuffer()
{
	CSpriteShader::VERTEX vertices[] =
	{
		D3DXVECTOR3(-FONT_SIZE,  FONT_SIZE, 0.0f ),	D3DXVECTOR2( 0.0f, 1.0f ),
		D3DXVECTOR3(-FONT_SIZE, -FONT_SIZE, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),
		D3DXVECTOR3( FONT_SIZE,  FONT_SIZE, 0.0f ),	D3DXVECTOR2( 1.0f, 1.0f ),
		D3DXVECTOR3( FONT_SIZE, -FONT_SIZE, 0.0f ),	D3DXVECTOR2( 1.0f, 0.0f )
	};
	// �ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@.
	bd.ByteWidth			= sizeof(CSpriteShader::VERTEX)*uVerMax;	//���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;		// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;		// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;		// �\���̂̃T�C�Y(���g�p).

	// �T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;		// �|���̒��_���Z�b�g.

	// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"���_�ޯ̧�쐬���s" );
		return E_FAIL;
	}
	return S_OK;
}

//---------------------------------.
// �T���v���[�̍쐬.
//---------------------------------.
HRESULT CFont::CreateSampleLinear()
{
	// �V�F�[�_�p�ɃT���v�����쐬����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof(samDesc) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pDevice11->CreateSamplerState( &samDesc, &m_pSampleLinear );

	return S_OK;
}

//---------------------------------.
// ���\�[�X�r���[�̍쐬.
//---------------------------------.
HRESULT CFont::CreateResourceView( ID3D11Texture2D* texture )
{
	// �e�N�X�`�������擾����.
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetDesc( &texDesc );

	// ShaderResourceView�̏����쐬����.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc) );
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	m_pDevice11->CreateShaderResourceView( texture, &srvDesc, &m_pResourceView );

	return S_OK;
}

//---------------------------------.
// ���[���h�}�g���b�N�X�̍쐬.
//---------------------------------.
D3DXMATRIX CFont::CreateWorldMatrix()
{
	// ���[���h�s��, �X�P�[���s��, ��]�s��, ���s�ړ��s��.
	D3DXMATRIX mScale, mRot, mTran;

	// �g��k���s��쐬.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// ��]�s����쐬.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.x, m_vRot.y, m_vRot.z );
	// ���s�ړ��s��.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );

	// ���[���h�s��쐬.
	return mScale * mRot * mTran;
}