#ifndef SCENE_3D_H
#define SCENE_3D_H
#pragma once

#include "Agata.h"

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
private:
	bool m_Running;
	Agata::Timer m_Timer;
	double m_Dt;
	double m_Cycle;

	Agata::Framebuffer* m_FBO;

	std::unique_ptr<Agata::Camera> m_Camera;

	std::shared_ptr<Agata::StaticModel> m_Vehicle;

	std::vector<std::shared_ptr<Agata::StaticModel>> m_Models;

	std::shared_ptr<Agata::Terrain> m_Terrain;
	std::shared_ptr<Agata::Skybox> m_Skybox;
	std::shared_ptr<Agata::Water> m_Water;

	std::shared_ptr<Agata::Billboard> m_Tree1, m_Tree2;
	std::shared_ptr<Agata::Fire> m_Fire;

	std::shared_ptr<Agata::Shader> m_StaticModelShader;
	std::shared_ptr<Agata::Shader> m_TerrainShader;
	std::shared_ptr<Agata::Shader> m_SkyboxShader;
	std::shared_ptr<Agata::Shader> m_QuadShader;
	std::shared_ptr<Agata::Shader> m_WaterShader;
	std::shared_ptr<Agata::Shader> m_BillboardShader;
	std::shared_ptr<Agata::Shader> m_FireShader;
	std::shared_ptr<Agata::Shader> m_ZoomShader;

	std::shared_ptr<Agata::Quad> m_Quad;
	std::shared_ptr<Agata::Zoom> m_SpyGlass;

	std::shared_ptr<Agata::DirectionLight> m_Light;


	bool m_IsZoom = false;
};


#endif
