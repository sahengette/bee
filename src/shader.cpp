#include "shader.hpp"
bee::Program* bee::Program::m_globalprograminuse = NULL;
bee::Program::Program(bee::VertexShader* vtx, FragmentShader* frg)  
{
		m_vtx=vtx; 
		m_frg=frg; 
		m_usable=false;
		m_program = glCreateProgram();
		glAttachShader(m_program, vtx->getID());
		glAttachShader(m_program, frg->getID());
}
bool bee::Program::linkProgram()
{
		if(!m_usable)//seulement si le shader n'est pas deja finalise
		{
				glLinkProgram(m_program);

	glBindAttribLocation(m_program, 1, "vertex_in");
				GLsizei size;
				GLint status;
				glGetProgramiv(m_program, GL_LINK_STATUS, &status);
				if(status != GL_TRUE)
				{
						glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &size);
						m_log = (char*)malloc(size + 1);
						memset(m_log, '\0', size + 1);
						glGetProgramInfoLog(m_program, size, &size, m_log);
						std::cerr << "Impossible de \"linker\" le shader : " << m_log;

						free(m_log);
						m_usable = false;
						return false;
				}
				m_usable = true;
				return true;

		}
		return true;


}
void bee::Program::useProgram()
{
		if(m_globalprograminuse == NULL)
		{
				glUseProgram(m_program);
				m_globalprograminuse = this;
		}
		else std::cerr << "Impossible d'activer le program car un autre program est déjà activé " << std::endl;
}

void bee::Program::unuseProgram()
{
		glUseProgram(0);
		m_globalprograminuse = NULL;
}
bee::shader::~shader ()
{
}
bee::shader::shader() : m_usable(false), m_source(NULL), m_name(NULL)
{
}
bee::VertexShader::VertexShader()
{
		createShader();
}
bee::FragmentShader::FragmentShader()
{
		createShader();
}
bool bee::shader::compile()
{
		if (m_usable == true) return true;
		else{

				if(m_source == NULL)
				{
						std::cerr << "Le shader ne possède pas de code source, impossible de le compiler" << std::endl;
						m_usable = false;
						return false;
				}

				else{

						shaderSource(1, (const GLchar**) &m_source, NULL);

						compileShader();
						GLint status;
						GLsizei size;
						getShaderiv(GL_COMPILE_STATUS, &status);
						if(status != GL_TRUE)
						{
								getShaderiv(GL_INFO_LOG_LENGTH, &size);
								m_log = (char*)malloc(size+ 1);
								memset(m_log, '\0', size + 1);
								getShaderInfoLog(size, &size, m_log);
								std::cerr << "Erreur lors de la compilation du shader : "<< m_log << std::endl;
								m_usable = false;
								return false;
						}
						m_usable = true;
						return true;
				}
		}
}
void bee::shader::loadFromFile(const char *filepath)
{
		//char *src = NULL; /* code source de notre shader */
		FILE *fp = NULL; /* fichier */
		long size; /* taille du fichier */
		long i;
		/* on ouvre le fichier */
		fp = fopen(filepath, "r");
		/* on verifie si l'ouverture a echoue */
		if(fp == NULL)
		{
				std::cerr << "Fichier introuvable : " << filepath << std::endl;
		}
		else
		{
				fseek(fp, 0, SEEK_END);
				size = ftell(fp);
				rewind(fp);
				m_source = (char*)malloc(size+1);
		}
		if(m_source == NULL)
		{

				std::cerr << "Impossible d'allouer la mémoire pour le fichier : " << filepath << std::endl;
				fclose(fp);
		}
		else
		{
				for(i=0; i<size; i++)
						m_source[i] = fgetc(fp);
				m_source[size] = '\0';
				fclose(fp);
		}
}
void bee::shader::deleteShader ()
{
		glDeleteShader (m_ID);
}
void bee::shader::shaderSource (GLsizei nombre, const GLchar **sources, const GLint *longueur)
{
		glShaderSource (m_ID, nombre, sources, longueur);
}

void bee::shader::compileShader ()
{
		glCompileShader (m_ID);
}

void bee::shader::getShaderiv (GLenum type, GLint* resultat)
{
		glGetShaderiv (m_ID, type, resultat);
}

void bee::shader::getShaderInfoLog (GLsizei max_size, GLsizei *longueur, char *info_log)
{
		glGetShaderInfoLog (m_ID, max_size, longueur, info_log);
}

GLuint bee::shader::getID ()
{
		return m_ID;
}

void bee::VertexShader::createShader ()
{
		m_ID = glCreateShader(GL_VERTEX_SHADER);

}
void bee::FragmentShader::createShader ()
{
		m_ID = glCreateShader(GL_FRAGMENT_SHADER);

}
bee::VertexShader::~VertexShader ()
{
		glDeleteShader(m_ID);
}
bee::FragmentShader::~FragmentShader ()
{
		glDeleteShader(m_ID);
}
