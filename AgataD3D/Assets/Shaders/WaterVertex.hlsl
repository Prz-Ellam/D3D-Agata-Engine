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
	float4 colour : COLOUR;
	float3 toCameraVector : VECTOR3;
	float4 clipSpace : SPACE;
};

cbuffer WaterBuffer : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float4 c_Colour;
	float3 c_CameraPos;
}

PSInput main(VSInput vsInput) {

	PSInput psInput;

	float4 worldPos = mul(float4(vsInput.pos, 1.0f), c_Model);

	psInput.pos = worldPos;
	psInput.pos = mul(psInput.pos, c_View);
	psInput.pos = mul(psInput.pos, c_Projection);

	psInput.uv = vsInput.uv;
	psInput.colour = c_Colour;
	
	psInput.toCameraVector = c_CameraPos - worldPos.xyz;

	psInput.clipSpace = psInput.pos;

	return psInput;
}