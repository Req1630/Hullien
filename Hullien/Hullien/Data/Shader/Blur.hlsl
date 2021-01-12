//-------------------------------------------------.
// �e�N�X�`��.
//-------------------------------------------------.
Texture2D g_Texture : register(t0);
//-------------------------------------------------.
// �T���v��.
//-------------------------------------------------.
SamplerState g_SamLinear : register(s0);

//-------------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-------------------------------------------------.
cbuffer cBuffer : register(b0)
{
	matrix g_mW			: packoffset(c0); // ���[���h�s��.
	float2 g_vViewPort	: packoffset(c4); // �r���[�|�[�g��.
	float2 g_vPixelSize	: packoffset(c5); // �s�N�Z���̃T�C�Y.
	float2 g_Ratio		: packoffset(c6); // ��ʂ̔䗦.
};

//-------------------------------------------------.
// �\����.
//-------------------------------------------------.
struct VS_OUTPUT
{
	float4 Pos : SV_Position;
	float2 Tex : TEXCOORD0;
};

//-------------------------------------------------.
// ���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mW);

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = (output.Pos.x / g_vViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_vViewPort.y) * 2.0f;
	output.Tex = Tex;
	
	return output;
}

//-------------------------------------------------.
// �s�N�Z���V�F�[�_.
//-------------------------------------------------.
// �������ɂڂ���.
float4 PS_HorizontalBlur(VS_OUTPUT input) : SV_Target
{
	float4 color = float4( 0.0f, 0.0f, 0.0f, 1.0f );
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2(-3.0f,0.0f)) * 0.053;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2(-2.0f,0.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2(-1.0f,0.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 0.0f,0.0f)) * 0.240;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 1.0f,0.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 2.0f,0.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 3.0f,0.0f)) * 0.053;
	
	return color;
}

// �c�����ɂڂ���.
float4 PS_VerticalBlur(VS_OUTPUT input) : SV_Target
{
	float4 color = float4( 0.0f, 0.0f, 0.0f, 1.0f );
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f,-3.0f)) * 0.053;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f,-2.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f,-1.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 0.0f)) * 0.240;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 1.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 2.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 3.0f)) * 0.053;
	
	return color;
}
