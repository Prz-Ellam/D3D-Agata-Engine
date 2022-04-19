struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
};

Texture2D tex : register(t0);
SamplerState samplerX : register(s0);

float4 main(PSInput input) : SV_TARGET {

	float4 texColour = tex.Sample(samplerX, input.uv);

	clip(texColour.a < 0.5f ? -1 : 1);

	return texColour;


}