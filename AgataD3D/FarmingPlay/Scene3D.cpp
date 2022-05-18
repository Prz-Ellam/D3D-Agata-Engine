#include "pch.h"
#include "Scene3D.h"

#include "Joystick.h"


float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

Scene3D::Scene3D(const Agata::WindowParams& windowParams) : Agata::Scene(windowParams) {

	OnInit();

}

Scene3D::~Scene3D() {

	OnRelease();

}

void Scene3D::OnInit() {

	m_Window->Show();
	m_Window->SetIcon("Agata.ico");
	m_Running = true;

	 Agata::PerspectiveCameraProps properties = {
		45.0f,
		static_cast<float>(m_Window->GetWidth()) / m_Window->GetHeight(),
		0.1f,
		1000.0f
	};
	m_Camera = std::make_unique<Agata::Camera>(properties, 60.0f, 800.0f);


	// Load Models, Textures, Shaders, Resources
	m_TerrainShader = std::make_shared<Agata::Shader>("TerrainVertex.cso", "TerrainPixel.cso");
	m_TerrainShader->Bind();

	m_Terrain = Agata::TerrainBuilder::GenerateParams()
		.HeightMap("Assets/Images/Terrain/Heightmap.jpg")
		.Position(DirectX::XMFLOAT3(-125.0f, 0.0f, -125.0f))
		.Rotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
		.Scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f))
		.Height(20.0f)
		.Width(250.0f)
		.Depth(250.0f)
		.RedTexture("Assets//Images//Terrain//Street.jpg")
		.GreenTexture("Assets//Images//Terrain//Sand.png")
		.BlueTexture("Assets//Images//Terrain//Dirt.jpg")
		.BlackTexture("Assets//Images//Terrain//Grass.jpg")
		.RedNormal("Assets//Images//Terrain//StreetNorm.png")
		.GreenNormal("Assets//Images//Terrain//SandNorm.png")
		.BlueNormal("Assets//Images//Terrain//DirtNorm.jpg")
		.BlackNormal("Assets//Images//Terrain//GrassNorm.png")
		.BlendMap("Assets//Images//Terrain//Splat_Map.png")
		.TilingFactor(40.0f)
		.Build();


	m_GUIShader = std::make_shared<Agata::Shader>("GUIVertex.cso", "GUIPixel.cso");
	m_GUIShader->Bind();

	m_GUI = std::make_shared<Agata::GUI>("Assets//Images//UI//Observar.png", 
		DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f));
	m_VehicleGUI = std::make_shared<Agata::GUI>("Assets//Images//UI//Subir.png",
		DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f));
	m_Icon = std::make_shared<Agata::GUI>("Assets//Images//UI//lose.png",
		DirectX::XMFLOAT2(-0.85f, -0.8f), DirectX::XMFLOAT2(0.1f, 0.1f));

	m_TextShader = std::make_shared<Agata::Shader>("TextVertex.cso", "TextPixel.cso");
	m_TextShader->Bind();

	m_Text = std::make_shared<Agata::Text>("Assets//Images//Fonts//Fixedsys16x28.png");


	m_SkeletalModelShader = std::make_shared<Agata::Shader>("SkeletalModelVertex.cso", "SkeletalModelPixel.cso");
	m_SkeletalModelShader->Bind();

	float x, z;
	x = 0.0f;
	z = 0.0f;
	m_SkeletalModel = Agata::SkeletalModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Personaje//Forward.fbx").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(0.01f, 0.01f, 0.01f)).
		DiffuseTexture("Assets//Models//Personaje//CharacterDiffuse.jpg").
		//NormalTexture("Assets//Models//Casa 1//norm.png").
		Build();



	m_StaticModelShader = std::make_shared<Agata::Shader>("StaticModelVertex.cso", "StaticModelPixel.cso");
	m_StaticModelShader->Bind();

	x = 10.0f;
	z = 0.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Casa 1//rural1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Casa 1//diff.png").
		NormalTexture("Assets//Models//Casa 1//norm.png").
		DefaultCollider(true).
		Build());

	x = 10.0f;
	z = 20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Casa 2//rural5.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Casa 2//diffuse.png").
		NormalTexture("Assets//Models//Casa 2//normal.png").
		DefaultCollider(true).
		Build());

	x = -10.0f;
	z = -20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Telescopio//telescopio.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Telescopio//albedo.png").
		NormalTexture("Assets//Models//Telescopio//normal.png").
		Build());

	DirectX::XMFLOAT3 min = DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z);
	m_SpyGlassCollider = Agata::BoxCollider(DirectX::XMFLOAT3(min.x - 4, min.y, min.z - 4),
		                                DirectX::XMFLOAT3(min.x + 4, min.y, min.z + 4));

	x = 10.0f;
	z = -20.0f;
	m_Vehicle = Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Tractor//tractor.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(0.8f, 0.8f, 0.8f)).
		DiffuseTexture("Assets//Models//Tractor//foto2.jpg").
		Build();

	min = DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z);
	m_VehicleCollider = Agata::BoxCollider(DirectX::XMFLOAT3(min.x - 4, min.y, min.z - 4),
		                                   DirectX::XMFLOAT3(min.x + 4, min.y, min.z + 4));
	
	x = 20.0f;
	z = -40.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Casa 3//casa1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Casa 3//House4_lambert1_AlbedoTransparency.png").
		NormalTexture("Assets//Models//Casa 3//House4_lambert1_Normal.png").
		AddCollider("Assets//Colliders//Casa3_1.aabb").
		AddCollider("Assets//Colliders//Casa3_2.aabb").
		AddCollider("Assets//Colliders//Casa3_3.aabb").
		Build());

	x = -17.0f;
	z = -45.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//church//church.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//church//chiesa_Base_Color.jpg").
		AddCollider("Assets//Colliders//Church_1.aabb").
		AddCollider("Assets//Colliders//Church_2.aabb").
		AddCollider("Assets//Colliders//Church_3.aabb").
		Build());

	x = 5.0f;
	z = -12.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Table//Table.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Table//DefaultMaterial_Base_Color.png").
		DefaultCollider(true).
		Build());

	x = -15.0f;
	z = -16.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//kiosk//kiosco.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//kiosk//3.png").
		DefaultCollider(true).
		Build());

	x = -45.0f;
	z = -23.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//farm//farm.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//farm//FarmUV.png").
		Build());
	
	x = 3.0f;
	z = -47.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//plants.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Plants_Texture.png").
		Build());

	x = -40.0f;//1
	z = -33.0f;//-45
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta1//planta1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta1//Plants_Texture.png").
		DefaultCollider(true).
		Build());

	x = -34.0f;//2
	z = 25.0f;//-45
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta1//planta1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta1//Plants_Texture.png").
		DefaultCollider(true).
		Build());

	x = 14.0f;//3
	z = -10.0f;//-45
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta2//planta2.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta2//Plants_Texture.png").
		DefaultCollider(true).
		Build());

	x = -14.0f; //4
	z = -25.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta3//planta3.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta3//Plants_Texture.png").
		DefaultCollider(true).
		Build());

	x = -12.0f; //5
	z = 25.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta3//planta3.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta3//Plants_Texture.png").
		DefaultCollider(true).
		Build());

	x = -20.0f; //6
	z = 4.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta4//planta4.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta4//Plants_Texture.png").
		DefaultCollider(true).
		Build());


	x = 8.0f; //7
	z = -36.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta5//planta5.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta5//Plants_Texture.png").
		DefaultCollider(true).
		Build());
	
	
	x = 74.0f; //1
	z = 25.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//llave2//llave2.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//llave2//wrenchExport_lambert2_R.png").
		DefaultCollider(true).
		Build());


	x = 74.0f; //2
	z = 10.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//llave2//llave2.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//llave2//wrenchExport_lambert2_R.png").
		DefaultCollider(true).
		Build());

	x = 6.0f;//1
	z = -45.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		DefaultCollider(true).
		Build());

	x = -14.0f;//2
	z = 25.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		DefaultCollider(true).
		Build());

	x = -22.0f;//3
	z = 5.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		DefaultCollider(true).
		Build());

	x = 5.0f; //4
	z = 10.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		DefaultCollider(true).
		Build());

	x = 2.0f;//5
	z = -10.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		DefaultCollider(true).
		Build());

	x = -50.0f;//6
	z = -35.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		DefaultCollider(true).
		Build());

	x = 32.0f; //7
	z = 20.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		DefaultCollider(true).
		Build());
	
	x = 18.0f;
	z = 8.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//bench//bench.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//bench//wooden_bench_d.tga.png").
		DefaultCollider(true).
		Build());

	x = -18.0f;
	z = -3.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//bench//bench.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//bench//wooden_bench_d.tga.png").
		DefaultCollider(true).
		Build());

	x = -15.0f;
	z = 10.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Stand//stand.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Stand//FoodStand_Base_Color.png").
		DefaultCollider(true).
		Build());

	x = 22.0f;
	z = 20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//toilet//toilet.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//toilet//toilet.png").
		DefaultCollider(true).
		Build());
	
	x = 20.0f;
	z = -11.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//campfire//campfire.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//campfire//barkTexture_COLOR.png").
		NormalTexture("Assets//Models//campfire//barkTexture_NRM.png").
		Build());

	x = 10-2.7f;
	z = 0.8f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Cuadro//Cuadro.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z) + 0.8f, z)).
		Rotation(DX::XMFLOAT3(0.0f, 90.0f, 0.0f)).
		Scale(DX::XMFLOAT3(-1.2f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Cuadro//texture.jpg").
		DefaultCollider(true).
		Build());

	
	x = -10.0f;
	z = 20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Arbol//Tree01.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(5.0f, 5.0f, 5.0f)).
		DiffuseTexture("Assets//Models//Arbol//Wood_002.png").
		NormalTexture("Assets//Models//Arbol//Wood_002_normal.tga.png").
		Build());

	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Arbol//Tree02.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(5.0f, 5.0f, 5.0f)).
		DiffuseTexture("Assets//Models//Arbol//Bitmap2Material_3_Base_Color.tga.png").
		NormalTexture("Assets//Models//Arbol//Bitmap2Material_3_Normal.tga.png").
		Build());
	

	m_SkyboxShader = std::make_shared<Agata::Shader>("SkyboxVertex.cso", "SkyboxPixel.cso");
	m_SkyboxShader->Bind();

	{
		Agata::SkyboxTexture morning = {
			"Assets//Images//Skybox//rightr.png",
			"Assets//Images//Skybox//leftr.png",
			"Assets//Images//Skybox//topr.png",
			"Assets//Images//Skybox//bottomr.png",
			"Assets//Images//Skybox//frontr.png",
			"Assets//Images//Skybox//backr.png"
		};
		Agata::SkyboxTexture day = {
			"Assets//Images//Skybox//right.png",
			"Assets//Images//Skybox//left.png",
			"Assets//Images//Skybox//top.png",
			"Assets//Images//Skybox//bottom.png",
			"Assets//Images//Skybox//front.png",
			"Assets//Images//Skybox//back.png"
		};
		Agata::SkyboxTexture sunset = {
			"Assets//Images//Skybox//rightaf.png",
			"Assets//Images//Skybox//leftaf.png",
			"Assets//Images//Skybox//topaf.png",
			"Assets//Images//Skybox//bottomaf.png",
			"Assets//Images//Skybox//frontaf.png",
			"Assets//Images//Skybox//backaf.png"
		};
		Agata::SkyboxTexture night = {
			"Assets//Images//Skybox//rightni.png",
			"Assets//Images//Skybox//leftni.png",
			"Assets//Images//Skybox//topni.png",
			"Assets//Images//Skybox//bottomni.png",
			"Assets//Images//Skybox//frontni.png",
			"Assets//Images//Skybox//backni.png"
		};

		m_Skybox = std::make_shared<Agata::Skybox>(morning, day, sunset, night, 500.0f, 1.0f);
	} 
	


	m_FBO = new Agata::Framebuffer(m_Window->GetWidth(), m_Window->GetHeight());

	m_QuadShader =  std::make_shared<Agata::Shader>("QuadVertex.cso", "QuadPixel.cso");
	m_QuadShader->Bind();

	m_Quad = std::make_shared<Agata::Quad>();

	m_WaterShader = std::make_shared<Agata::Shader>("WaterVertex.cso", "WaterPixel.cso");
	m_WaterShader->Bind();
	m_Water = std::make_shared<Agata::Water>(DX::XMFLOAT3(30, 1.5, 0), DX::XMFLOAT3(0, 0, 0),
		DX::XMFLOAT3(2, 2, 2), "Assets//Images//Water//dudv.png", 
		"Assets//Images//Water//normalMap.png", 
		m_Window->GetWidth(), m_Window->GetHeight());

	
	m_BillboardShader = std::make_shared<Agata::Shader>("BillboardVertex.cso", "BillboardPixel.cso");
	m_BillboardShader->Bind();

	m_Tree1 = std::make_shared<Agata::Billboard>("Assets//Images//Billboards//Autumn_Pine.png", 
		DX::XMFLOAT3(13, m_Terrain->GetHeight(13, -11) + 1, -11), DX::XMFLOAT3(1, 1, 1));
	m_Tree2 = std::make_shared<Agata::Billboard>("Assets//Images//Billboards//tree2.png", 
		DX::XMFLOAT3(10, m_Terrain->GetHeight(10, -11) + 1, -11), DX::XMFLOAT3(1, 1, 1));

	m_FireShader = std::make_shared<Agata::Shader>("FireVertex.cso", "FirePixel.cso");
	m_FireShader->Bind();
	m_Fire = std::make_shared<Agata::Fire>("Assets//Images//Fire//rampFire.gif", 
		"Assets//Images//Fire//alphaFire.gif", 
		"Assets//Images//Fire//dudv.png", 
		"Assets//Images//Fire//fireNoise.png",
		DX::XMFLOAT3(20, m_Terrain->GetHeight(20, -11) + 1, -11), DX::XMFLOAT3(1, 1, 1));


	m_ZoomShader = std::make_shared<Agata::Shader>("ZoomVertex.cso", "ZoomPixel.cso");
	m_ZoomShader->Bind();
	m_SpyGlass = std::make_shared<Agata::Zoom>("Assets//Images//Zoom//zoom.jpg");


	m_Light = std::make_shared<Agata::DirectionLight>(DX::XMFLOAT3(0.0f, 0.0f, -400.0f), 
		DX::XMFLOAT3(1.0f, 0.5f, 0.5f));

	m_Cycle = 180.0f;


}

