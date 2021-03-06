//-------------------------------------------------.
// テクスチャ.
//-------------------------------------------------.
Texture2D g_Texture		: register(t0); // モデルのテクスチャ.
Texture2D g_ShadowMap1	: register(t1); // シャドウテクスチャ.
Texture2D g_ShadowMap2	: register(t2); // シャドウテクスチャ.
Texture2D g_ShadowMap3	: register(t3); // シャドウテクスチャ.
Texture2D g_ShadowMap4	: register(t4); // シャドウテクスチャ.
Texture2D g_ToonMap		: register(t5); // toonシェーダー用のテクスチャ.
Texture2D g_FogTexture	: register(t6); // フォグ用のテクスチャ.
//-------------------------------------------------.
// サンプラ.
//-------------------------------------------------.
SamplerState g_SamLinear		: register(s0);	// 通常サンプラ.
SamplerState g_ToonSamLinear	: register(s1);	// toon用サンプラ.
SamplerState g_ShadowSamLinear	: register(s2);	// シャドウ用サンプラ.

struct FOG
{
	float4  FogTex;		// フォグのテクスチャ座標.
	float4  FogColor;	// フォグの色.
	float	MinHeight;	// フォグの最小高さ.
	float	MaxHeight;	// フォグの最大高さ.
	float	TexScale;	// フォグのテクセルスケール.
};

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
// メッシュ単位.
cbuffer per_mesh : register(b0)
{
	matrix g_mW;	// ワールド行列.
	matrix g_mWVP;	// ワールド,ビュー,プロジェクションの合成行列.
};
// マテリアル単位.
cbuffer per_material : register(b1)
{
	float4 g_vAmbient;	// アンビエント色(環境色).
	float4 g_vDiffuse;	// ディヒューズ色(拡散反射色).
	float4 g_vSpecular; // スペキュラ色(鏡面反射色).
};
// フレーム単位.
cbuffer per_frame : register(b2)
{
	float4 g_vCamPos;		// カメラ位置.
	float4 g_vLightPos;		// ライト位置.
	float4 g_vLightDir;		// ライト方向.
	matrix g_mLightRot;		// ライト回転行列.
	float4 g_fIntensity;	// ライト強度(明るさ). ※xのみ使用する.
	float4 g_Color;			// 色.
	matrix g_LightWVP[4];	// ライト行列4つ.
	float4 g_SpritPos;		// ライトの区切り座標.
	float4 g_IsShadow;		// ライトを使用するか.
	FOG	g_Fog;
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
// 頂点インプット.
struct VS_INPUT
{
	float4 Pos	: POSITION;	// 座標.
	float4 Norm : NORMAL;	// 法線.
	float2 Tex	: TEXCOORD;	// テクスチャ座標.
};
// 頂点アウトプット.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;	// WVPでの座標.
	float4 PosW			: Position;		// Wでの座標.
	float4 Color		: COLOR;		// 色.
	float3 LightDir		: TEXCOORD0;	// ライト方向.
	float3 Normal		: TEXCOORD1;	// 法線.
	float3 EyeVector	: TEXCOORD2;	// 視点ベクトル.
	float2 Tex			: TEXCOORD3;	// テクスチャ座標.
	float4 SpritPos		: TEXCOORD4;	// 区切り座標.
	float4 ZDepth[4]	: TEXCOORD5;	// 各深度値.
};

//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

	Out.Pos			= mul(input.Pos, g_mWVP);	// WVP座標.
	Out.PosW		= mul(input.Pos, g_mW);		// world座標.
	Out.Normal		= mul(input.Norm.xyz, (float3x3) g_mW); // 法線.
	Out.LightDir	= normalize(g_vLightDir).xyz; // ライト方向.
	Out.EyeVector	= normalize(g_vCamPos - Out.PosW).xyz; // 視点ベクトル.
	Out.Tex = input.Tex; //　テクスチャ座標.
	// 各ライトからの深度値の取得.
	Out.ZDepth[0] = mul(input.Pos, g_LightWVP[0]);
	Out.ZDepth[1] = mul(input.Pos, g_LightWVP[1]);
	Out.ZDepth[2] = mul(input.Pos, g_LightWVP[2]);
	Out.ZDepth[3] = mul(input.Pos, g_LightWVP[3]);
	Out.SpritPos = g_SpritPos;
	
	return Out;
}

