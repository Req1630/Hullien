//-------------------------------------------------.
// �e�N�X�`��.
//-------------------------------------------------.
Texture2D g_Texture		: register(t0);	// ���f���̃e�N�X�`��.
Texture2D g_MaskTexture	: register(t1);	// �}�X�N�p�e�N�X�`��.
//-------------------------------------------------.
// �T���v��.
//-------------------------------------------------.
SamplerState g_SamLinear : register(s0); // �ʏ�T���v��.

//-------------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-------------------------------------------------.
// �t���[���P��.
cbuffer per_frame : register(b0)
{
	matrix g_mW;		// ���[���h�s��.
	matrix g_mWVP;		// ���[���h,�r���[,�v���W�F�N�V�����̍����s��.
	matrix g_mVPT; // �v���C���[�ʒu.
	float4 g_PlayerPos; // �v���C���[�ʒu.
	float4 g_Color;		// �F.
};

//-------------------------------------------------.
// �\����.
//-------------------------------------------------.
// ���_�C���v�b�g.
struct VS_INPUT
{
	float4 Pos	: POSITION;	// ���W.
};
// ���_�A�E�g�v�b�g.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;	// WVP�ł̍��W.
	float4 PosW		: Position;		// W�ł̍��W.
	float4 Color	: COLOR;		// �F.
	float4 Tex		: TEXCOORD; // �F.
};

//-------------------------------------------------.
//	���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_Main( VS_INPUT input )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos		= mul(input.Pos, g_mWVP);	// WVP���W.
	Out.PosW	= mul(input.Pos, g_mW);		// world���W.
	Out.Tex		= mul(input.Pos, g_mVPT); // world���W.
	return Out;
}

//-------------------------------------------------.
//	�s�N�Z���V�F�[�_�[.
//-------------------------------------------------.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 color = float4(0.2f, 0.2f, 1.5f, 0.7f);
	float d = distance(input.PosW, g_PlayerPos);
	const float LENGHT = 10.0f;
	const float POWER = 1.0f/LENGHT;
	if( d>=LENGHT )
	{
		discard;
	}
	else
	{
		float p = (LENGHT-d)*POWER;
		float u = distance(g_PlayerPos.xz, input.PosW.xz)*POWER;
		float v = (g_PlayerPos.y-input.PosW.y)*POWER;
		u -= 0.06f;
		v += 0.5f;
		float mask = 1-g_MaskTexture.Sample(g_SamLinear, float2(u, v)).r;
		float maskAlpha = saturate(mask+(p*2.0f-1.0f));
		color = color*maskAlpha+color*(1-color.a);
		
		//// AT�t�B�[���h.
		// color = float4(1.0f, 0.5f, 0.0f, 1.0f);
		//float alpha = frac(d)+0.2f;
		//color.a *= alpha;
	}
	
	return color;
}