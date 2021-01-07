#include "BlendSprite.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\SceneTexRenderer\SceneTexRenderer.h"
#include <crtdbg.h>
#include <algorithm>

static const CBlendSprite::SPSShaderName PS_SHADER_LIST[] = 
{
	{ EBLEND_MODE_Normal,		"PS_Main" },
	{ EBLEND_MODE_HardLight,	"PS_HardLight" },
};

// �R���X�g���N�^.
CBlendSprite::CBlendSprite()
	: m_pSpriteData		( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pVertexShader	( nullptr )
	, m_pVertexShaderUI	( nullptr )
	, m_pPixelShaderList()
	, m_pSampleLinear	( nullptr )
	, m_pDestTexture	( nullptr )
	, m_VerticesUI		()
{
	// PS�V�F�[�_�[�̃T�C�Y���m��.
	const int psShaderListSize = sizeof(PS_SHADER_LIST)/sizeof(PS_SHADER_LIST[0]);
	m_pPixelShaderList.resize( psShaderListSize );
}

CBlendSprite::CBlendSprite(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SSpriteState& pSs)
	: CBlendSprite()
{
	Init(pDevice11, pContext11, fileName, pSs);
}

// �f�X�g���N�^.
CBlendSprite::~CBlendSprite()
{
	Release();
}

// ������.
HRESULT CBlendSprite::Init(ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SSpriteState& pSs)
{
	m_pSpriteData->SState = pSs;
	m_vPos = m_pSpriteData->SState.vPos;
	m_pSpriteData->enLocalPosition = static_cast<enLocalPosition>(m_pSpriteData->SState.LocalPosNum);
	m_pSpriteData = std::make_shared<SSpriteData>();
	if( FAILED( InitPram( pDevice11, pContext11 ) )) return E_FAIL;
	if( FAILED( InitShader() )) return E_FAIL;
	if( FAILED( InitModel() )) return E_FAIL;
	if( FAILED( CreateTexture( fileName, &m_pSpriteData->pTexture ) )) return E_FAIL;
	if( FAILED( InitSample() )) return E_FAIL;

	return S_OK;
}

// ������.
HRESULT CBlendSprite::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	if( FAILED( InitPram( pDevice11, pContext11 ) )) return E_FAIL;
	if( FAILED( InitShader() )) return E_FAIL;
	if( FAILED( InitSample() )) return E_FAIL;

	return S_OK;
}

// ���.
void CBlendSprite::Release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pVertexShaderUI );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pVertexLayout );
	m_pDestTexture = nullptr;
}

