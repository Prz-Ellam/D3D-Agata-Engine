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
};

cbuffer TerrainBuffer : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float3 c_CameraPos;
	float c_Tiling;
}

PSInput main(VSInput vsInput) {

	PSInput psInput;

	psInput.pos = mul(float4(vsInput.pos, 1.0f), c_Model);
	psInput.pos = mul(psInput.pos, c_View);
	psInput.pos = mul(psInput.pos, c_Projection);

	psInput.uv = vsInput.uv;
	psInput.tiling = c_Tiling;

	return psInput;
}