void Scene3D::OnRun() {

	// Game Loop 

	m_Timer.Start();
	m_Dt = 1 / 60;
	while (m_Running) {

		Update();
		Render();

		m_Window->Update();

		m_Timer.Stop();
		m_Dt = m_Timer.GetMiliseconds();
		m_Ts += m_Dt;

		(m_Cycle > -180) ? m_Cycle -= 2.0f * m_Dt : m_Cycle = 180.0f;

		m_Timer.Restart();

	}

}

#define MORNING_TO_DAY_MIN 160.0f
#define MORNING_TO_DAY_MAX 135.0f

#define DAY_TO_SUNSET_MIN 45.0f
#define DAY_TO_SUNSET_MAX 20.0f

#define SUNSET_TO_NIGHT_MIN 0.0f
#define SUNSET_TO_NIGHT_MAX -25.0f

#define NIGHT_TO_MORNING_MIN -135.0f
#define NIGHT_TO_MORNING_MAX -160.0f

void Scene3D::Update() {

	m_Window->SetTitle(
		"X: " + std::to_string(m_Camera->GetPosition().x) + ", " + 
		"Y: " + std::to_string(m_Camera->GetPosition().y) + ", " +
		"Z: " + std::to_string(m_Camera->GetPosition().z));

	float y = 400 * sin(DX::XMConvertToRadians(m_Cycle));
	float z = 400 * cos(DX::XMConvertToRadians(m_Cycle));

	m_Light->SetPositionY(y);
	m_Light->SetPositionZ(z);
	float I = std::max(0.4f * std::sin(DirectX::XMConvertToRadians(m_Cycle)) + 0.2f, 0.2f) + 0.1f;

	for (auto& model : m_Models) {
		model->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		model->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));
	}
	
	for (auto& Item : m_Items) {
		Item->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		Item->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));
	}
	
	m_SkeletalModel->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
	m_SkeletalModel->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));
	
	m_Camera->Move(m_Dt);

	if (m_SpyGlassCollider.IsColliding(m_Camera->GetPosition()) && !m_IsZoom) {
		m_SpyGlassArea = true;
	}
	else {
		m_SpyGlassArea = false;
	}

	if (m_VehicleCollider.IsColliding(m_Camera->GetPosition())) {
		m_VehicleArea = true;
	}
	else {
		m_VehicleArea = false;
	}

	for (auto& model : m_Models) {
		model->CheckCollision(m_Camera);
	}
	
	for (auto& Item : m_Items) {
		Item->CheckCollision(m_Camera);
	}

	for (auto& Item : m_Items) {
		if (Item->IsColliding(m_Camera) == true) { // al menos 1 de las condiciones sale falsa
			Item->SetPosition(DirectX::XMFLOAT3(300.0f,300.0f,300.0f));
			m_Icon->OnRender();
			cont++;
		}

		mod++;
	}

	//m_Camera->SetY(m_Terrain->GetHeight(m_Camera->GetX(), m_Camera->GetZ()) + 1.665);
	m_Camera->Update(m_Terrain);
	m_Skybox->OnUpdate(m_Dt);
	m_Water->OnUpdate(m_Dt);
	m_Fire->OnUpdate(m_Dt);

	m_Tree1->OnUpdate(m_Dt);
	m_Tree2->OnUpdate(m_Dt);

	m_SkeletalModel->OnUpdate(m_Ts);
	//m_SkeletalModel->SetPosition(DirectX::XMFLOAT3(m_Camera->GetPosition().x,
	//	m_Camera->GetPosition().y - 1.665f + 0.2f, m_Camera->GetPosition().z));
	//m_SkeletalModel->SetRotation(DirectX::XMFLOAT3(0.0f, -(m_Camera->GetYaw() - 90.0f), 0.0f));


	//m_Vehicle->FollowCamera(m_Camera);


	if (m_Cycle < MORNING_TO_DAY_MIN && m_Cycle > MORNING_TO_DAY_MAX) {

		// Conversion de [MORNING_TO_DAY_MIN, MORNING_TO_DAY_MAX] a [0, 1]
		float MorningLength = (MORNING_TO_DAY_MAX - MORNING_TO_DAY_MIN);
		float t = (m_Cycle - MORNING_TO_DAY_MIN) / MorningLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(1 - t, t, 0.0f, 0.0f));
		m_Light->SetColourG(lerp(0.5f, 1.0f, t));
		m_Light->SetColourB(lerp(0.5f, 1.0f, t));

	}

	if (m_Cycle < DAY_TO_SUNSET_MIN && m_Cycle > DAY_TO_SUNSET_MAX) {

		// Conversion de [DAY_TO_SUNSET_MIN, DAY_TO_SUNSET_MAX] a [0, 1]
		float DayLength = (DAY_TO_SUNSET_MAX - DAY_TO_SUNSET_MIN);
		float t = (m_Cycle - DAY_TO_SUNSET_MIN) / DayLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(0.0f, 1 - t, t, 0.0f));
		m_Light->SetColourG(lerp(1.0f, 0.5f, t));
		m_Light->SetColourB(lerp(1.0f, 0.5f, t));

	}

	if (m_Cycle < SUNSET_TO_NIGHT_MIN && m_Cycle > SUNSET_TO_NIGHT_MAX) {

		// Conversion de [SUNSET_TO_NIGHT_MIN, SUNSET_TO_NIGHT_MAX] a [0, 1]
		float SunsetLength = (SUNSET_TO_NIGHT_MAX - SUNSET_TO_NIGHT_MIN);
		float t = (m_Cycle - SUNSET_TO_NIGHT_MIN) / SunsetLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(0.0f, 0.0f, 1 - t, t));
		m_Light->SetColourR(lerp(1.0f, 0.05f, t));
		m_Light->SetColourG(lerp(0.5f, 0.15f, t));
		m_Light->SetColourB(lerp(0.5f, 0.6f, t));

	}

	if (m_Cycle < NIGHT_TO_MORNING_MIN && m_Cycle > NIGHT_TO_MORNING_MAX) {

		// Conversion de [NIGHT_TO_MORNING_MIN, NIGHT_TO_MORNING_MAX] a [0, 1]
		float NightLength = (NIGHT_TO_MORNING_MAX - NIGHT_TO_MORNING_MIN);
		float t = (m_Cycle - NIGHT_TO_MORNING_MIN) / NightLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(t, 0.0f, 0.0f, 1 - t));
		m_Light->SetColourR(lerp(0.05f, 1.0f, t));
		m_Light->SetColourG(lerp(0.15f, 0.5f, t));
		m_Light->SetColourB(lerp(0.6f, 0.5f, t));

	}

}

