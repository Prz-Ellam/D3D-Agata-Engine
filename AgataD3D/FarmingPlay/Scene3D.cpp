#include "pch.h"
#include "Scene3D.h"

#include "Joystick.h"

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
		.BlendMap("Assets//Images//Terrain//Splat_Map.png")
		.TilingFactor(40.0f)
		.Build();

	m_StaticModelShader = std::make_shared<Agata::Shader>("StaticModelVertex.cso", "StaticModelPixel.cso");
	m_StaticModelShader->Bind();

	float x, z;
	x = 0.0f;
	z = 0.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Casa 1//rural1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Casa 1//diff.png").
		Build());

	x = 10.0f;
	z = 20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Casa 2//rural5.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Casa 2//diffuse.png").
		Build());

	x = -10.0f;
	z = -20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Telescopio//telescopio.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Telescopio//albedo.png").
		Build());

	x = 10.0f;
	z = -20.0f;
	m_Vehicle = Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Tractor//tractor.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(0.8f, 0.8f, 0.8f)).
		DiffuseTexture("Assets//Models//Tractor//foto2.jpg").
		Build();
	
	x = 20.0f;
	z = -40.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Casa 3//casa1.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Casa 3//House4_lambert1_AlbedoTransparency.png").
		Build());

	x = -17.0f;
	z = -45.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//church//church.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//church//chiesa_Base_Color.jpg").
		Build());

	x = 5.0f;
	z = -12.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Table//Table.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Table//DefaultMaterial_Base_Color.png").
		Build());

	x = -15.0f;
	z = -16.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//kiosk//kiosco.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//kiosk//3.png").
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

	x = 18.0f;
	z = 8.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//bench//bench.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//bench//wooden_bench_d.tga.png").
		Build());

	x = -18.0f;
	z = -3.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//bench//bench.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//bench//wooden_bench_d.tga.png").
		Build());

	x = -15.0f;
	z = 10.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Stand//stand.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//Stand//FoodStand_Base_Color.png").
		Build());

	x = 22.0f;
	z = 20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//toilet//toilet.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)).
		DiffuseTexture("Assets//Models//toilet//toilet.png").
		Build());

	
	x = -10.0f;
	z = 20.0f;
	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Arbol//Tree01.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(5.0f, 5.0f, 5.0f)).
		DiffuseTexture("Assets//Models//Arbol//Wood_002.png").
		Build());

	m_Models.push_back(Agata::StaticModelBuilder::GenerateParams().
		ModelPath("Assets//Models//Arbol//Tree02.obj").
		Position(DX::XMFLOAT3(x, m_Terrain->GetHeight(x, z), z)).
		Rotation(DX::XMFLOAT3(0.0f, 0.0f, 0.0f)).
		Scale(DX::XMFLOAT3(5.0f, 5.0f, 5.0f)).
		DiffuseTexture("Assets//Models//Arbol//Bitmap2Material_3_Base_Color.tga.png").
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
	m_Water = std::make_shared<Agata::Water>(DX::XMFLOAT3(30, 5, 30), DX::XMFLOAT3(0, 0, 0),
		DX::XMFLOAT3(1, 1, 1), "Assets//Images//Water//dudv.png", 
		"Assets//Images//Water//normalMap.png", 
		m_Window->GetWidth(), m_Window->GetHeight());

	m_Cycle = 0.0f;

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
		m_Cycle += m_Dt;
		if (m_Cycle >= 360.0f) {
			m_Cycle = 0.0f;
		}
		m_Timer.Restart();

	}

}

#define MORNING_TO_DAY_MIN 20.0f
#define MORNING_TO_DAY_MAX 45.0f

#define DAY_TO_SUNSET_MIN 135.0f
#define DAY_TO_SUNSET_MAX 160.0f

#define SUNSET_TO_NIGHT_MIN 180.0f
#define SUNSET_TO_NIGHT_MAX 205.0f

#define NIGHT_TO_MORNING_MIN 315.0f
#define NIGHT_TO_MORNING_MAX 340.0f

