#include "SingleVertex.h"

SingleVertex::SingleVertex(glm::vec3& init_Pos)
	: m_VertexID(0), v_Pos(init_Pos), v_Color(0.0f, 0.0f, 0.0f, 1.0f), v_TexCoord({0.0, 0.0, 0.0}){}

SingleVertex::~SingleVertex()
{}

std::vector<float> SingleVertex::BuildVertex(glm::vec3 v_Pos, glm::vec4 v_Color, std::vector<float> v_TexCoord)
{
	// TODO: concatenate input coordinates into valid output here
	return std::vector<float>();
}
