struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float4 colour : COLOUR;
	float3 toCameraVector : VECTOR3;
	float4 clipSpace : SPACE;
};

SamplerState waterSampler : register(s0);
Texture2D t_Reflection : register(t0);
Texture2D t_Refraction : register(t1);

float4 main(PSInput psInput) : SV_TARGET 
{

	// Normalize device coordinates
	float ndcX = psInput.clipSpace.x / psInput.clipSpace.w / 2.0f + 0.5f;
    float ndcY = -psInput.clipSpace.y / psInput.clipSpace.w / 2.0f + 0.5f;
    float2 refractionTexCoords = float2(ndcX, ndcY);
	float2 reflectionTexCoords = float2(ndcX, 1 - ndcY);
	//psInput.uv = reflectionTexCoords;

	float4 color = lerp(t_Reflection.Sample(waterSampler, reflectionTexCoords),
		t_Refraction.Sample(waterSampler, refractionTexCoords), 0.5f);
	color = lerp(color, psInput.colour, 0.5f);

	return color;
}