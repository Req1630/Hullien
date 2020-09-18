cbuffer per_mesh : register(b0)
{
	matrix g_LightWVP; // ���C�g�̃��[���h�A�r���[�A�v���W�F�N�V����.
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};

//-----------------------------------------.
// �X�^�e�B�b�N���b�V��.
//-----------------------------------------.
// ���_�V�F�[�_�[�̓��̓p�����[�^.
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

// �X�^�e�B�b�N���b�V���̃��C���֐�.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	// ���C�g�s��ƍ��W���v�Z.
	output.Pos = mul(input.Pos, g_LightWVP);
	
	return output;
}
//-----------------------------------------.

//-----------------------------------------.
// �X�L�����b�V��.
//-----------------------------------------.
//��`.
#define MAX_BONE_MATRICES (255)	// �{�[���ő吔.
// �{�[���̃|�[�Y�s�񂪓���.
cbuffer per_bones : register(b1)
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};
//�X�L�j���O��̒��_�E�@��������.
struct Skin
{
	float4 Pos;
	float3 Norm;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���.
struct VSSkinIn
{
	float4 Pos : POSITION; //�ʒu.  
	float3 Norm : NORMAL; //���_�@��.
	float2 Tex : TEXCOORD; //�e�N�X�`���[���W.
	uint4 Bones : BONE_INDEX; //�{�[���̃C���f�b�N�X.
	float4 Weights : BONE_WEIGHT; //�{�[���̏d��.
};
//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ�.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
matrix FetchBoneMatrix(uint iBone)
{
	return g_mConstBoneWorld[iBone];
}
//���_���X�L�j���O�i�{�[���ɂ��ړ��j����.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
Skin SkinVert(VSSkinIn Input)
{
	Skin Output = (Skin) 0;

	float4 Pos = float4(Input.Pos);
	float3 Norm = Input.Norm;
	//�{�[��0.
	uint iBone = Input.Bones.x;
	float fWeight = Input.Weights.x;
	matrix m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//�{�[��1.
	iBone = Input.Bones.y;
	fWeight = Input.Weights.y;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//�{�[��2.
	iBone = Input.Bones.z;
	fWeight = Input.Weights.z;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//�{�[��3.
	iBone = Input.Bones.w;
	fWeight = Input.Weights.w;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);

	return Output;
}
// �o�[�e�b�N�X�V�F�[�_.
VS_OUTPUT VS_SkinMain(VSSkinIn input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert(input);

	// ���C�g�s��ƍ��W���v�Z.
	output.Pos = mul(vSkinned.Pos, g_LightWVP);
	
	return output;
}
//-----------------------------------------.

// �[�x�l���v�Z.
float4 outRBGA(float depth)
{
	// R�������o
	float R = round(depth * 256.0f) / 256.0f;
	// G�������o
	float Def = depth - R;
	float G = round(Def * 65536.0f) / 256.0f;
	// B�������o
	Def -= G / 256.0f;
	float B = round(Def * 65536.0f * 256.0f) / 256.0f;
	// A�������o
	Def -= B / 65536.0f;
	float A = round(Def * 65536.0f * 65536.0f) / 256.0f;
	
	return float4(R, G, B, A);
}

// �s�N�Z���V�F�[�_�[.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	return input.Pos.z / input.Pos.w;
	return outRBGA(input.Pos.z / input.Pos.w);
}