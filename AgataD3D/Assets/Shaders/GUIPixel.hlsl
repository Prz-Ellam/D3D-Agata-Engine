struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float alpha : ALPHA;
};

Texture2D t_Texture : register(t0);
SamplerState t_Sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
	if (input.alpha == 0.0f) {
		clip(-1);
     }

	float4 fragColor = t_Texture.Sample(t_Sampler, input.uv);
	
	clip(fragColor.a < 0.5f ? -1 : 1); // Discard in GLSL

	fragColor.a = input.alpha;

	return fragColor;
}