struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORDS;
	float3 normal : NORMAL;
	float3 toLightVector : LIGHTVECTOR;
	float3 toCameraVector : CAMERAVECTOR;
	float4 lightColour : LIGHTCOLOUR;
	float4 ambientMaterial : AMBIENT;
	float4 diffuseMaterial : DIFFUSE;
};

Texture2D tex : register(t0);
Texture2D t_NormalMap : register(t1);
SamplerState s_Sampler : register(s0);

float4 main(PSInput input) : SV_TARGET {

	// Normal Map
	float3 unitNormal = 2.0f * t_NormalMap.Sample(s_Sampler, input.uv).xyz - 1.0f;
	unitNormal = normalize(unitNormal);


	float4 texColour = tex.Sample(s_Sampler, input.uv);


	// Ambient Light
	float3 ambient = input.lightColour.xyz * 0.85;


	//float3 unitNormal = normalize(input.normal);

	// Diffuse Light
	float diffuseScalar = dot(unitNormal, normalize(input.toLightVector));
	diffuseScalar = max(diffuseScalar, 0.0f);
	float3 diffuse = diffuseScalar * 0.85 * input.lightColour;
	float4 diffuseColour = tex.Sample(s_Sampler, input.uv);


	// Specular Light
	float3 lightDirection = -normalize(input.toLightVector);
	float3 reflectLightDir = reflect(lightDirection, unitNormal);
	float specularScalar = dot(reflectLightDir, normalize(input.toCameraVector));
	specularScalar = max(specularScalar, 0.0f);
	specularScalar = pow(specularScalar, 32);
	//float4 specularMap = texture(u_SpecularMap, fs_TexCoords);
	float4 specular = specularScalar * 1 * input.lightColour;// *vec3(specularMap);


	clip(texColour.a < 0.5f ? -1 : 1); // Discard in GLSL

	return diffuseColour * float4(ambient + diffuse + specular, 1.0f);


}