struct PS_OUTPUT
{
	float4 Color : SV_Target0;
	float4 Normal : SV_Target1;
	float4 ZDepth : SV_Target2;
};

float4 outRBGA(float depth)
{
	// R成分抽出
	float R = round(depth * 256.0f) / 256.0f;
	// G成分抽出
	float Def = depth - R;
	float G = round(Def * 65536.0f) / 256.0f;
	// B成分抽出
	Def -= G / 256.0f;
	float B = round(Def * 65536.0f * 256.0f) / 256.0f;
	// A成分抽出
	Def -= B / 65536.0f;
	float A = round(Def * 65536.0f * 65536.0f) / 256.0f;
	
	return float4(R, G, B, A);
}

//-------------------------------------------------.
//	ピクセルシェーダー.
//-------------------------------------------------.
PS_OUTPUT PS_Main(VS_OUTPUT input) : SV_Target
{
	// モデルのテクスチャ色を取得.
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);
	color *= g_Color;
	
	// 各ピクセル位置までの距離.
	float dist = input.Pos.w; // ビュー空間でのZ座標.
	float shadowColor = 1.0f;
	float zValue = 0.0f; // 深度値.
	float baias = 0.01f;
	float2 cord = float2(0.0f, 0.0f);
	if (dist < input.SpritPos.x)
	{
		zValue = input.ZDepth[0].z / input.ZDepth[0].w;
		cord.x = (1.0f + input.ZDepth[0].x / input.ZDepth[0].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[0].y / input.ZDepth[0].w) * 0.5f;
		float4 depthColor = g_ShadowMap1.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.y)
	{
		zValue = input.ZDepth[1].z / input.ZDepth[1].w;
		cord.x = (1.0f + input.ZDepth[1].x / input.ZDepth[1].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[1].y / input.ZDepth[1].w) * 0.5f;
		float4 depthColor = g_ShadowMap2.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.z)
	{
		zValue = input.ZDepth[2].z / input.ZDepth[2].w;
		cord.x = (1.0f + input.ZDepth[2].x / input.ZDepth[2].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[2].y / input.ZDepth[2].w) * 0.5f;
		float4 depthColor = g_ShadowMap3.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else
	{
		zValue = input.ZDepth[3].z / input.ZDepth[3].w;
		cord.x = (1.0f + input.ZDepth[3].x / input.ZDepth[3].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[3].y / input.ZDepth[3].w) * 0.5f;
		float4 depthColor = g_ShadowMap4.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	if (g_IsShadow.x >= 1.0f) color.rgb *= shadowColor;
	
	//-----トゥーン処理------.
	// ハーフランバート拡散照明によるライティング計算
	float p = dot(input.Normal, input.LightDir);
	p = p * 0.5f + 0.5f;
	p = p * p;
	// 計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
	float4 toonColor = g_ToonMap.Sample(g_ToonSamLinear, float2(p, 0.0f));
	color.rgb *= toonColor.rgb * g_fIntensity.x + + g_vSpecular.rgb;;
	
	//-----高さフォグ処理------.
	// fogテクスチャの座標を取得、計算.
	const float fogColor =
		(g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.xy).r +
		 g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.zw).r) * 0.5f;
	float alpha = clamp((input.PosW.y - g_Fog.MinHeight) / (g_Fog.MaxHeight - g_Fog.MinHeight), 0.0f, 1.0f);
	float alphas = 1.0f - (1.0f - alpha) * fogColor;

	color.rgb = color.rgb * alphas + g_Fog.FogColor.rgb * (1.0f - alphas);
	float depthAlpha = 1.0f;
	if (length(input.Normal) <= 0.0){
		color = g_Texture.Sample(g_SamLinear, input.Tex) * g_Color;
		input.Pos.z = 0.0f;
		input.Pos.w = 1.0f;
		depthAlpha = 0.0f;
	}
	
	PS_OUTPUT output = (PS_OUTPUT) 0;
	output.Color = color;
	output.Normal = float4(input.Normal, 1.0f);
	float z = input.Pos.z/input.Pos.w;
	output.ZDepth = float4(z, z, z, depthAlpha);
	if (output.Color.a <= 0.0f) discard;
	
	return output;
}

