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
	float3 normal : NORMAL;
	float3 toLightVector : LIGHTVECTOR;
	float3 toCameraVector : CAMERAVECTOR;
	float4 lightColour : LIGHTCOLOUR;
};

cbuffer VS_CB : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float4 c_CameraPosition;
	float4 c_LightPosition;
	float4 c_LightColour;
}

PSInput main(VSInput input) {

	PSInput output;

	float4 worldPos = mul(float4(input.pos, 1.0f), c_Model);
	output.pos = worldPos;
	output.pos = mul(output.pos, c_View);
	output.pos = mul(output.pos, c_Projection);
	output.uv = input.uv;

	output.normal = input.normal;
	output.lightColour = c_LightColour;

	output.toLightVector = c_LightPosition - worldPos.xyz;
	output.toCameraVector = c_CameraPosition - worldPos.xyz;

	return output;
}