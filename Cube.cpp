#include "Cube.h"
CCube::CCube()
{}
CCube::~CCube()
{
	Release();
}
void CCube::Create(string filename)
{
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();


	// Write the code to add interleaved vertex attributes to the VBO

	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); //first side
	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, 1.0f)); //second side
	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));

	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); //third side
	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));


	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, 1.0f)); //fourth side
	m_controlPoints.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, -1.0f));


	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, -1.0f)); //fith side
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); //sixth side
	m_controlPoints.push_back(glm::vec3(- 1.0f, 1.0f, -1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	m_controlPoints.push_back(glm::vec3(1.0f, -1.0f, 1.0f));




	texCoord.push_back(glm::vec2(0.000059f, 1.0f - 0.000004f));
	texCoord.push_back(glm::vec2(0.000103f, 1.0f - 0.336048f));
	texCoord.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
	texCoord.push_back(glm::vec2(1.000023f, 1.0f - 0.000013f));
	texCoord.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
	texCoord.push_back(glm::vec2(0.999958f, 1.0f - 0.336064f));
	texCoord.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));

	texCoord.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
	texCoord.push_back(glm::vec2(0.667969f, 1.0f - 0.671889f));
	texCoord.push_back(glm::vec2(1.000023f, 1.0f - 0.000013f));
	texCoord.push_back(glm::vec2(0.668104f, 1.0f - 0.000013f));
	texCoord.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
	texCoord.push_back(glm::vec2(0.000059f, 1.0f - 0.000004f));
	texCoord.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));

	texCoord.push_back(glm::vec2(0.336098f, 1.0f - 0.000071f));
	texCoord.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
	texCoord.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
	texCoord.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
	texCoord.push_back(glm::vec2(1.000004f, 1.0f - 0.671847f));
	texCoord.push_back(glm::vec2(0.999958f, 1.0f - 0.336064f));
	texCoord.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));


	texCoord.push_back(glm::vec2(0.668104f, 1.0f - 0.000013f));
	texCoord.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
	texCoord.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));
	texCoord.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
	texCoord.push_back(glm::vec2(0.668104f, 1.0f - 0.000013f));
	texCoord.push_back(glm::vec2(0.336098f, 1.0f - 0.000071f));
	texCoord.push_back(glm::vec2(0.000103f, 1.0f - 0.336048f));


	texCoord.push_back(glm::vec2(0.000004f, 1.0f - 0.671870f));
	texCoord.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
	texCoord.push_back(glm::vec2(0.000103f, 1.0f - 0.336048f));
	texCoord.push_back(glm::vec2(0.336024f, 1.0f - 0.671877f));
	texCoord.push_back(glm::vec2(0.335973f, 1.0f - 0.335903f));
	texCoord.push_back(glm::vec2(0.667969f, 1.0f - 0.671889f));
	texCoord.push_back(glm::vec2(1.000004f, 1.0f - 0.671847f));
	texCoord.push_back(glm::vec2(0.667979f, 1.0f - 0.335851f));


	for (unsigned int i = 0; i < m_controlPoints.size(); i++) {
		//float t = (float)i / 100.0f;
		m_vbo.AddData(&m_controlPoints[i], sizeof(glm::vec3));
		m_vbo.AddData(&texCoord[i], sizeof(glm::vec2));
		m_vbo.AddData(&m_controlPoints[i], sizeof(glm::vec3));
	}


	// Upload data to GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}
void CCube::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	// Call glDrawArrays to render each side
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, m_controlPoints.size());
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 индексов начинающихся с 0. -> 12 треугольников -> 6 граней.
}
void CCube::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}