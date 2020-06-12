//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D g_Texture : register(t0);
//����ׂ́Aڼ޽� s(n).
SamplerState g_samLinear : register(s0);

//�ݽ����ޯ̧.
cbuffer global : register(b0)
{
    matrix g_mW; // ܰ��ލs��.
    matrix g_mWVP; // ܰ��ލs��.
    float4 g_Color; // �J���[.
    float2 g_vUV; // UV���W.
    float g_fViewPortW; // �r���[�|�[�g��.
    float g_fViewPortH; // �r���[�|�[�g����.
};

//�\����.
struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, g_mWVP);
    output.Tex = Tex;
	// UV���W�����炷.
    output.Tex.x += g_vUV.x;
    output.Tex.y += g_vUV.y;

    return output;
}


// ���_�V�F�[�_.
VS_OUTPUT VS_MainUI(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, g_mW);

	// �X�N���[�����W�ɍ��킹��v�Z,
    output.Pos.x = (output.Pos.x / g_fViewPortW) * 2.0f - 1.0f;
    output.Pos.y = 1.0f - (output.Pos.y / g_fViewPortH) * 2.0f;

    output.Tex = Tex;
	
	// UV���W�����炷.
    output.Tex.x += g_vUV.x;
    output.Tex.y += g_vUV.y;

    return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    float4 color = g_Texture.Sample(g_samLinear, input.Tex);
    color.a *= g_Color.a;
    return color;
}