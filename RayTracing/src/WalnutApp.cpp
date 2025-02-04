#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"

#include "glm/gtc/type_ptr.hpp"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() :m_Camera(45.0f, 0.1f, 100.0f)
	{
		Material& pinkMat = m_Scene.materials.emplace_back();
		pinkMat.albedo = { 1.0f, 0.0f, 1.0f };
		pinkMat.roughness = 0.0f;

		Material& blueMat = m_Scene.materials.emplace_back();
		blueMat.albedo = { 0.2f, 0.3f, 1.0f };
		blueMat.roughness = 0.1f;

		Material& orangeMat = m_Scene.materials.emplace_back();
		orangeMat.albedo = { 0.8f, 0.5f, 0.2f };
		orangeMat.roughness = 0.1f;
		orangeMat.emissionColor = orangeMat.albedo;
		orangeMat.emissionPower = 2.0f;

		{
			Sphere sphere;
			sphere.center = { 0.0f, 0.0f, 0.0f };
			sphere.radius = 1.0f;
			sphere.materialIndex = 0;
			m_Scene.spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.center = { 0.0f, -101.0f, -5.0f };
			sphere.radius = 100.f;
			sphere.materialIndex = 1;
			m_Scene.spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.center = { 2.0f, 0.0f, 0.0f };
			sphere.radius = 1.0f;
			sphere.materialIndex = 2;
			m_Scene.spheres.push_back(sphere);
		}
	}
	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.OnUpdate(ts)) 
		{
			m_Renderer.ResetFrameIndex();
		}
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);
		if (ImGui::Button("Reset"))
		{
			m_Renderer.ResetFrameIndex();
		}
		ImGui::End();

		ImGui::Begin("Scene");
		for (size_t i = 0; i < m_Scene.spheres.size(); i++)
		{
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.spheres[i];
			ImGui::DragFloat3("Center", glm::value_ptr(sphere.center), 0.1f);
			ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
			ImGui::DragInt("MaterialIndex", &sphere.materialIndex, 1.0f, 0, (int)m_Scene.materials.size() - 1);

			ImGui::Separator();
			ImGui::PopID();
		}

		for (size_t i = 0; i < m_Scene.materials.size(); i++)
		{
			ImGui::PushID(i);

			Material& material = m_Scene.materials[i];
			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.albedo), 0.1f);
			ImGui::DragFloat("Roughness", &material.roughness, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &material.metallic, 0.05f, 0.0f, 1.0f);
			ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.emissionColor), 0.1f);
			ImGui::DragFloat("Emission Power", &material.emissionPower, 0.05f, 0.0f, FLT_MAX);

			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}