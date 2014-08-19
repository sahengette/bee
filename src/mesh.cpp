#include "mesh.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/lambda/lambda.hpp>
using namespace boost::spirit;
using namespace boost::phoenix;
struct obj_p{
	std::vector<float> p, n, t;
	std::vector<unsigned int> ip, in, it;
	std::string mtl;
};

struct obj_grammar : qi::grammar<std::string::iterator, qi::blank_type>
{
	obj_grammar(obj_p& O) : obj_grammar::base_type(start)
	{
		using qi::float_;
		using qi::uint_;
		using qi::_1;
		using qi::char_;
		using qi::eol;
		using qi::eps;
		using boost::phoenix::at;

		v = "v" >> float_[ push_back(boost::phoenix::ref(O.p) , _1) ] >> float_[push_back(boost::phoenix::ref(O.p) , _1)] >> float_[push_back(boost::phoenix::ref(O.p) , _1)] >> eol;

		vn = "vn" >> float_[push_back(boost::phoenix::ref(O.n) , _1)] >> float_[push_back(boost::phoenix::ref(O.n) , _1)] >> float_[push_back(boost::phoenix::ref(O.n) , _1)] >> eol;

		vt = "vt" >> float_[push_back(boost::phoenix::ref(O.t) , _1)] >> float_[push_back(boost::phoenix::ref(O.t) , _1)] >> eol;


		comment = *(char_ - eol) >> eol;

		usemtl = "usemtl" >> comment;

		vertex_attrib = v | vn | vt ;

		face_component = uint_[ push_back(boost::phoenix::ref(O.ip), _1 - 1 ) ] >> -char_('/') >> -uint_[ push_back(boost::phoenix::ref(O.it), _1 - 1 ) ] >> -char_('/') >> -uint_[ push_back(boost::phoenix::ref(O.in), _1 - 1 ) ] ;

		f = "f" >> face_component >> face_component >> face_component >> eol;

		mtllib = "mtllib" >> *(char_ [push_back(boost::phoenix::ref(O.mtl), _1)] - eol) >> eol;
		start = *(vertex_attrib | f | mtllib | comment);

	}

	qi::rule<std::string::iterator, qi::blank_type>start, vertex_attrib, v, vn, vt, face_component, f, comment, mtllib, usemtl;
};



bool bee::mesh::loadOBJfilespirit(std::string filepath)
{
	std::ifstream objfile(filepath.c_str(), std::ios::in);
	char buffer;
	std::string chaine;

	while(objfile.get(buffer))
	{
		chaine.push_back(buffer);
	}

	obj_p mesh_t;
	obj_grammar grammar(mesh_t);
	std::cout << " début du parsing ..." ;

	phrase_parse(chaine.begin(), chaine.end(), grammar, qi::blank);
	for (unsigned int i = 0; i < mesh_t.ip.size() ; i++)
	{
		unsigned int cpi = mesh_t.ip[i]*3;
		glm::vec3 cvec = glm::vec3(mesh_t.p[cpi],mesh_t.p[cpi+1],mesh_t.p[cpi+2]);
		m_vertex_pos.push_back(cvec);
	}
	for (unsigned int i = 0; i < mesh_t.in.size() ; i++)
	{
		unsigned int cni = mesh_t.in[i]*3;
		glm::vec3 cvec = glm::vec3(mesh_t.n[cni],mesh_t.n[cni+1],mesh_t.n[cni+2]);
		m_vertex_norm.push_back(cvec);
	}
	std::cout << " fin du parsing ..." ;
	return true;
}
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
			m_vertex_pos.push_back(v);
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
	m_vertex_norm.resize(m_vertex_pos.size(), glm::vec3(0.0f));
	for (unsigned int i = 0; i < m_indice.size() ; i +=3)
	{
		//	std::cout << m_indice[i] << " " << m_indice[i+1] << " " << m_indice[i+2] << " ";
		glm::vec3 a,b,c;
		a = m_vertex_pos[m_indice[i]];
		b = m_vertex_pos[m_indice[i+1]];
		c = m_vertex_pos[m_indice[i+2]];
		glm::vec3 cotea = b - a;
		glm::vec3 coteb = c - a;
		glm::vec3 normal = glm::normalize(glm::cross(cotea, coteb));
		//std::cout << normal.x << "  " << normal.y << "  " <<normal.z<< " | ";
		m_vertex_norm[m_indice[i]] = normal;
		m_vertex_norm[m_indice[i+1]] = normal;
		m_vertex_norm[m_indice[i+2]] = normal;
		std::cout << m_vertex_norm[i].x << "  " << m_vertex_norm[i].y << "   " << m_vertex_norm[i].z << " | " ;


	}
	return true;
}
