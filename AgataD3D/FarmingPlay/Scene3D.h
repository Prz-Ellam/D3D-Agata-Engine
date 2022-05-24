#ifndef SCENE_3D_H
#define SCENE_3D_H
#pragma once

#include "Agata.h"
#include "Joystick.h"

class Scene3D : public Agata::Scene {
public:
	Scene3D(const Agata::WindowParams& windowParams);
	Scene3D(const Scene3D& rhs) = default;
	Scene3D(Scene3D&& rhs) noexcept = default;
	virtual ~Scene3D();

	Scene3D& operator=(const Scene3D& rhs) = default;
	Scene3D& operator=(Scene3D&& rhs) noexcept = default;

	virtual void OnInit() override;
	virtual void OnRun() override;
	virtual void OnRelease() override;

	virtual void OnWindowCloseEvent(Agata::WindowCloseEvent e) override;
	virtual void OnKeyEvent(Agata::KeyEvent e) override;

	void Update();
	void Render();

	void RenderScene();
	void Restart();
private:
	bool m_Running;
	int cont = 0;
	int contL = 0;
	Agata::Timer m_Timer;
	double m_Dt, m_Ts = 0.0f;
	double m_Cycle;

	std::unique_ptr<Agata::Camera> m_Camera;

	std::shared_ptr<Agata::StaticModel> m_Vehicle;
	std::shared_ptr<Agata::StaticModel> m_Telescope;

	std::shared_ptr<Agata::SkeletalModel> m_Forward;
	std::shared_ptr<Agata::SkeletalModel> m_Idle;
	std::shared_ptr<Agata::SkeletalModel> m_Backward;
	std::shared_ptr<Agata::SkeletalModel> m_Sitting;

	std::vector<std::shared_ptr<Agata::StaticModel>> m_Models;
	std::shared_ptr<Agata::StaticModel> m_Tree[2];
	std::vector<std::shared_ptr<Agata::StaticModel>> m_Items;
	std::vector<std::shared_ptr<Agata::StaticModel>> m_Llaves;

	std::shared_ptr<Agata::Terrain> m_Terrain;
	std::shared_ptr<Agata::Skybox> m_Skybox;
	std::shared_ptr<Agata::Water> m_Water;

	std::shared_ptr<Agata::Billboard> m_Tree1, m_Tree2, m_Pole;
	std::shared_ptr<Agata::Fire> m_Fire;

	std::shared_ptr<Agata::Shader> m_StaticModelShader;
	std::shared_ptr<Agata::Shader> m_SkeletalModelShader;
	std::shared_ptr<Agata::Shader> m_TerrainShader;
	std::shared_ptr<Agata::Shader> m_SkyboxShader;
	std::shared_ptr<Agata::Shader> m_QuadShader;
	std::shared_ptr<Agata::Shader> m_WaterShader;
	std::shared_ptr<Agata::Shader> m_BillboardShader;
	std::shared_ptr<Agata::Shader> m_FireShader;
	std::shared_ptr<Agata::Shader> m_ZoomShader;
	std::shared_ptr<Agata::Shader> m_GUIShader;
	std::shared_ptr<Agata::Shader> m_TextShader;

	std::shared_ptr<Agata::Quad> m_Quad;
	std::shared_ptr<Agata::Zoom> m_SpyGlass;
	std::shared_ptr<Agata::GUI> m_VehicleGUI;
	std::shared_ptr<Agata::GUI> m_GUI;
	std::shared_ptr<Agata::GUI> m_Icon;
	std::shared_ptr<Agata::GUI> m_Win;
	std::shared_ptr<Agata::GUI> m_IconO;
	std::shared_ptr<Agata::GUI> m_IconOL;
	std::shared_ptr<Agata::GUI> m_Menu;
	std::shared_ptr<Agata::Text> m_Text;
	std::shared_ptr<Agata::Text> m_cronometro;
	std::shared_ptr<Agata::Text> m_cantidad;

	std::shared_ptr<Agata::DirectionLight> m_Light;

	std::vector<DirectX::XMFLOAT3> m_TreePositions;

	Agata::BoxCollider m_SpyGlassCollider;
	Agata::BoxCollider m_VehicleCollider;
	Agata::BoxCollider m_VehicleCollision;
	bool m_IsZoom = false;
	bool m_SpyGlassArea = false;
	bool m_VehicleArea = false;
	bool m_ItemArea = false;
	bool m_ItemArea2 = false;
	bool m_VehicleEnable = false;
	bool m_IsThirdPerson = true;
	bool m_IsInVehicle = false;

	Agata::Joystick m_Joystick;

	enum AnimatedModel {
		IDLE,
		FORWARD,
		BACKWARD,
		SITTING
	} m_CharacterStates = IDLE;

	enum GameState {
		MENU,
		GAMEPLAY,
		WIN,
		LOSE
	} m_GameState = MENU;
};

#endif
