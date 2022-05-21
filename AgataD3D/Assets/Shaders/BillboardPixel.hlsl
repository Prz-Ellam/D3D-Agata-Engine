struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float3 normal : NORMAL;
	float3 toLightVector : LIGHTVECTOR;
	float3 toCameraVector : CAMERAVECTOR;
	float4 lightColour : LIGHTCOLOUR;
};

Texture2D t_DiffuseTex : register(t0);
Texture2D t_NormalTex : register(t1);
SamplerState s_Sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{

	float3 unitNormal = 2.0f * t_NormalTex.Sample(s_Sampler, input.uv).xyz - 1.0f;
	unitNormal = normalize(unitNormal);
	
	float4 ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
	
	float diffuseScalar = dot(normalize(input.toLightVector), unitNormal);
	diffuseScalar = max(diffuseScalar, 0.0f);
	float4 diffuse = diffuseScalar * input.lightColour;

	float4 diffuseColour = t_DiffuseTex.Sample(s_Sampler, input.uv);

	clip(diffuseColour.a < 0.5f ? -1 : 1); // Discard in GLSL

	return diffuseColour * (ambient + diffuse);
}