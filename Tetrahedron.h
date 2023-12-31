#pragma once
#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"
// Class for generating a unit cube
class CTetrahedron
{
public:
	CTetrahedron();
	~CTetrahedron();
	void Create(string filename);
	void Render();
	void Release();
private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
	vector<glm::vec3> m_controlPoints;
	vector<glm::vec3> normal;
	vector<glm::vec2> texCoord;
};