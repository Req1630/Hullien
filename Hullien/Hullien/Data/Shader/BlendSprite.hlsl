// 以下の記事を参考にブレンドシェーダーを実装.
// https://pgming-ctrl.com/directx11/shader-blend-test/

//-------------------------------------------------.
// テクスチャ.
//-------------------------------------------------.
Texture2D g_DestTexture	: register(t0);	// 元テクスチャ.
Texture2D g_SrcTexture	: register(t1);	// 合成する際に使用するテクスチャ.
//-------------------------------------------------.
// サンプラー.
//-------------------------------------------------.
SamplerState g_SamLinear : register(s0);

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
cbuffer global : register(b0)
{
	matrix g_mW			: packoffset(c0);	// ワールド行列.
	matrix g_mWVP		: packoffset(c4);	// ワールド・ビュー・プロジェクション行列.
	float4 g_Color		: packoffset(c8);	// カラー.
	float2 g_UV			: packoffset(c9);	// UV座標.
	float2 g_ViewPort	: packoffset(c10);	// ビューポートのサイズ.
};

//-------------------------------------------------.
//構造体.
//-------------------------------------------------.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
};

//-------------------------------------------------.
// 頂点シェーダー(3Dポリゴン用).
//-------------------------------------------------.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;
	output.Tex.x += g_UV.x;
	output.Tex.y += g_UV.y;

	return output;
}

//-------------------------------------------------.
// 頂点シェーダ(2Dスプライト用).
//-------------------------------------------------.
VS_OUTPUT VS_MainUI(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mW);

	// スクリーン座標に合わせる計算,
	output.Pos.x = (output.Pos.x / g_ViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_ViewPort.y) * 2.0f;

	output.Tex.x = (output.Pos.x + 1.0) / 2;
	output.Tex.y = (-output.Pos.y + 1.0) / 2;

	// UV座標をずらす.
	output.Tex.x += g_UV.x;
	output.Tex.y += g_UV.y;

	return output;
}

//---------------------------------------------.
// ピクセルシェーダ.
//---------------------------------------------.

// 通常.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 color = g_DestTexture.Sample(g_SamLinear, input.Tex);
	color *= g_Color;
	clip(color.a);
	return color;
}

// ハードライト.
float4 PS_HardLight( VS_OUTPUT input ) : SV_TARGET
{
	float4 dest = g_DestTexture.Sample(g_SamLinear, input.Tex); // 規定色.
	float4 src = g_SrcTexture.Sample(g_SamLinear, input.Tex); // 合成色.
	
	float4 result;
	
	if (dest.r > 0.5) result.r = dest.r * src.r * 2;
	else result.r = 2 * (dest.r + src.r - dest.r * src.r) - 1.0;
	if (dest.g > 0.5) result.g = dest.g * src.g * 2;
	else result.g = 2 * (dest.g + src.g - dest.g * src.g) - 1.0;
	if (dest.b > 0.5) result.b = dest.b * src.b * 2;
	else result.b = 2 * (dest.b + src.b - dest.b * src.b) - 1.0;
	result.a = src.a * g_Color.a;
	
	return result;
}
