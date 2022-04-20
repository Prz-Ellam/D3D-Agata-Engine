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
	float4 toCameraVector : VECTOR3;
	float4 lightDirection : LIGHTVECTOR;
	float4 clipSpace : SPACE;
	float displacement : DISPLACEMENT;
};

cbuffer WaterBuffer : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float4 c_Colour;
	float4 c_CameraPos;
	float4 c_LightPosition;
	float4 c_LightColour;
	float c_Displacement;
}

PSInput main(VSInput vsInput) {

	PSInput psInput;

	float4 worldPos = mul(float4(vsInput.pos, 1.0f), c_Model);

	psInput.pos = worldPos;
	psInput.pos = mul(psInput.pos, c_View);
	psInput.pos = mul(psInput.pos, c_Projection);

	psInput.uv = vsInput.uv * 3.0f;
	psInput.colour = c_Colour;
	psInput.displacement = c_Displacement;
	
	psInput.toCameraVector = float4(c_CameraPos - worldPos.xyz, 1.0f);
	psInput.lightDirection = float4(worldPos.xyz - c_LightPosition.xyz, 1.0f);

	psInput.clipSpace = psInput.pos;

	return psInput;
}