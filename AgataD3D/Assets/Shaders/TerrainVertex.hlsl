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
	float tiling : TILING;
	float3 normal : NORMAL;
	float3 toLightVector : LIGHTVECTOR;
	float3 toCameraVector : CAMERAVECTOR;
	float4 lightColour : LIGHTCOLOUR;
};

cbuffer TerrainBuffer : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float4 c_CameraPosition;
	float4 c_LightPosition;
	float4 c_LightColour;
	float c_Tiling;
}

PSInput main(VSInput vsInput) {

	PSInput psInput;

	float4 worldPos = mul(float4(vsInput.pos, 1.0f), c_Model);
	psInput.pos = worldPos;
	psInput.pos = mul(psInput.pos, c_View);
	psInput.pos = mul(psInput.pos, c_Projection);

	psInput.uv = vsInput.uv;
	psInput.tiling = c_Tiling;

	psInput.normal = vsInput.normal;
	psInput.lightColour = c_LightColour;

	float3 T = normalize(mul(c_Model, float4(vsInput.tangent, 0.0f)).xyz);
	float3 N = normalize(mul(c_Model, float4(vsInput.normal, 0.0f)).xyz);
	T = normalize(T - dot(T, N) * N);
	float3 B = normalize(mul(c_Model, float4(vsInput.bitangent, 0.0f)).xyz);
	float3x3 TBN = transpose(float3x3(T, B, N));

	psInput.toLightVector = mul(normalize(c_LightPosition - worldPos.xyz), TBN);
	psInput.toCameraVector = mul(normalize(c_CameraPosition - worldPos.xyz), TBN);

	return psInput;
}