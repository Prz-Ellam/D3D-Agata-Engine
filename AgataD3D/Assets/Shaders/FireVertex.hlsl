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
	float displacement : DISPLACEMENT;
};

cbuffer VS_CB : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float c_Displacement;
}

PSInput main(VSInput input)
{
	PSInput output;

    output.pos = mul(float4(input.pos, 1.0f), c_Model);
	output.pos = mul(output.pos, c_View);
	output.pos = mul(output.pos, c_Projection);
	output.uv = input.uv;
	output.displacement = c_Displacement;
	
	return output;
}