//========= テクスチャ無し用 ========================================
//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm : NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

	Out.Pos			= mul(Pos, g_mWVP); // WVP座標.
	Out.PosW		= mul(Pos, g_mW);	// world座標.
	Out.Normal		= mul(Norm.xyz, (float3x3) g_mW); // 法線.
	Out.LightDir	= normalize(g_vLightDir).xyz; // ライト方向.
	Out.EyeVector	= normalize(g_vCamPos - Out.PosW).xyz; // 視点ベクトル.

	// 各ライトからの深度値の取得.
	Out.ZDepth[0] = mul(Pos, g_LightWVP[0]);
	Out.ZDepth[1] = mul(Pos, g_LightWVP[1]);
	Out.ZDepth[2] = mul(Pos, g_LightWVP[2]);
	Out.ZDepth[3] = mul(Pos, g_LightWVP[3]);
	Out.SpritPos[0] = g_SpritPos.x;
	Out.SpritPos[1] = g_SpritPos.y;
	Out.SpritPos[2] = g_SpritPos.z;
	Out.SpritPos[3] = g_SpritPos.w;
	
	return Out;
}

//-------------------------------------------------.
//	ピクセルシェーダー.
//-------------------------------------------------.
PS_OUTPUT PS_NoTex(VS_OUTPUT input) : SV_Target
{
	// モデルのテクスチャ色を取得.
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	color *= g_Color;
	
	// 各ピクセル位置までの距離.
	float dist = input.Pos.w; // ビュー空間でのZ座標.
	float shadowColor = 1.0f;
	float zValue = 0.0f; // 深度値.
	float baias = 0.01f;
	float2 cord = float2(0.0f, 0.0f);
	if (dist < input.SpritPos.x)
	{
		zValue = input.ZDepth[0].z / input.ZDepth[0].w;
		cord.x = (1.0f + input.ZDepth[0].x / input.ZDepth[0].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[0].y / input.ZDepth[0].w) * 0.5f;
		float4 depthColor = g_ShadowMap1.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.y)
	{
		zValue = input.ZDepth[1].z / input.ZDepth[1].w;
		cord.x = (1.0f + input.ZDepth[1].x / input.ZDepth[1].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[1].y / input.ZDepth[1].w) * 0.5f;
		float4 depthColor = g_ShadowMap2.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.z)
	{
		zValue = input.ZDepth[2].z / input.ZDepth[2].w;
		cord.x = (1.0f + input.ZDepth[2].x / input.ZDepth[2].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[2].y / input.ZDepth[2].w) * 0.5f;
		float4 depthColor = g_ShadowMap3.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else
	{
		zValue = input.ZDepth[3].z / input.ZDepth[3].w;
		cord.x = (1.0f + input.ZDepth[3].x / input.ZDepth[3].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[3].y / input.ZDepth[3].w) * 0.5f;
		float4 depthColor = g_ShadowMap4.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	if (g_IsShadow.x >= 1.0f) color.rgb *= shadowColor;
	
	//-----トゥーン処理------.
	// ハーフランバート拡散照明によるライティング計算
	float p = dot(input.Normal, input.LightDir);
	p = p * 0.5f + 0.5f;
	p = p * p;
	// 計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
	float4 toonColor = g_ToonMap.Sample(g_ToonSamLinear, float2(p, 0.0f));
	color.rgb *= toonColor.rgb * g_fIntensity.x + g_vSpecular.rgb;
	
	//-----高さフォグ処理------.
	// fogテクスチャの座標を取得、計算.
	const float fogColor =
		(g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.xy).r +
		 g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.zw).r) * 0.5f;
	float alpha = clamp((input.PosW.y - g_Fog.MinHeight) / (g_Fog.MaxHeight - g_Fog.MinHeight), 0.0f, 1.0f);
	float alphas = 1.0f - (1.0f - alpha) * fogColor;

	color.rgb = color.rgb * alphas + g_Fog.FogColor.rgb * (1.0f - alphas);
	
	PS_OUTPUT output = (PS_OUTPUT) 0;
	output.Color = color;
	output.Normal = float4(input.Normal, 1.0f);
	output.ZDepth = input.Pos.z / input.Pos.w;
	if (output.Color.a <= 0.0f) discard;
	
	return output;
}