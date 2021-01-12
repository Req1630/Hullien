//-------------------------------------------------.
// テクスチャ.
//-------------------------------------------------.
Texture2D g_Texture			: register(t0);
//-------------------------------------------------.
// サンプラ.
//-------------------------------------------------.
SamplerState g_SamLinear	: register(s0);

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
cbuffer global : register(b0)
{
	matrix g_mWVP;
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
// 頂点入力用.
struct VS_INPUT
{
	float4 Pos	: POSITION;	// 頂点座標.
	float4 Norm	: NORMAL;	// 法線.
	float2 Tex	: TEXCOORD;	// テクスチャ座標.
};
// 頂点出力用.
struct VS_OUTPUT
{
	float4 Pos	: SV_Position;	// WVPでの座標.
	float2 Tex	: TEXCOORD0;	// ワールド座標.
};

//-------------------------------------------------.
// 頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_Main( VS_INPUT input )
{
	VS_OUTPUT Out	= (VS_OUTPUT) 0;
	input.Pos.w		= 1.0f;
	Out.Pos			= mul(input.Pos, g_mWVP);
	Out.Tex			= input.Tex;
	return Out;
}

//-------------------------------------------------.
// ピクセルシェーダー.
//-------------------------------------------------.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	return g_Texture.Sample(g_SamLinear, input.Tex);
}