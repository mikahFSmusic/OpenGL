#include "Renderer.h"
#include "imgui/imgui.h"

#include "TestTexture2D.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -100.0f, 100.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 40), m_TranslationB(400, 200, 100)
	{
		float positions[] =
		{
			-50.0f, -50.0f, 50.0,  0.0f, 0.0f,  0.0f,  0.18f, 0.6f, 0.96f, 1.0f,    // 0
			 50.0f, -50.0f, 50.0,  1.0f, 0.0f,  0.0f,  0.18f, 0.2f, 0.96f, 1.0f,    // 1
			 50.0f,  50.0f, 50.0,  1.0f, 1.0f,  0.0f,  0.18f, 0.6f, 0.96f, 1.0f,    // 2
			-50.0f,  50.0f, 50.0,  0.0f, 1.0f,  0.0f,  0.90f, 0.6f, 0.96f, 1.0f,    // 3

			-100.0f, -100.0f,  50.0,  0.0f, 0.0f,  1.0f,  1.0f, 0.20f, 0.24f, 1.0f, // 4
			   0.0f, -100.0f,  50.0,  1.0f, 0.0f,  1.0f,  1.0f, 0.93f, 0.24f, 1.0f, // 5
			   0.0f,    0.0f,  50.0,  1.0f, 1.0f,  1.0f,  1.0f, 0.93f, 0.24f, 0.2f, // 6
			-100.0f,    0.0f,  50.0,  0.0f, 1.0f,  1.0f,  1.0f, 0.93f, 0.70f, 1.0f  // 7
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		
		// init vertex buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 10 * sizeof(float));
		// init vertex buffer layout
		VertexBufferLayout layout;
		// position
		layout.Push<float>(3);
		// tex coord
		layout.Push<float>(2);
		// tex index
		layout.Push<float>(1);
		// color
		layout.Push<float>(4);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);
		
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		m_TextureA = std::make_unique<Texture>("res/textures/cancel_blank_back.png");
		m_TextureB = std::make_unique<Texture>("res/textures/wood_stripe_texture.png");
		
		
		
	}
	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.2f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		// Bind the textures to the relevant slots
		m_TextureA->Bind(0);
		m_TextureB->Bind(1);

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			// set uniform to slot 0
			int samplers[2] = { 0, 1 };
			m_Shader->SetUniform1iv("u_Textures", 2, samplers);
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}