// �V�F�[�_�[�̍쐬.
HRESULT CBlendSprite::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif	// #ifdef _DEBUG.


	// ���_�V�F�[�_�[�̍쐬.

	// 3D�`��p.
	if( FAILED( D3DX10CompileFromFile(
		SHADER_NAME,		// �V�F�[�_�[�t�@�C���p�X.
		nullptr,			// �}�N����`�̃|�C���^�[ : �I�v�V����.
		nullptr,			// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
		"VS_Main",			// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
		"vs_5_0",			// �V�F�[�_�[���f���̖��O.
		uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
		0,					// �G�t�F�N�g�R���p�C���t���O (0����).
		nullptr,			// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
		&pCompiledShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
		&pErrors,			// �R���p�C�����̃G���[�o�� (out).
		nullptr ) )){		// �߂�l�̃|�C���^.
		return E_FAIL;
	}
	if( FAILED( m_pDevice11->CreateVertexShader( 
		pCompiledShader->GetBufferPointer(), 
		pCompiledShader->GetBufferSize(), 
		nullptr, 
		&m_pVertexShader ) )){
		return E_FAIL;
	}
	// 2D�`��p.
	if( FAILED( D3DX10CompileFromFile(
		SHADER_NAME,		// �V�F�[�_�[�t�@�C���p�X.
		nullptr,			// �}�N����`�̃|�C���^�[ : �I�v�V����.
		nullptr,			// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
		"VS_MainUI",		// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
		"vs_5_0",			// �V�F�[�_�[���f���̖��O.
		uCompileFlag,		// �V�F�[�_�[�R���p�C���t���O.
		0,					// �G�t�F�N�g�R���p�C���t���O (0����).
		nullptr,			// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
		&pCompiledShader,	// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
		&pErrors,			// �R���p�C�����̃G���[�o�� (out).
		nullptr ) )){		// �߂�l�̃|�C���^.
		return E_FAIL;
	}
	if( FAILED( m_pDevice11->CreateVertexShader( 
		pCompiledShader->GetBufferPointer(), 
		pCompiledShader->GetBufferSize(), 
		nullptr, 
		&m_pVertexShaderUI ) )){
		return E_FAIL;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// ���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// ���_�C���v�b�g���C�A�E�g���쐬.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ))){
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	
	for( int i = 0; i < static_cast<int>(m_pPixelShaderList.size()); i++ ){
		// 3D�`��p.
		if( FAILED( D3DX10CompileFromFile(
			SHADER_NAME,					// �V�F�[�_�[�t�@�C���p�X.
			nullptr,						// �}�N����`�̃|�C���^�[ : �I�v�V����.
			nullptr,						// �C���N���[�h�t�@�C�����������邽�߂̃|�C���^ : �I�v�V����.
			PS_SHADER_LIST[i].entryName,	// �V�F�[�_�[�̃G���g���[�|�C���g�֐���.
			"ps_5_0",						// �V�F�[�_�[���f���̖��O.
			uCompileFlag,					// �V�F�[�_�[�R���p�C���t���O.
			0,								// �G�t�F�N�g�R���p�C���t���O (0����).
			nullptr,						// �X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[.
			&pCompiledShader,				// �R���p�C�����ꂽ�V�F�[�_�[�̃f�[�^ (out).
			&pErrors,						// �R���p�C�����̃G���[�o�� (out).
			nullptr ) )){					// �߂�l�̃|�C���^.
			ERROR_MESSAGE( (char*)pErrors->GetBufferPointer()  );
			return E_FAIL;
		}
		if( FAILED( m_pDevice11->CreatePixelShader( 
			pCompiledShader->GetBufferPointer(), 
			pCompiledShader->GetBufferSize(), 
			nullptr,
			&m_pPixelShaderList[PS_SHADER_LIST[i].enBlendMode] ) )){
			return E_FAIL;
		}
		SAFE_RELEASE(pCompiledShader);
	}
	SAFE_RELEASE(pCompiledShader);

	D3D11_BUFFER_DESC cb;

	cb.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth			= sizeof(C_BUFFER);
	cb.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags			= 0;
	cb.StructureByteStride	= 0;
	cb.Usage				= D3D11_USAGE_DYNAMIC;
	// �R���X�^���g�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer( &cb, nullptr, &m_pConstantBuffer ) )){
		ERROR_MESSAGE("Instant buffer creation failed");
		return E_FAIL;
	}

	return S_OK;
}

