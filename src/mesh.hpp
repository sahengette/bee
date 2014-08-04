#include "stdafx.hpp"
namespace bee{
	struct mesh{
		public :

			bool loadOBJfile(std::string filepath);

			std::vector<GLfloat> m_vertex_pos, m_vertex_tex, m_vertex_norm;
			std::vector<GLushort> m_indice;
	};
}
