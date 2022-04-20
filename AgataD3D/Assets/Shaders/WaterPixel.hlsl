struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float4 colour : COLOUR;
	float4 toCameraVector : VECTOR3;
	float4 lightDirection : LIGHTVECTOR;
	float4 clipSpace : SPACE;
	float displacement : DISPLACEMENT;
};

SamplerState waterSampler : register(s0);
Texture2D t_Reflection : register(t0);
Texture2D t_Refraction : register(t1);
Texture2D t_DudvMap : register(t2);
Texture2D t_NormalMap : register(t3);

float4 main(PSInput psInput) : SV_TARGET 
{

	// Normalize device coordinates
	float ndcX = psInput.clipSpace.x / psInput.clipSpace.w / 2.0f + 0.5f;
	float ndcY = -psInput.clipSpace.y / psInput.clipSpace.w / 2.0f + 0.5f;
	float2 refractionTexCoords = float2(ndcX, ndcY);
	float2 reflectionTexCoords = float2(ndcX, 1 - ndcY);

	float4 dudvMap = t_DudvMap.Sample(waterSampler, float2(psInput.uv.x + psInput.displacement, psInput.uv.y));
	float2 distortedTexCoords = dudvMap.xy * 0.2f; // tiling factor
	distortedTexCoords = psInput.uv + float2(distortedTexCoords.x, distortedTexCoords.y + psInput.displacement);
	dudvMap = t_DudvMap.Sample(waterSampler, distortedTexCoords);
	float2 distortion = (dudvMap.xy * 2.0f - 1.0f) * 0.02f; // Distortion strength

	refractionTexCoords += distortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001f, 0.999f);
	reflectionTexCoords += distortion;
	reflectionTexCoords = clamp(reflectionTexCoords, 0.001f, 0.999f);

	float4 refraction = float4(t_Refraction.Sample(waterSampler, refractionTexCoords).xyz, 1.0f);
	float4 reflection = float4(t_Reflection.Sample(waterSampler, reflectionTexCoords).xyz, 1.0f);


	float4 normalMap = t_NormalMap.Sample(waterSampler, distortedTexCoords);
	float3 normal = normalize(float3(normalMap.x * 2.0f - 1.0f, normalMap.y, normalMap.z * 2.0f - 1.0f));

	float reflectivity = 1.0f;
	float3 reflectLight = reflect(normalize(psInput.lightDirection.xyz), normal);
	float specularScalar = dot(reflectLight, normalize(psInput.toCameraVector.xyz));
	specularScalar = max(specularScalar, 0.0f);
	specularScalar = pow(specularScalar, 32);

	float3 specularLight = /* Light Colour * */float3(1,1,1) * specularScalar * reflectivity;

	// Fresnel effect
	float reflectivityFactor = dot(normalize(psInput.toCameraVector), float3(0.0f, 1.0f, 0.0f));
	reflectivityFactor = pow(reflectivityFactor, 0.3f);


	float4 color = lerp(reflection, refraction, reflectivityFactor);
	color = lerp(color, psInput.colour, 0.3f) + float4(specularLight, 0.0f);

	return color;
}