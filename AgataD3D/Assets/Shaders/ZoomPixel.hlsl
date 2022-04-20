struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
};

Texture2D t_Shape : register(t0);
SamplerState s_Sampler : register(s0);

float4 main(PSInput psInput) : SV_TARGET {

	float4 shape = t_Shape.Sample(s_Sampler, psInput.uv);
	shape.w = 1.2f - ((shape.x + shape.y + shape.z) / 3.0f);

	float4 color = shape;

	return color;

}