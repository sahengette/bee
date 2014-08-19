#include "stdafx.hpp"
namespace bee{
	struct mesh{
		public :

			bool loadOBJfile(std::string filepath);
			bool loadOBJfilespirit(std::string filepath);

			std::vector<glm::vec3> m_vertex_norm, m_vertex_pos, m_vertex_uv;
			std::vector<GLushort> m_indice;
	};
}
