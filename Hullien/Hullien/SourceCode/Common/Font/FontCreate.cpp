#include "FontCreate.h"

CFontCreate::CFontCreate()
	: m_pDevice11	( nullptr )
	, m_pContext11	( nullptr )
{
	FontAvailable();	// �t�H���g�𗘗p�\�ɂ���.
}

CFontCreate::CFontCreate( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
	: m_pDevice11	( pDevice11 )
	, m_pContext11	( pContext11 )
{
	FontAvailable();	// �t�H���g�𗘗p�\�ɂ���.
}

CFontCreate::~CFontCreate()
{
	DESIGNVECTOR design;
	RemoveFontResourceEx( 
		TEXT(FONT_PATH),
		FR_PRIVATE,
		&design
	);
}

//-----------------------------------.
// �t�H���g�摜�̍쐬.
//-----------------------------------.
HRESULT CFontCreate::CreateFontTexture2D( const char* c, ID3D11Texture2D** textur2D )
{
	if( m_pDevice11 == nullptr ) return E_FAIL;
	if( m_pContext11 == nullptr ) return E_FAIL;

	HRESULT hr;
	
	// �����R�[�h�擾
	UINT code = 0;
#if _UNICODE
	// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ��ł�
	code = (UINT)*c;
#else
	// �}���`�o�C�g�����̏ꍇ�A
	// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
	// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]�ł�
	if(IsDBCSLeadByte(*c))
		code = (BYTE)c[0]<<8 | (BYTE)c[1];
	else
		code = c[0];
#endif

	HDC hdc = nullptr;
	HFONT hFont = nullptr;
	HFONT oldFont = nullptr;

	// �t�H���g�̐���
	LOGFONT lf = { 
		FONT_BMP_SIZE,
		0,
		0, 
		0, 
		0, 
		0,
		0,
		0, 
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		TEXT("Data\\Font\\mplus-1p-medium.ttf")
	};

	hFont = CreateFontIndirect( &lf );

	// �f�o�C�X�R���e�L�X�g�擾
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	hdc = GetDC( NULL );
	oldFont = (HFONT)SelectObject( hdc, hFont );

	// �t�H���g�r�b�g�}�b�v�擾
	TEXTMETRIC TM;
	GetTextMetrics( hdc, &TM );
	GLYPHMETRICS GM;
	CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD size = GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat );
	BYTE* ptr = new BYTE[size];
	GetGlyphOutline( hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat );

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject( hdc, oldFont );
	DeleteObject( hFont );
	ReleaseDC( NULL, hdc );


	//--------------------------------
	// �������݉\�e�N�X�`���쐬
	// CPU�ŏ������݂��ł���e�N�X�`�����쐬
	// �e�N�X�`���쐬
	D3D11_TEXTURE2D_DESC desc = CreateDesc( GM.gmCellIncX, TM.tmHeight );

	hr = m_pDevice11->CreateTexture2D( &desc, 0, textur2D );

	D3D11_MAPPED_SUBRESOURCE hMappedResource;
	hr = m_pContext11->Map( 
		*textur2D,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&hMappedResource );

	// �����ŏ�������
	BYTE* pBits = (BYTE*)hMappedResource.pData;
	
	// �t�H���g���̏�������
	// iOfs_x, iOfs_y : �����o���ʒu(����)
	// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
	int iOfs_x = GM.gmptGlyphOrigin.x;
	int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	int iBmp_w = GM.gmBlackBoxX + ( 4 - ( GM.gmBlackBoxX % 4 ) ) % 4 ;
	int iBmp_h = GM.gmBlackBoxY;
	int Level = 17;
	int x, y;
	DWORD Alpha, Color;
	memset( pBits, 0, hMappedResource.RowPitch * TM.tmHeight );
	for( y = iOfs_y; y < iOfs_y + iBmp_h; y++ ){
		for( x = iOfs_x; x < iOfs_x + iBmp_w; x++){
			Alpha = ( 255 * ptr[x - iOfs_x + iBmp_w * ( y - iOfs_y )] ) / ( Level - 1 );
			Color = 0x00ffffff | ( Alpha << 24 );

			memcpy( (BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof(DWORD) );
		}
	}
	m_pContext11->Unmap( *textur2D, 0 );

	delete[] ptr;

	return S_OK;

}

//-----------------------------------.
// �t�H���g�𗘗p�\�ɂ���.
//-----------------------------------.
int CFontCreate::FontAvailable()
{
	DESIGNVECTOR design;

	return AddFontResourceEx( 
		TEXT(FONT_PATH),	// �t�H���g���\�[�X��.
		FR_PRIVATE,			// �v���Z�X�I�����ɃC���X�g�[�������t�H���g���폜.
		&design );			// �t�H���g�\����.
}

//-----------------------------------.
// Textur2D_Desc���쐬.
//-----------------------------------.
D3D11_TEXTURE2D_DESC CFontCreate::CreateDesc( UINT width, UINT height )
{
	D3D11_TEXTURE2D_DESC desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.Width				= width;
	desc.Height				= height;
	desc.MipLevels			= 1;
	desc.ArraySize			= 1;
	desc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)�^�C�v
	desc.SampleDesc.Count	= 1;
	desc.Usage				= D3D11_USAGE_DYNAMIC;			// ���I�i�������݂��邽�߂̕K�{�����j
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_���\�[�X�Ƃ��Ďg��
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;		// CPU����A�N�Z�X���ď�������OK

	return desc;
}