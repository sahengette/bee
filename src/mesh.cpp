#include "mesh.hpp"


bool bee::mesh::loadOBJfile(std::string filepath)
{
	std::ifstream file;
	file.open(filepath);
	if (!file)
	{
		std::cerr << "Impossile d'ouvrir le fichier : " << filepath << std::endl;
		return false;
	}
	std::string l_courante;

	while(getline(file, l_courante))
	{
		if (l_courante.substr(0,2)== "v ")
		{
			std::istringstream tri_v(l_courante.substr(2));
			glm::vec3 v;
			tri_v >> v.x >> v.y >> v.z ;
			m_vertex_pos.push_back(v.x);
			m_vertex_pos.push_back(v.y);
			m_vertex_pos.push_back(v.z);
		}
		else if(l_courante.substr(0,2)== "f ")
		{
			std::istringstream tri_f(l_courante.substr(2));
			glm::vec3 f;
			tri_f >> f.x >> f.y >> f.z ;
			m_indice.push_back((f.x)-1 );
			m_indice.push_back((f.y)-1 );
			m_indice.push_back((f.z)-1 );
		}
	}
		m_vertex_norm.resize(m_vertex_pos.size(), 0.0f);
		for (unsigned int i = 0; i < m_indice.size() ; i +=3)
		{
			GLushort ia = m_indice[i] * 3;
			GLushort ib = m_indice[i+1] * 3;
			GLushort ic = m_indice[i+2] * 3;

			glm::vec3 a = glm::vec3(m_vertex_pos[ia], m_vertex_pos[ia+1], m_vertex_pos[ia+2]);
			glm::vec3 b = glm::vec3(m_vertex_pos[ib], m_vertex_pos[ib+1], m_vertex_pos[ib+2]);
			glm::vec3 c = glm::vec3(m_vertex_pos[ic], m_vertex_pos[ic+1], m_vertex_pos[ic+2]);

			glm::vec3 normal = glm::normalize(
			glm::cross(b - a, c - a));
			m_vertex_norm[ia] = normal.x;
			m_vertex_norm[ia+1] = normal.y;
			m_vertex_norm[ia+2] = normal.z;
			
			m_vertex_norm[ib] = normal.x;
			m_vertex_norm[ib+1] = normal.y;
			m_vertex_norm[ib+2] = normal.z;
			
			m_vertex_norm[ic] = normal.x;
			m_vertex_norm[ic+1] = normal.y;
			m_vertex_norm[ic+2] = normal.z;

			return true;
		}
	return true;
}