void Scene3D::Update() {
	
	m_Camera->Move(m_Dt);
	m_Camera->SetY(m_Terrain->GetHeight(m_Camera->GetX(), m_Camera->GetZ()));
	m_Camera->Update();
	m_Skybox->OnUpdate(m_Dt);
	m_Water->OnUpdate(m_Dt);

	//m_Vehicle->FollowCamera(m_Camera);


	if (m_Cycle >= MORNING_TO_DAY_MIN && m_Cycle <= MORNING_TO_DAY_MAX) {

		// Conversion de [MORNING_TO_DAY_MIN, MORNING_TO_DAY_MAX] a [0, 1]
		float MorningLength = (MORNING_TO_DAY_MAX - MORNING_TO_DAY_MIN);
		float t = (m_Cycle - MORNING_TO_DAY_MIN) / MorningLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(1 - t, t, 0.0f, 0.0f));

	}

	if (m_Cycle >= DAY_TO_SUNSET_MIN && m_Cycle <= DAY_TO_SUNSET_MAX) {

		// Conversion de [DAY_TO_SUNSET_MIN, DAY_TO_SUNSET_MAX] a [0, 1]
		float DayLength = (DAY_TO_SUNSET_MAX - DAY_TO_SUNSET_MIN);
		float t = (m_Cycle - DAY_TO_SUNSET_MIN) / DayLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(0.0f, 1 - t, t, 0.0f));

	}

	if (m_Cycle >= SUNSET_TO_NIGHT_MIN && m_Cycle <= SUNSET_TO_NIGHT_MAX) {

		// Conversion de [SUNSET_TO_NIGHT_MIN, SUNSET_TO_NIGHT_MAX] a [0, 1]
		float SunsetLength = (SUNSET_TO_NIGHT_MAX - SUNSET_TO_NIGHT_MIN);
		float t = (m_Cycle - SUNSET_TO_NIGHT_MIN) / SunsetLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(0.0f, 0.0f, 1 - t, t));

	}

	if (m_Cycle >= NIGHT_TO_MORNING_MIN && m_Cycle <= NIGHT_TO_MORNING_MAX) {

		// Conversion de [NIGHT_TO_MORNING_MIN, NIGHT_TO_MORNING_MAX] a [0, 1]
		float NightLength = (NIGHT_TO_MORNING_MAX - NIGHT_TO_MORNING_MIN);
		float t = (m_Cycle - NIGHT_TO_MORNING_MIN) / NightLength;

		// Conversion de interpolacion lineal a smootherstep
		t = t * t * t * (t * (6.0f * t - 15.0f) + 10.0f);

		m_Skybox->SetBlendFactor(DX::XMFLOAT4(t, 0.0f, 0.0f, 1 - t));

	}

}

void Scene3D::Render() {

	//m_FBO->BindFrameBuffer();
	//m_FBO->Clear(0, 1, 0, 0);

	float distance = 2 * (m_Camera->GetY() - m_Water->GetPosition().y);
	m_Camera->ChangePitchDirection();
	m_Camera->MoveHeight(-distance);
	m_Camera->Update();

	Agata::Renderer::Clear(1.0f, 1.0f, 1.0f, 1.0f);
	Agata::Renderer::BeginScene(m_Camera);

	m_Water->BeginReflection();
	RenderScene();
	m_Water->EndReflection();

	// Agata::Renderer::EndScene();



	m_Camera->MoveHeight(distance);
	m_Camera->ChangePitchDirection();
	m_Camera->Update();

	Agata::Renderer::Clear(1.0f, 1.0f, 1.0f, 1.0f);
	Agata::Renderer::BeginScene(m_Camera);

	m_Water->BeginRefraction();
	RenderScene();
	m_Water->EndRefraction();

	// Agata::Renderer::EndScene();



	Agata::Renderer::BeginScene(m_Camera);
	Agata::Renderer::Clear(0.2f, 0.2f, 0.2f, 1.0f);

	RenderScene();

	m_WaterShader->Bind();
	m_Water->OnRender();

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
	//m_Vehicle->OnRender();

}