void Scene3D::Render() {

	//m_FBO->BindFrameBuffer();
	//m_FBO->Clear(0, 1, 0, 0);

	float distance = 2 * (m_Camera->GetY() - m_Water->GetPosition().y);
	m_Camera->ChangePitchDirection();
	//m_Camera->SetY(m_Terrain->GetHeight(m_Camera->GetX(), m_Camera->GetZ()));
	m_Camera->Update(m_Terrain);
	//m_Camera->MoveHeight(-distance);


	Agata::Renderer::Clear(1.0f, 1.0f, 1.0f, 1.0f);
	Agata::Renderer::BeginScene(m_Camera, m_Light);

	m_Water->BeginReflection();
	RenderScene();
	m_Water->EndReflection();

	// Agata::Renderer::EndScene();



	m_Camera->MoveHeight(distance);
	m_Camera->ChangePitchDirection();
	m_Camera->Update(m_Terrain);

	Agata::Renderer::Clear(1.0f, 1.0f, 1.0f, 1.0f);
	Agata::Renderer::BeginScene(m_Camera, m_Light);

	m_Water->BeginRefraction();
	RenderScene();
	m_Water->EndRefraction();

	// Agata::Renderer::EndScene();



	Agata::Renderer::BeginScene(m_Camera, m_Light);
	Agata::Renderer::Clear(0.2f, 0.2f, 0.2f, 1.0f);

	RenderScene();

	m_WaterShader->Bind();
	m_Water->OnRender();

	if (m_IsZoom) {
		m_ZoomShader->Bind();
		m_SpyGlass->OnRender();
	}

	m_GUIShader->Bind();
	if (m_SpyGlassArea) {
		m_GUI->OnRender();
	}

	if (m_VehicleArea) {
		m_VehicleGUI->OnRender();
	}

	m_Icon->OnRender();


	m_TextShader->Bind();
	m_Text->DrawString(std::to_string(m_Cycle), DirectX::XMFLOAT2(-0.9, 0.9), 1.0f);
	m_Text->DrawString("Objetos: ", DirectX::XMFLOAT2(-0.7, -0.8), 1.0f);
	//m_FBO->UnbindFramebuffer();

	//Agata::Renderer::Clear(1.0f, 0.0f, 0.0f, 1.0f);

	//m_QuadShader->Bind();
	//m_FBO->BindTexture(0);
	//m_Quad->Bind();


}

