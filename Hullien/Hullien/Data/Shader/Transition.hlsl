//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D g_MaskTexture	: register(t0);
Texture2D g_Texture		: register(t1);
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState g_SamLinear : register(s0);

cbuffer CBufferPerInit : register(b0)
{
	matrix g_mW			: packoffset( c0 ); // ﾜｰﾙﾄﾞ行列.
	float2 g_fViewPort	: packoffset( c4 ); // UV座標.
}

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer CBufferPerFrame : register(b1)
{
    float Value : packoffset( c0 ); // ﾜｰﾙﾄﾞ行列.
};

//構造体.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
};

// 頂点シェーダ.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mW);

	// スクリーン座標に合わせる計算,
	output.Pos.x = (output.Pos.x / g_fViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_fViewPort.y) * 2.0f;

	output.Tex = Tex;

	return output;
}

// ピクセルシェーダ.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    float maskColor = 1 - g_MaskTexture.Sample( g_SamLinear, input.Tex ).r;
	float4 texColor = g_Texture.Sample( g_SamLinear, input.Tex );

	float maskAlpha = saturate(maskColor + (Value * 2.0f - 1.0f ));
	float4 outColor = texColor * maskAlpha + texColor * ( 1 - texColor.a );
	
	clip( outColor.a - 0.001f );
	
    return outColor;
}