// ���f���쐬.
HRESULT CBlendSprite::InitModel()
{
	float w = m_pSpriteData->SState.Disp.w;	// �\���X�v���C�g��,
	float h = m_pSpriteData->SState.Disp.h;	// �\���X�v���C�g����,
	float u = m_pSpriteData->SState.Stride.w / m_pSpriteData->SState.Base.w;	// 1�R�}������̕�,
	float v = m_pSpriteData->SState.Stride.h / m_pSpriteData->SState.Base.h;	// 1�R�}������̍���.

	m_pSpriteData->PatternMax.x = static_cast<LONG>( m_pSpriteData->SState.Base.w / m_pSpriteData->SState.Stride.w );// x�̍ő�}�X��.
	m_pSpriteData->PatternMax.y = static_cast<LONG>( m_pSpriteData->SState.Base.h / m_pSpriteData->SState.Stride.h );// y�̍ő�}�X��.
	//-------------------------------.
	// UI�p.
	//-------------------------------.
	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// �g�p���@(�f�t�H���g).
	bd.ByteWidth = sizeof(CSpriteShader::VERTEX) * 4;	// ���_�̃T�C�Y.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;								// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags = 0;									// ���̑��̃t���O(���g�p).
	bd.StructureByteStride = 0;							// �\���̂̃T�C�Y(���g�p).

	// �T�u���\�[�X�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	CreateVERTEX(w, h, u, v);
	InitData.pSysMem = m_VerticesUI;	// �|���̒��_���Z�b�g.

	// ���_�o�b�t�@�̍쐬.
	if( FAILED( 
		m_pDevice11->CreateBuffer( 
			&bd, &InitData, &m_pSpriteData->pVertexBufferUI ) )) {
		_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
		return E_FAIL;
	}

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(CSpriteShader::VERTEX);	// �f�[�^�Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pSpriteData->pVertexBufferUI, &stride, &offset );

	//-------------------------------.
	// 3D�p.
	//-------------------------------.
	// �摜�̔䗦�����߂�.
	int as = myGcd((int)m_pSpriteData->SState.Disp.w, (int)m_pSpriteData->SState.Disp.h);
	w = m_pSpriteData->SState.Disp.w / as;
	h = m_pSpriteData->SState.Disp.h / as;
	w *= 0.1f;
	h *= 0.1f;
	// �|��(�l�p�`)�̒��_���쐬.
	CSpriteShader::VERTEX vertices[] =
	{
		// �|���S���̒��S�𒸓_�Ƃ���.
		// ���_���W(x,y,z)					UV���W(u,v)
		D3DXVECTOR3(-w / 2, -h / 2, 0.0f), D3DXVECTOR2(0.0f,    v),	//���_�P(����).
		D3DXVECTOR3(-w / 2,  h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f),	//���_�Q(����).
		D3DXVECTOR3( w / 2, -h / 2, 0.0f), D3DXVECTOR2(   u,    v),	//���_�R(�E��).
		D3DXVECTOR3( w / 2,  h / 2, 0.0f), D3DXVECTOR2(   u, 0.0f)	//���_�S(�E��).
	};

	// �T�u���\�[�X�\����.
	InitData.pSysMem = vertices;	// �|���̒��_���Z�b�g.

	// ���_�o�b�t�@�̍쐬.
	if (FAILED(m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pSpriteData->pVertexBuffer))) {
		_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
		return E_FAIL;
	}

	// ���_�o�b�t�@���Z�b�g.
	m_pContext11->IASetVertexBuffers(0, 1, &m_pSpriteData->pVertexBuffer, &stride, &offset);

	return S_OK;
}

HRESULT CBlendSprite::InitSample()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// �T���v���쐬.
	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))
	{
		ERROR_MESSAGE("SamplerState creation failed");
		return E_FAIL;
	}

	return S_OK;
}

// �e�N�X�`���쐬.
HRESULT CBlendSprite::CreateTexture(const char* fileName,
	ID3D11ShaderResourceView** pTexture)
{
	//ø����쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ؿ�����g�p�������޲����߲��.
		fileName,		//̧�ٖ�.
		nullptr,
		nullptr,
		pTexture,		//(out)ø���.
		nullptr ))){
		std::string err = fileName;
		err += " : �e�N�X�`���ǂݍ��ݎ��s";
		ERROR_MESSAGE( err );
		return E_FAIL;
	}

	return S_OK;
}

//�����ݸޗp.
void CBlendSprite::Render(const bool& isBillboard)
{
	if( m_pSpriteData == nullptr ) return;

	//�@���[���h�s��.
	D3DXMATRIX mWorld;

	// ���[���h�s��쐬.
	mWorld = CreateWorldMatrix();
	if (isBillboard == true) {
		// �r���{�[�h�p.
		D3DXMATRIX CancelRotation = CCameraManager::GetViewMatrix();
		CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f; // xyz��0�ɂ���.
		// CancelRotation�̋t�s������߂�.
		D3DXMatrixInverse(&CancelRotation, nullptr, &CancelRotation);
		mWorld = CancelRotation * mWorld;
	}
	// WVP�̍쐬.
	D3DXMATRIX mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	AnimUpdate();
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// �R���X�^���g�o�b�t�@.

	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData ))) {
		//ܰ��ލs���n��.
		cb.mWVP = mWVP;
		cb.mW = mWVP;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );//�s���]�u����.
		D3DXMatrixTranspose( &cb.mW, &cb.mW );//�s���]�u����.
		// �r���[�|�[�g�̕�,������n��.
		cb.vViewPort.x	= static_cast<float>(WND_W);
		cb.vViewPort.y	= static_cast<float>(WND_H);

		// �A���t�@�l��n��.
		cb.vColor = m_vColor;

		// �e�N�X�`�����W.
		cb.vUV.x = m_pSpriteData->UV.x;
		cb.vUV.y = m_pSpriteData->UV.y;

		memcpy_s(
			pData.pData,
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}
	// 3D�p�̃V�F�[�_�[��ݒ�.
	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );		// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShaderList[0], nullptr, 0 );	// �s�N�Z���V�F�[�_.

	// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(VERTEX); // �f�[�^�̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pSpriteData->pVertexBuffer, &stride, &offset );

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ø����𼪰�ނɓn��.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pSpriteData->pTexture );

	//����è�ނ������ݸ�.
	m_pContext11->Draw( 4, 0 );//�����(���_4��).
}

