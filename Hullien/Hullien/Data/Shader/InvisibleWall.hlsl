//-------------------------------------------------.
// �e�N�X�`��.
//-------------------------------------------------.
Texture2D g_Texture		: register(t0); // ���f���̃e�N�X�`��.
Texture2D g_MaskTexture		: register(t1); // ���f���̃e�N�X�`��.
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
	float4 color = float4(0.2f, 0.2f, 1.0f, 1.0f);
	float d = distance(input.PosW, g_PlayerPos);
	
	if( d>=10.0f )
	{
		discard;
	}
	else
	{
		float p =(10.0f-d)*0.1f;
		float u = distance(g_PlayerPos.xz, input.PosW.xz)*0.1f;
		float v = (g_PlayerPos.y-input.PosW.y)*0.1f;
		u -= 0.06f;
		v += 0.5f;
		float mask = g_MaskTexture.Sample(g_SamLinear, float2(u, v)).r;
		float maskAlpha = saturate(mask+(p*2.0f-1.0f));
		float alpha = 1-g_Texture.Sample(g_SamLinear, float2(u, v)).r;
		float a = p;
		color.a = alpha*p;
		color = color*maskAlpha+color*(1-color.a);
		//// AT�t�B�[���h.
		// color = float4(1.0f, 0.5f, 0.0f, 1.0f);
		//float alpha = frac(d)+0.2f;
		//color.a *= alpha;
	}
	
	return color;
}