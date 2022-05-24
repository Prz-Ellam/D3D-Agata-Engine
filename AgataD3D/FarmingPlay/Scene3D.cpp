#include "pch.h"
#include "Scene3D.h"
#include "Joystick.h"
#include <Audio.h>
#include <AudioRManager.h>

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
	Audio::GetInstance().Init();
	AudioRManager::GetInstance().LoadAudioFiles();

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
		DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	m_Win = std::make_shared<Agata::GUI>("Assets//Images//UI//win.png",
		DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	m_IconO = std::make_shared<Agata::GUI>("Assets//Images//UI//objetos.png",
		DirectX::XMFLOAT2(-0.8f, -0.8f), DirectX::XMFLOAT2(0.3f, 0.3f));
	m_IconOL = std::make_shared<Agata::GUI>("Assets//Images//UI//objetosllave.png",
		DirectX::XMFLOAT2(0.8f, -0.8f), DirectX::XMFLOAT2(0.3f, 0.3f));
	m_Menu = std::make_shared<Agata::GUI>("Assets//Images//UI//Menu.png",
		DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));

	m_TextShader = std::make_shared<Agata::Shader>("TextVertex.cso", "TextPixel.cso");
	m_TextShader->Bind();

	m_Text = std::make_shared<Agata::Text>("Assets//Images//Fonts//Fixedsys16x28.png");
	m_cronometro= std::make_shared<Agata::Text>("Assets//Images//Fonts//Fixedsys16x28.png");
    m_cantidad= std::make_shared<Agata::Text>("Assets//Images//Fonts//Fixedsys16x28.png");


	m_SkeletalModelShader = std::make_shared<Agata::Shader>("SkeletalModelVertex.cso", "SkeletalModelPixel.cso");
	m_SkeletalModelShader->Bind();

	float x, z;
	x = 0.0f;
	z = 0.0f;
	m_Forward = Agata::SkeletalModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Personaje//Forward.fbx").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(0.01f, 0.01f, 0.01f)).
		DiffuseTexture("Assets//Models//Personaje//CharacterDiffuse.jpg").
		Build();

	m_Idle = Agata::SkeletalModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Personaje//Idle.fbx").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(0.01f, 0.01f, 0.01f)).
		DiffuseTexture("Assets//Models//Personaje//CharacterDiffuse.jpg").
		Build();

	m_Backward = Agata::SkeletalModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Personaje//Backward.fbx").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(0.01f, 0.01f, 0.01f)).
		DiffuseTexture("Assets//Models//Personaje//CharacterDiffuse.jpg").
		Build();

	m_Sitting = Agata::SkeletalModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Personaje//Sitting.fbx").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(0.01f, 0.01f, 0.01f)).
		DiffuseTexture("Assets//Models//Personaje//CharacterDiffuse.jpg").
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

	x = -9.0f;
	z = -23.0f;
	m_Telescope = Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Telescopio//telescopio.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Telescopio//albedo.png").
		NormalTexture("Assets//Models//Telescopio//normal.png").
		Build();

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
		NormalTexture("Assets//Models//Tractor//normal.png").
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
	        AddCollider("Assets//Colliders//farmC9.aabb").
		AddCollider("Assets//Colliders//farmC10.aabb").
		AddCollider("Assets//Colliders//farmC11.aabb").
		AddCollider("Assets//Colliders//farmC15.aabb").
		Build());
	
	x = 3.0f;
	z = -47.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//plants.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Plants_Texture.png").
	        DefaultCollider(true).
		Build());

	x = -40.0f;//1
	z = -33.0f;//-45
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta1//planta1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta1//Plants_Texture.png").
		AddCollider("Assets//Colliders//planta1C.aabb").
		Build());

	x = -34.0f;//2
	z = 25.0f;//-45
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta1//planta1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta1//Plants_Texture.png").
		AddCollider("Assets//Colliders//planta1C.aabb").
		Build());

	x = 14.0f;//3
	z = -10.0f;//-45
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta2//planta2.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta2//Plants_Texture.png").
		AddCollider("Assets//Colliders//planta2C.aabb").
		Build());

	x = -14.0f; //4
	z = -25.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta3//planta3.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta3//Plants_Texture.png").
		AddCollider("Assets//Colliders//planta3C.aabb").
		Build());

	x = -12.0f; //5
	z = 25.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta3//planta3.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta3//Plants_Texture.png").
		AddCollider("Assets//Colliders//planta3C.aabb").
		Build());

	x = -20.0f; //6
	z = 4.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta4//planta4.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta4//Plants_Texture.png").
		AddCollider("Assets//Colliders//planta4C.aabb").
		Build());


	x = 8.0f; //7
	z = -36.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//plants//Planta5//planta5.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//plants//Planta5//Plants_Texture.png").
		AddCollider("Assets//Colliders//planta5C.aabb").
		Build());
	
	
	x = 74.0f; //1
	z = 25.0f;
	m_Llaves.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//llave2//llave2.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//llave2//wrenchExport_lambert2_R.png").
		AddCollider("Assets//Colliders//llaveC.aabb").
		Build());


	x = 74.0f; //2
	z = 10.0f;
	m_Llaves.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//llave2//llave2.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//llave2//wrenchExport_lambert2_R.png").
		AddCollider("Assets//Colliders//llaveC.aabb").
		Build());

	x = 6.0f;//1
	z = -40.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		AddCollider("Assets//Colliders//tuercaC.aabb").
		Build());

	x = -14.0f;//2
	z = 25.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		AddCollider("Assets//Colliders//tuercaC.aabb").
		Build());

	x = -22.0f;//3
	z = 5.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		AddCollider("Assets//Colliders//tuercaC.aabb").
		Build());

	x = 5.0f; //4
	z = 10.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		AddCollider("Assets//Colliders//tuercaC.aabb").
		Build());

	x = 2.0f;//5
	z = -10.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		AddCollider("Assets//Colliders//tuercaC.aabb").
		Build());

	x = -50.0f;//6
	z = -35.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		AddCollider("Assets//Colliders//tuercaC.aabb").
		Build());

	x = 32.0f; //7
	z = 20.0f;
	m_Items.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//tuerca//tuerca.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//tuerca//NutMat_Roughness.jpeg").
		AddCollider("Assets//Colliders//tuercaC.aabb").
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
	
	x = -30.0f;
	z = -17.0f;
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
	m_Tree[0] = Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Arbol//Tree01.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(5.0f, 5.0f, 5.0f)).
		DiffuseTexture("Assets//Models//Arbol//Wood_002.png").
		NormalTexture("Assets//Models//Arbol//Wood_002_normal.tga.png").
		AddCollider("Assets//Colliders//troncoC1.aabb").
		AddCollider("Assets//Colliders//troncoC2.aabb").
		Build();

	m_Tree[1] = Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Arbol//Tree02.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(5.0f, 5.0f, 5.0f)).
		DiffuseTexture("Assets//Models//Arbol//Bitmap2Material_3_Base_Color.tga.png").
		NormalTexture("Assets//Models//Arbol//Bitmap2Material_3_Normal.tga.png").
		Build();


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
	m_Water = std::make_shared<Agata::Water>(DX::XMFLOAT3(53, 3.5, 0), DX::XMFLOAT3(0, 0, 0),
		DX::XMFLOAT3(2, 1, 4), "Assets//Images//Water//dudv.png", 
		"Assets//Images//Water//normalMap.png", 
		m_Window->GetWidth(), m_Window->GetHeight());

	
	m_BillboardShader = std::make_shared<Agata::Shader>("BillboardVertex.cso", "BillboardPixel.cso");
	m_BillboardShader->Bind();

	m_Tree1 = std::make_shared<Agata::Billboard>("Assets//Images//Billboards//Autumn_Pine.png",
		"Assets//Images//Billboards//Autumn_Pine_Normal.png",
		DX::XMFLOAT3(-36, m_Terrain->GetHeight(-36, -3) + 4.5, -3), DX::XMFLOAT3(5, 5, 5));
	m_Tree2 = std::make_shared<Agata::Billboard>("Assets//Images//Billboards//tree2.png",
		"Assets//Images//Billboards//tree2Norm.png",
		DX::XMFLOAT3(30, m_Terrain->GetHeight(30, -40) + 5, -40), DX::XMFLOAT3(5, 5, 5));

	m_FireShader = std::make_shared<Agata::Shader>("FireVertex.cso", "FirePixel.cso");
	m_FireShader->Bind();
	m_Fire = std::make_shared<Agata::Fire>("Assets//Images//Fire//rampFire.gif", 
		"Assets//Images//Fire//alphaFire.gif", 
		"Assets//Images//Fire//dudv.png", 
		"Assets//Images//Fire//fireNoise.png",
		DX::XMFLOAT3(-30, m_Terrain->GetHeight(-30, -17) + 1, -17), DX::XMFLOAT3(1, 1, 1));

	m_ZoomShader = std::make_shared<Agata::Shader>("ZoomVertex.cso", "ZoomPixel.cso");
	m_ZoomShader->Bind();
	m_SpyGlass = std::make_shared<Agata::Zoom>("Assets//Images//Zoom//zoom.jpg");

	m_Light = std::make_shared<Agata::DirectionLight>(DX::XMFLOAT3(0.0f, 0.0f, -400.0f), 
		DX::XMFLOAT3(1.0f, 0.5f, 0.5f));

	m_Cycle = 180.0f;

	m_TreePositions.push_back(DirectX::XMFLOAT3(-10.0f, m_Terrain->GetHeight(-10.0f, 20.0f), 20.0f));
	m_TreePositions.push_back(DirectX::XMFLOAT3(-9.74f, m_Terrain->GetHeight(-9.74, -1.91), -1.91f));
	m_TreePositions.push_back(DirectX::XMFLOAT3(-41.42f, m_Terrain->GetHeight(-41.42f, -44.31f), -44.31f));
	m_TreePositions.push_back(DirectX::XMFLOAT3(0.02f, m_Terrain->GetHeight(0.02f, -30.89f), -30.89f));
	m_TreePositions.push_back(DirectX::XMFLOAT3(26.13f, m_Terrain->GetHeight(26.13f, -9.79f), -9.79f));
	m_TreePositions.push_back(DirectX::XMFLOAT3(26.68f, m_Terrain->GetHeight(26.68f, 34.47f), 34.47f));
}