void Scene3D::OnRelease() {

	// Release Models, Shaders, Textures, Resources
	delete m_FBO;

}

void Scene3D::OnWindowCloseEvent(Agata::WindowCloseEvent e) {

	m_Running = false;

}

void Scene3D::OnKeyEvent(Agata::KeyEvent e) {

	if (e.GetKeyCode() == Agata::KeyCode::KeyEscape && e.GetKeyAction() == Agata::KeyAction::Press) {
		m_Running = false;
	}

	if (e.GetKeyCode() == Agata::KeyCode::KeyC && e.GetKeyAction() == Agata::KeyAction::Press) {
		m_Camera->TogglePerson();
	}

	if (e.GetKeyCode() == Agata::KeyCode::KeyZ && e.GetKeyAction() == Agata::KeyAction::Press) {
		m_IsZoom = !m_IsZoom;
		if (m_IsZoom) {
			m_Camera->SetSensitivity(124.44444f);
			m_Camera->GetPerspectiveCameraPropsRef().Fov = 7.0f;
		}
		else {
			m_Camera->SetSensitivity(800.0f);
			m_Camera->GetPerspectiveCameraPropsRef().Fov = 45.0f;
		}
	}

}
void Scene3D::RenderScene() {

	m_SkyboxShader->Bind();
	m_Skybox->OnRender();

	m_TerrainShader->Bind();
	m_Terrain->OnRender();

	m_StaticModelShader->Bind();
	for (auto& model : m_Models) {
		model->OnRender();
	}
	
	for (auto& Item : m_Items) {
		Item->OnRender();
	}

	m_Vehicle->OnRender();
	//m_Vehicle->OnRender();

	m_BillboardShader->Bind();
	m_Tree1->OnRender();
	m_Tree2->OnRender();

	m_FireShader->Bind();
	m_Fire->OnRender();

	m_SkeletalModelShader->Bind();
	m_SkeletalModel->OnRender();

}
