#ifndef SPRITE_STATE_H
#define SPRITE_STATE_H

#include "..\Common.h"

// �������\����(float�^).
struct WHDIZE_FLOAT
{
	float w;	// ��.
	float h;	// ����.
};

enum class enLocalPosition : unsigned char
{
	None,

	LeftUp = 0,	// ����.
	Left,		// ��.
	LeftDown,	// ����.
	Down,		// ��.
	RightDown,	// �E��.
	Right,		// �E.
	RightUp,	// �E��.
	Up,			// ��.
	Center,		// ����.

	Max,
} typedef ELocalPosition;

// �X�v���C�g�\����.
struct stSpriteState
{
	int				LocalPosNum;// ���[�J�����W��
	WHDIZE_FLOAT	Disp;		// �\����,����.
	WHDIZE_FLOAT	Base;		// ���摜�̕�,����.
	WHDIZE_FLOAT	Stride;		// 1�R�}������̕�,����.
	D3DXVECTOR3		vPos;		// ���W.
	int				AnimNum;	// �A�j���[�V������.

	enum enSPRITE_STATE : unsigned char
	{
		None,

		enLocalPosNum = 0,
		enDisp_w,
		enDisp_h,
		enBase_w,
		enBase_h,
		enStride_w,
		enStride_h,
		envPos_x,
		envPos_y,
		envPos_z,
		enAnimNum,

		MAX = enAnimNum,
	} typedef ESPRITE_STATE;
} typedef SSpriteState;

// �X�v���C�g���.
struct stSpriteData
{
	ID3D11InputLayout*	pVertexLayout;		// ���_���C�A�E�g.
	ID3D11Buffer*		pVertexBuffer;		// ���_�o�b�t�@.
	ID3D11Buffer*		pVertexBufferUI;	// ���_�o�b�t�@.

	ID3D11ShaderResourceView*	pTexture;	// �e�N�X�`��.

	SSpriteState	SState;				// �X�v���C�g���.
	POINT			PatternNo;			// �p�^�[���ԍ�.
	POINT			PatternMax;			// �p�^�[���ő吔.
	D3DXVECTOR2		UV;					// �e�N�X�`��UV���W.
	int				AnimNumber;			// �A�j���[�V�����i���o�[.
	int				FrameCount;			// �t���[���J�E���g.
	int				FrameCountSpeed;	// �t���[���J�E���g���x.
	int				FrameTime;			// �t���[���^�C��.
	bool			IsAnimation;		// �A�j���[�V�������邩.
	enLocalPosition enLocalPosition;	// ���[�J�����W�̎��.

	stSpriteData()
		: pVertexLayout		( nullptr )
		, pVertexBuffer		( nullptr )
		, pVertexBufferUI	( nullptr )
		, pTexture			( nullptr )
		, SState			()
		, PatternNo			{ 1, 1 }
		, PatternMax		{ 1, 1 }
		, UV				( 1.0f, 1.0f )
		, AnimNumber		( 0 )
		, FrameCount		( 0 )
		, FrameCountSpeed	( 1 )
		, FrameTime			( 20 )
		, IsAnimation		( true )
		, enLocalPosition	( ELocalPosition::None )
	{}
	~stSpriteData()
	{
		SAFE_RELEASE( pTexture );
		SAFE_RELEASE( pVertexBufferUI );
		SAFE_RELEASE( pVertexBuffer );
		SAFE_RELEASE( pVertexLayout );
	}
	void Release()
	{
		SAFE_RELEASE( pTexture );
		SAFE_RELEASE( pVertexBufferUI );
		SAFE_RELEASE( pVertexBuffer );
		SAFE_RELEASE( pVertexLayout );
	}
} typedef SSpriteData;

#endif	// #ifndef SPRITE_STATE_H.