void Scene3D::OnRun() {

	// Game Loop 
	Audio::GetInstance().Update();
	m_Timer.Start();
	m_Dt = 1 / 60;
	while (m_Running) {

		Update();
		Render();

		m_Window->Update();

		m_Timer.Stop();
		m_Dt = m_Timer.GetMiliseconds();
		m_Ts += m_Dt;

		m_Timer.Restart();

		Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Juego")->second, 1, 0.035f);

	
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

	if (m_GameState == GAMEPLAY) {

		(m_Cycle > -180) ? m_Cycle -= 2.0f * m_Dt : m_Cycle = 180.0f;

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

		for (auto& Llave : m_Llaves) {
			Llave->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
			Llave->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));
		}

		m_Telescope->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		m_Telescope->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));

		m_Vehicle->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		m_Vehicle->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));

		m_Forward->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		m_Forward->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));
		m_Idle->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		m_Idle->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));
		m_Backward->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		m_Backward->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));
		m_Sitting->GetMaterial()->SetAmbient(DirectX::XMFLOAT4(I, I, I, 1.0f));
		m_Sitting->GetMaterial()->SetDiffuse(DirectX::XMFLOAT4(I + 0.1f, I + 0.1f, I + 0.1f, 1.0f));

		m_Camera->Move(m_Dt);

		if (m_VehicleCollider.IsColliding(m_Camera->GetPosition()) && m_VehicleEnable && !m_IsInVehicle) {
			m_VehicleArea = true;
		}
		else {
			m_VehicleArea = false;
		}

		if (m_SpyGlassCollider.IsColliding(m_Camera->GetPosition()) && !m_IsZoom && !m_IsInVehicle && !m_VehicleArea && !m_IsThirdPerson) {
			m_SpyGlassArea = true;
		}
		else {
			m_SpyGlassArea = false;
		}

		m_VehicleCollider.SetMinX(m_Vehicle->GetPosition().x - 5.0f);
		m_VehicleCollider.SetMinZ(m_Vehicle->GetPosition().z - 5.0f);
		m_VehicleCollider.SetMaxX(m_Vehicle->GetPosition().x + 5.0f);
		m_VehicleCollider.SetMaxZ(m_Vehicle->GetPosition().z + 5.0f);

		for (auto& model : m_Models) {
			model->CheckCollision(m_Camera);
		}


		for (auto& Item : m_Items) {
			if (Item->IsColliding(m_Camera) == true) {
				Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Recoger")->second, 2, 0.035f);

				Item->SetPositionC(DirectX::XMFLOAT3(500.0f, 0.0f, 500.0f));

				cont++;
				m_ItemArea = true;

			}
		}

		for (auto& Llave : m_Llaves) {
			if (Llave->IsColliding(m_Camera) == true) {
				Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Recoger2")->second, 3, 0.035f);
				Llave->SetPositionC(DirectX::XMFLOAT3(500.0f, 0.0f, 500.0f));
				contL++;
				m_ItemArea2 = true;

			}
		}
		
		for (int i = 0; i < m_TreePositions.size(); i++) {
			m_Tree[0]->CheckCollision(m_Camera);
			m_Tree[1]->CheckCollision(m_Camera);
		}
		
		if (m_Cycle < 60) {
			Audio::GetInstance().StopSound(1);
			Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"JuegoR")->second, 5, 0.035f);

		}

		if (m_Cycle < 0 && cont < 14 && contL < 2) {
			m_GameState = LOSE;

		}
		else if (m_Cycle < 0 && cont < 14 && contL == 2) {
			m_GameState = LOSE;
		}
		else if (m_Cycle < 0 && cont == 14 && contL < 2) {
			m_GameState = LOSE;
		}
		else if (contL == 2) {
			m_VehicleEnable = true;
		}

		//tractor
		if (m_VehicleEnable && cont == 14) {
			m_GameState = WIN;//UI de ganador
		}

		//m_Camera->SetY(m_Terrain->GetHeight(m_Camera->GetX(), m_Camera->GetZ()) + 1.665);
		m_Camera->Update(m_Terrain);
		m_Skybox->OnUpdate(m_Dt);
		m_Water->OnUpdate(m_Dt);
		m_Fire->OnUpdate(m_Dt);

		m_Tree1->OnUpdate(m_Dt);
		m_Tree2->OnUpdate(m_Dt);

		if (GetAsyncKeyState(static_cast<int>('W')) & 1 << 16) {
			m_CharacterStates = FORWARD;
		}
		if (GetAsyncKeyState(static_cast<int>('S')) & 1 << 16) {
			m_CharacterStates = BACKWARD;
		}

		if (!(GetAsyncKeyState(static_cast<int>('W')) & 1 << 16) &&
			!(GetAsyncKeyState(static_cast<int>('S')) & 1 << 16)) {
			m_CharacterStates = IDLE;
		}

		if (m_IsInVehicle) {
			m_CharacterStates = SITTING;

		}

		if (m_IsThirdPerson) {

			switch (m_CharacterStates) {
			case FORWARD: {
				Audio::GetInstance().StopSound(4);
				m_Forward->OnUpdate(m_Ts);
				m_Forward->SetPosition(DirectX::XMFLOAT3(m_Camera->GetPosition().x,
					m_Camera->GetPosition().y - 1.665f + 0.2f, m_Camera->GetPosition().z));
				m_Forward->SetRotation(DirectX::XMFLOAT3(0.0f, -(m_Camera->GetYaw() - 90.0f), 0.0f));
				break;
			}
			case IDLE: {
				Audio::GetInstance().StopSound(4);
				m_Idle->OnUpdate(m_Ts);
				m_Idle->SetPosition(DirectX::XMFLOAT3(m_Camera->GetPosition().x,
					m_Camera->GetPosition().y - 1.665f + 0.2f, m_Camera->GetPosition().z));
				m_Idle->SetRotation(DirectX::XMFLOAT3(0.0f, -(m_Camera->GetYaw() - 90.0f), 0.0f));
				break;
			}
			case BACKWARD: {
				Audio::GetInstance().StopSound(4);
				m_Backward->OnUpdate(m_Ts);
				m_Backward->SetPosition(DirectX::XMFLOAT3(m_Camera->GetPosition().x,
					m_Camera->GetPosition().y - 1.665f + 0.2f, m_Camera->GetPosition().z));
				m_Backward->SetRotation(DirectX::XMFLOAT3(0.0f, -(m_Camera->GetYaw() - 90.0f), 0.0f));
				break;
			}
			case SITTING: {
				Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Tractor")->second, 4, 0.25f);
				m_Sitting->OnUpdate(m_Ts);
				m_Sitting->SetPosition(DirectX::XMFLOAT3(m_Camera->GetPosition().x,
					m_Camera->GetPosition().y - 1.665f + 0.2f, m_Camera->GetPosition().z));
				m_Sitting->SetRotation(DirectX::XMFLOAT3(0.0f, -(m_Camera->GetYaw() - 90.0f), 0.0f));
				break;
			}
			}

		}

		if (m_IsInVehicle) {

			m_Vehicle->FollowCamera(m_Camera);
		}


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

}

