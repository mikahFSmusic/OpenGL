#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

class SingleVertex
{
private: 
	unsigned int m_VertexID;
public:
	glm::vec3 v_Pos;
	glm::vec4 v_Color;
	std::vector<float> v_TexCoord;
	std::vector<float> vtx_Out[];

	SingleVertex(glm::vec3& init_Pos);
	~SingleVertex();

	std::vector<float> BuildVertex(glm::vec3 v_Pos, glm::vec4 v_Color, std::vector<float> v_TexCoord);
};

