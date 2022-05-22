struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float displacement : DISPLACEMENT;
};

Texture2D noise : register(t0);
Texture2D alpha : register(t1);
Texture2D ramp : register(t2);
Texture2D dudv : register(t3);
SamplerState samplerX : register(s0);

float4 main(PSInput input) : SV_TARGET
{
	float2 dudvMap = dudv.Sample(samplerX, input.uv).xy * 0.05f;

	float2 noiseUV = float2(input.uv.x + dudvMap.x, input.uv.y + dudvMap.y - input.displacement);

	float4 noiseTex = noise.Sample(samplerX, noiseUV) * 2.0f - 1.0f;
	float4 alphaTex = alpha.Sample(samplerX, float2(input.uv.x + dudvMap.x, input.uv.y));


	float4 rampTex = ramp.Sample(samplerX, float2(input.uv.x + dudvMap.x, input.uv.y + dudvMap.y));


	float4 outColour = lerp(noiseTex, rampTex, 0.8f);
	outColour.x += 0.1f;
	outColour.a = (alphaTex.x + alphaTex.y + alphaTex.z) / 3;

	return outColour;
}