void CBlendSprite::RenderUI()
{
	if( m_pSpriteData == nullptr ) return;
	// ���W��z�l��������.
	m_vPos.z = 0.0f;
	AnimUpdate();
	// �R���X�^���g�o�b�t�@�̐ݒ�.

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	// �R���X�^���g�o�b�t�@.
	// �o�b�t�@���̃f�[�^�̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pConstantBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData ))) {
		//ܰ��ލs���n��.
		cb.mW = cb.mWVP = CreateWorldMatrix();;
		D3DXMatrixTranspose( &cb.mWVP, &cb.mWVP );//�s���]�u����.
		D3DXMatrixTranspose( &cb.mW, &cb.mW );//�s���]�u����.
		// �r���[�|�[�g�̕�,������n��.
		cb.vViewPort.x	= static_cast<float>(WND_W);
		cb.vViewPort.y	= static_cast<float>(WND_H);

		// �A���t�@�l��n��.
		cb.vColor = m_vColor;

		// �e�N�X�`�����W.
		cb.vUV.x = m_pSpriteData->UV.x;
		cb.vUV.y = m_pSpriteData->UV.y;

		memcpy_s(
			pData.pData,
			pData.RowPitch,
			(void*)(&cb),
			sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	// �g�p����V�F�[�_�̃Z�b�g.
	m_pContext11->VSSetShader( m_pVertexShaderUI, nullptr, 0 );	// ���_�V�F�[�_.
	m_pContext11->PSSetShader( m_pPixelShaderList[1], nullptr, 0 );	// �s�N�Z���V�F�[�_.

	// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�p���邩�H.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// ���_�V�F�[�_.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	// �s�N�Z���V�F�[�_�[.

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(VERTEX); // �f�[�^�̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pSpriteData->pVertexBufferUI, &stride, &offset );

	// ���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pContext11->IASetInputLayout( m_pSpriteData->pVertexLayout );

	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//ø����𼪰�ނɓn��.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pDestTexture );
	m_pContext11->PSSetShaderResources( 1, 1, &m_pSpriteData->pTexture );

	//����è�ނ������ݸ�.
	m_pContext11->Draw(4, 0);//�����(���_4��).
}

D3DXMATRIX CBlendSprite::CreateWorldMatrix()
{
	//�@���[���h�s��, �X�P�[���s��, ��]�s��, ���s�ړ��s��.
	D3DXMATRIX mScale, mRot, mTran;

	// �g��k���s��쐬.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// ��]�s����쐬.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.y, m_vRot.x, m_vRot.z );
	// ���s�ړ��s��.
	D3DXMatrixTranslation(&mTran, m_vPos.x, m_vPos.y, m_vPos.z);

	// ���[���h�s��쐬.
	return mScale * mRot * mTran;
}

// �A�j���[�V�����ԍ��̐ݒ�.
void CBlendSprite::SetAnimNumber(const int& animNumber)
{
	if (m_pSpriteData->AnimNumber >= m_pSpriteData->SState.AnimNum) return;
	m_pSpriteData->IsAnimation = false;	// �A�j���[�V���������Ȃ�.
	// �A�j���[�V�����ԍ����Z�o.
	m_pSpriteData->PatternNo.x = animNumber % m_pSpriteData->PatternMax.x;
	m_pSpriteData->PatternNo.y = animNumber / m_pSpriteData->PatternMax.x;
	// UV���W�ɕϊ�.
	m_pSpriteData->UV = ConvertIntoUV();
}

// UV���W�ɕϊ�.
D3DXVECTOR2 CBlendSprite::ConvertIntoUV()
{
	return {
		// x���W.
		static_cast<float>(m_pSpriteData->PatternNo.x) / static_cast<float>(m_pSpriteData->PatternMax.x),
		// y���W.
		static_cast<float>(m_pSpriteData->PatternNo.y) / static_cast<float>(m_pSpriteData->PatternMax.y)
	};
}

// �A�j���[�V�����̍X�V.
void CBlendSprite::AnimUpdate()
{
	if (m_pSpriteData->IsAnimation == false) return;

	if (m_pSpriteData->FrameCount % m_pSpriteData->FrameTime == 0) {
		// �A�j���[�V�����ԍ����Z�o.
		m_pSpriteData->PatternNo.x = m_pSpriteData->AnimNumber % m_pSpriteData->PatternMax.x;
		m_pSpriteData->PatternNo.y = m_pSpriteData->AnimNumber / m_pSpriteData->PatternMax.x;
		m_pSpriteData->AnimNumber++;	// �A�j���[�V�����ԍ������Z.
		if (m_pSpriteData->AnimNumber >= m_pSpriteData->SState.AnimNum) {
			// �A�j���[�V�����ԍ����ő�A�j���[�V��������葽�����.
			// ����������.
			m_pSpriteData->FrameCount = 0;
			m_pSpriteData->AnimNumber = 0;
		}
	}
	m_pSpriteData->FrameCount += m_pSpriteData->FrameCountSpeed;
	// UV���W�ɕϊ�.
	m_pSpriteData->UV = ConvertIntoUV();
}

void CBlendSprite::CreateVERTEX(const float& w, const float& h, const float& u, const float& v)
{
	switch (m_pSpriteData->enLocalPosition)
	{
	case ELocalPosition::LeftUp:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(0.0f,    h, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w,    h, 0.0f), D3DXVECTOR2(u,    v) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w, 0.0f, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�S(�E��).
		break;
	case ELocalPosition::Left:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(0.0f, -h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(0.0f,  h / 2, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w, -h / 2, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w,  h / 2, 0.0f), D3DXVECTOR2(u,    v) };	//���_�S(�E��).
		break;
	case ELocalPosition::LeftDown:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(0.0f,    -h, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(0.0f,  0.0f, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w,    -h, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w,  0.0f, 0.0f), D3DXVECTOR2(u,    v) };	//���_�S(�E��).
		break;
	case ELocalPosition::Down:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w / 2,    -h, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w / 2,  0.0f, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w / 2,    -h, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w / 2,  0.0f, 0.0f), D3DXVECTOR2(u,    v) };	//���_�S(�E��).
		break;
	case ELocalPosition::RightDown:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w,   -h, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w, 0.0f, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(0.0f,   -h, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(u,    v) };	//���_�S(�E��).
		break;
	case ELocalPosition::Right:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w, -h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w,  h / 2, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(0.0f, -h / 2, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(0.0f,  h / 2, 0.0f), D3DXVECTOR2(u,    v) };	//���_�S(�E��).
		break;
	case ELocalPosition::RightUp:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w,    h, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(0.0f,    h, 0.0f), D3DXVECTOR2(u,    v) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�S(�E��).
		break;
	case ELocalPosition::Up:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w / 2,    h, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w / 2, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w / 2,    h, 0.0f), D3DXVECTOR2(u,    v) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w / 2, 0.0f, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�S(�E��).
		break;
	case ELocalPosition::Center:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(-w / 2, -h / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(-w / 2,  h / 2, 0.0f), D3DXVECTOR2(0.0f,    v) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(w / 2, -h / 2, 0.0f), D3DXVECTOR2(u, 0.0f) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(w / 2,  h / 2, 0.0f), D3DXVECTOR2(u,    v) };	//���_�S(�E��).
		break;
	default:
		break;
	}
}