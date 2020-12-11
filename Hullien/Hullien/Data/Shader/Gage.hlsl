//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D		g_Texture : register( t0 );
//����ׂ́Aڼ޽� s(n).
SamplerState	g_samLinear		: register( s0 );

//�ݽ����ޯ̧.
cbuffer global : register(b0)
{
	matrix	g_mW			        : packoffset(c0);	// ܰ��ލs��.
	float4	g_Color			        : packoffset(c4);	// �J���[.
	float2	g_fViewPort		        : packoffset(c5);	// �r���[�|�[�g��.
    float   g_fImageWidth           : packoffset(c6);	// �摜��.
    float   g_fDispXPos             : packoffset(c7);	// �\��X�ʒu.
    float   g_fMaxGaugeValue        : packoffset(c8);	// �Q�[�W�ő�l.
    float   g_fNowGaugeRangeValue   : packoffset(c9);	// �Q�[�W���݂̒l.
};

//�\����.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float4 Color		: COLOR;
	float2 Tex			: TEXCOORD;
};

// ���_�V�F�[�_.
VS_OUTPUT VS_Main( 
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul( Pos, g_mW );

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = ( output.Pos.x / g_fViewPort.x ) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - ( output.Pos.y / g_fViewPort.y ) * 2.0f;

	output.Tex = Tex;

	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 maskColor = g_Texture.Sample(g_samLinear, input.Tex);
	float4 texColor = float4(0.5f, 0.f, 0.f, 1.0f);
	
	if( maskColor.a <= 0.0f ) discard;
	
	float maskAlpha = saturate(length(maskColor) + (g_fNowGaugeRangeValue * 2.0f - 1.0f));
	float4 outColor = texColor * maskAlpha + texColor * (1 - texColor.a);

	return outColor;
}