void Scene3D::Render() {


	switch (m_GameState) {
	case MENU: {
		m_GUIShader->Bind();
		m_Menu->OnRender();
		break;
	}
	case GAMEPLAY: {
		float distance = 2 * (m_Camera->GetY() - m_Water->GetPosition().y);
		m_Camera->ChangePitchDirection();
		//m_Camera->MoveHeight(distance);
		m_Camera->Update(m_Terrain);
		//m_Camera->MoveHeight(-distance);


		Agata::Renderer::Clear(1.0f, 1.0f, 1.0f, 1.0f);
		Agata::Renderer::BeginScene(m_Camera, m_Light);

		m_Water->BeginReflection();
		RenderScene();
		m_Water->EndReflection();

		// Agata::Renderer::EndScene();

		//m_Camera->MoveHeight(distance);
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

		m_IconO->OnRender();
		m_IconOL->OnRender();

		m_TextShader->Bind();
		m_cronometro->DrawString(std::to_string(m_Cycle), DirectX::XMFLOAT2(0.3, 0.9), 1.0f);
		m_Text->DrawString("+ Encuentra los 16 recursos", DirectX::XMFLOAT2(-0.9, 0.9), 0.5f);
		m_Text->DrawString("+ Repara el tractor", DirectX::XMFLOAT2(-0.9, 0.8), 0.5f);

		//if (m_ItemArea) {
			m_cantidad->DrawString(std::to_string(cont), DirectX::XMFLOAT2(-0.6, -0.8), 1.0f);
		//}

		//if (m_ItemArea2) {
			m_cantidad->DrawString(std::to_string(contL), DirectX::XMFLOAT2(0.6, -0.8), 1.0f);
		//}

		//m_FBO->UnbindFramebuffer();

		//Agata::Renderer::Clear(1.0f, 0.0f, 0.0f, 1.0f);

		//m_QuadShader->Bind();
		//m_FBO->BindTexture(0);
		//m_Quad->Bind();
		break;
	}
	case WIN: {
		Audio::GetInstance().StopSound(5);
		Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"win")->second, 6, 0.35f);
		m_GUIShader->Bind();
		m_Win->OnRender();
		break;
	}
	case LOSE: {
		Audio::GetInstance().StopSound(5);
		Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"lose")->second, 7, 0.25f);
		m_GUIShader->Bind();
		m_Icon->OnRender();
		break;
	}
	}


	//m_FBO->BindFrameBuffer();
	//m_FBO->Clear(0, 1, 0, 0);

	


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

	if (e.GetKeyCode() == Agata::KeyCode::KeyF && e.GetKeyAction() == Agata::KeyAction::Press
		&& m_GameState == MENU) {
		m_GameState = GAMEPLAY;
	}

	if (e.GetKeyCode() == Agata::KeyCode::KeyC && e.GetKeyAction() == Agata::KeyAction::Press && !m_IsZoom) {
		m_Camera->TogglePerson();
		m_IsThirdPerson = !m_IsThirdPerson;
	}

	if (e.GetKeyCode() == Agata::KeyCode::KeyV && e.GetKeyAction() == Agata::KeyAction::Press &&
		(m_VehicleArea || m_IsInVehicle)) {
		m_IsInVehicle = !m_IsInVehicle;
		if (m_IsInVehicle) {
			m_Camera->SetSpeed(90.0f);
		}
		else {
			m_Camera->SetSpeed(60.0f);
		}
	}

	if (e.GetKeyCode() == Agata::KeyCode::KeyR && e.GetKeyAction() == Agata::KeyAction::Press) {//reiniciar
		Restart();
	}

	if (e.GetKeyCode() == Agata::KeyCode::KeyZ && e.GetKeyAction() == Agata::KeyAction::Press) {
		m_IsZoom = !m_IsZoom;
		m_Camera->SetPosition(DirectX::XMFLOAT3(-10.32f, m_Terrain->GetHeight(-10.32f, -23.0f), -23.0f));
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

	for (int i = 0; i < m_TreePositions.size(); i++) {
		m_Tree[0]->SetPosition(m_TreePositions[i]);
		m_Tree[1]->SetPosition(m_TreePositions[i]);
		m_Tree[0]->OnRender();
		m_Tree[1]->OnRender();
	}

	for (auto& Item : m_Items) {
			Item->OnRender();
	}

	for (auto& Llave : m_Llaves) {
		Llave->OnRender();
	}

	m_Vehicle->OnRender();

	if (!m_IsZoom) {
		m_Telescope->OnRender();
	}

	m_BillboardShader->Bind();
	m_Tree1->OnRender();
	m_Tree2->OnRender();

	m_SkeletalModelShader->Bind();

	if (m_IsThirdPerson) {

		switch (m_CharacterStates) {
		case FORWARD: {
			m_Forward->OnRender();
			break;
		}
		case IDLE: {
			m_Idle->OnRender();
			break;
		}
		case BACKWARD: {
			m_Backward->OnRender();
			break;
		}
		case SITTING: {
			m_Sitting->OnRender();
			break;
		}
		}
		
	}


	m_FireShader->Bind();
	m_Fire->OnRender();

}

