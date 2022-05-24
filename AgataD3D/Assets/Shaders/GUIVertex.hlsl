struct VSInput {
	float3 pos : POSITION;
	float2 uv : TEXCOORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float alpha : ALPHA;
};

cbuffer VS_CB : register(b0) {
	matrix c_Model;
	float c_Alpha;
}

PSInput main(VSInput input)
{
	PSInput output;

	output.pos = mul(float4(input.pos, 1.0f), c_Model);
	output.uv = input.uv;
	output.alpha = c_Alpha;

	return output;
}