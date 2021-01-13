//-------------------------------------------------.
// テクスチャ.
//-------------------------------------------------.
Texture2D g_Texture		: register(t0); // モデルのテクスチャ.
Texture2D g_MaskTexture		: register(t1); // モデルのテクスチャ.
//-------------------------------------------------.
// サンプラ.
//-------------------------------------------------.
SamplerState g_SamLinear : register(s0); // 通常サンプラ.

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
// フレーム単位.
cbuffer per_frame : register(b0)
{
	matrix g_mW;		// ワールド行列.
	matrix g_mWVP;		// ワールド,ビュー,プロジェクションの合成行列.
	matrix g_mVPT; // プレイヤー位置.
	float4 g_PlayerPos; // プレイヤー位置.
	float4 g_Color;		// 色.
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
// 頂点インプット.
struct VS_INPUT
{
	float4 Pos	: POSITION;	// 座標.
};
// 頂点アウトプット.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;	// WVPでの座標.
	float4 PosW		: Position;		// Wでの座標.
	float4 Color	: COLOR;		// 色.
	float4 Tex		: TEXCOORD; // 色.
};

//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_Main( VS_INPUT input )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos		= mul(input.Pos, g_mWVP);	// WVP座標.
	Out.PosW	= mul(input.Pos, g_mW);		// world座標.
	Out.Tex		= mul(input.Pos, g_mVPT); // world座標.
	return Out;
}

//-------------------------------------------------.
//	ピクセルシェーダー.
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
		//// ATフィールド.
		// color = float4(1.0f, 0.5f, 0.0f, 1.0f);
		//float alpha = frac(d)+0.2f;
		//color.a *= alpha;
	}
	
	return color;
}