void Scene3D::Restart() {

	m_Light = std::make_shared<Agata::DirectionLight>(DX::XMFLOAT3(0.0f, 0.0f, -400.0f), 
		DX::XMFLOAT3(1.0f, 0.5f, 0.5f));

	m_Cycle = 180.0f;

	m_Skybox->SetBlendFactor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));

	Agata::PerspectiveCameraProps properties = {
		45.0f,
		static_cast<float>(m_Window->GetWidth()) / m_Window->GetHeight(),
		0.1f,
		1000.0f
	};
	m_Camera = std::make_unique<Agata::Camera>(properties, 60.0f, 800.0f);

	m_GameState = GAMEPLAY;
	m_CharacterStates = IDLE;

	cont = 0;
	contL = 0;

	m_IsZoom = false;
	m_SpyGlassArea = false;
	m_VehicleArea = false;
	m_ItemArea = false;
	m_ItemArea2 = false;
	m_VehicleEnable = false;
	m_IsThirdPerson = true;
	m_IsInVehicle = false;

	m_Items[0]->SetPositionC(DirectX::XMFLOAT3(-40.0f, m_Terrain->GetHeight(-40.0f, -33.0f), -33.0f));
	m_Items[1]->SetPositionC(DirectX::XMFLOAT3(-34.0f, m_Terrain->GetHeight(-34.0f, 25.0f), 25.0f));
	m_Items[2]->SetPositionC(DirectX::XMFLOAT3(14.0f, m_Terrain->GetHeight(14.0f, -10.0f), -10.0f));
	m_Items[3]->SetPositionC(DirectX::XMFLOAT3(-14.0f, m_Terrain->GetHeight(-14.0f, -25.0f), -25.0f));
	m_Items[4]->SetPositionC(DirectX::XMFLOAT3(-12.0f, m_Terrain->GetHeight(-12.0f, 25.0f), 25.0f));
	m_Items[5]->SetPositionC(DirectX::XMFLOAT3(-20.0f, m_Terrain->GetHeight(-20.0f, 4.0f), 4.0f));
	m_Items[6]->SetPositionC(DirectX::XMFLOAT3(8.0f, m_Terrain->GetHeight(8.0f, -36.0f), -36.0f));
	m_Items[7]->SetPositionC(DirectX::XMFLOAT3(-20.0f, m_Terrain->GetHeight(-20.0f, 4.0f), 4.0f));
	m_Items[8]->SetPositionC(DirectX::XMFLOAT3(6.0f, m_Terrain->GetHeight(6.0f, -40.0f), -40.0f));
	m_Items[9]->SetPositionC(DirectX::XMFLOAT3(-14.0f, m_Terrain->GetHeight(-14.0f, 25.0f), 25.0f));
	m_Items[10]->SetPositionC(DirectX::XMFLOAT3(-22.0f, m_Terrain->GetHeight(-22.0f, 5.0f), 5.0f));
	m_Items[11]->SetPositionC(DirectX::XMFLOAT3(5.0f, m_Terrain->GetHeight(5.0f, 10.0f), 10.0f));
	m_Items[12]->SetPositionC(DirectX::XMFLOAT3(2.0f, m_Terrain->GetHeight(2.0f, -10.0f), -10.0f));
	m_Items[13]->SetPositionC(DirectX::XMFLOAT3(-50.0f, m_Terrain->GetHeight(-50.0f, -35.0f), -35.0f));

	m_Llaves[0]->SetPositionC(DirectX::XMFLOAT3(74.0f, m_Terrain->GetHeight(74.0f, 25.0f), 25.0f));
	m_Llaves[1]->SetPositionC(DirectX::XMFLOAT3(74.0f, m_Terrain->GetHeight(74.0f, 10.0f), 10.0f));

	m_Vehicle->SetPositionC(DirectX::XMFLOAT3(10.0f, m_Terrain->GetHeight(10.0f, -20.0f), -20.0f));
	m_Vehicle->SetRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	
	Audio::GetInstance().StopSound(6);
	Audio::GetInstance().StopSound(7);
	Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find((char*)"Juego")->second, 1, 0.035f);

}
