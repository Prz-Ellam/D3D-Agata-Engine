struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
};

Texture2D t_Texture : register(t0);
SamplerState t_Sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
	float u = 1 / 32.0f;
float v = 1 / 3.0f;

//int row = 32 / 32;
//int column = ((ascii / 32.0f) - row + 1) * 32;

//u = u * input.uv.x + (1.0f * (1.0f / 32.0f));
//v = v * input.uv.y;


	float4 fragColor = t_Texture.Sample(t_Sampler, input.uv);

	//if (all(fragColor == float4(1.0f, 1.0f, 1.0f, 1.0f))) {
	//	clip(0.0f);
	//}
	if (fragColor.a < 0.5f) {
		clip(-1);
	}

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}