struct VertexInput {
	float3 pos : POSITION;
};

struct PixelInput {
	float4 pos : SV_POSITION;
	float3 uv : TEXCOORDS;
	float4 blendFactor : BLEND;
};

cbuffer SkyboxBuffer : register(b0) {
	matrix c_Model;
	matrix c_View;
	matrix c_Projection;
	float4 c_BlendFactor;
}

PixelInput main(VertexInput vsInput) {

	PixelInput psInput;

	psInput.pos = mul(float4(vsInput.pos, 1.0f), c_View);
	psInput.pos = mul(psInput.pos, c_Projection);

	psInput.uv = float3(vsInput.pos.x, -vsInput.pos.y, vsInput.pos.z);

	psInput.pos = float4(psInput.pos.x, psInput.pos.y, psInput.pos.w, psInput.pos.w);

	psInput.blendFactor = c_BlendFactor;

	return psInput;
}