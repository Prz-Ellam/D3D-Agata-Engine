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

	std::shared_ptr<Agata::StaticModel> m_Model;

	std::vector<std::shared_ptr<Agata::StaticModel>> m_Models;

	std::shared_ptr<Agata::Terrain> m_Terrain;
	std::shared_ptr<Agata::Skybox> m_Skybox;
	std::shared_ptr<Agata::Water> m_Water;

	std::shared_ptr<Agata::Shader> m_StaticModelShader;
	std::shared_ptr<Agata::Shader> m_TerrainShader;
	std::shared_ptr<Agata::Shader> m_SkyboxShader;
	std::shared_ptr<Agata::Shader> m_QuadShader;
	std::shared_ptr<Agata::Shader> m_WaterShader;

	std::shared_ptr<Agata::Quad> m_Quad;
};


#endif
