//-------------------------------------------------.
// テクスチャ.
//-------------------------------------------------.
Texture2D		g_Texture	: register(t0);
//-------------------------------------------------.
// サンプラ.
//-------------------------------------------------.
SamplerState	g_samLinear	: register(s0);

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
cbuffer global
{
	matrix	g_WVP;		// ワールド、ビュー、プロジェクション行列.
	float4	g_Color;	// 色.
	float	g_fAlpha;	// 透過値.
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
struct VS_OUTPUT
{
	float4	Pos : SV_Position;
	float2	Tex	: TEXCOORD;
};

//-------------------------------------------------.
// 頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	return output;
}

//-------------------------------------------------.
// ピクセルシェーダー.
//-------------------------------------------------.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	float4 color = g_Color * g_Texture.Sample(g_samLinear, input.Tex);
	color.a *= g_fAlpha;
	clip(color.a);
	return color;
}