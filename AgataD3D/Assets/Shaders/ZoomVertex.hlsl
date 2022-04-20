struct VSInput {
	float3 pos : POSITION;
	float2 uv : TEXCOORDS;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
};

PSInput main(VSInput vsInput) {

	PSInput psInput;

    psInput.pos = float4(vsInput.pos, 1.0f);
	psInput.uv = vsInput.uv;

	return psInput;

}