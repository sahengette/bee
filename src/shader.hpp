#ifndef beeshader
#define beeshader


#include "stdafx.hpp"



namespace bee
{

		class shader
		{
				public :
						
						void loadFromFile(const char *filepath);
						bool compile();
						GLuint getID();
						shader();

						virtual ~shader();

				protected :
						bool m_usable;
						GLuint m_ID;
						
						char *m_source;
						char *m_name;
						char *m_log;
						virtual void createShader () =0;
						
						void deleteShader();
						void shaderSource(GLsizei nombre, const GLchar **sources, const GLint *longueur);
						void compileShader();
						void getShaderiv(GLenum type, GLint* resultat) ; 
						void getShaderInfoLog(GLsizei max_size, GLsizei *longueur, char *info_log);
		};
	
		
		class VertexShader : public shader
		{
				public :
						VertexShader();
						void createShader();
						~VertexShader();
		};
		
		class FragmentShader : public shader
		{
				public :
						FragmentShader();
						void createShader();
						~FragmentShader();

		};
		class Program
		{
			public :
				Program(VertexShader* vtx, FragmentShader* frg);
				bool linkProgram();
				void useProgram();
				void unuseProgram();
				int getID(){return m_program;};


			
			private :
				static Program* m_globalprograminuse;
				bool m_usable;
				GLuint m_program;
				VertexShader* m_vtx;
				FragmentShader* m_frg;
				char* m_log;

		};
}


#endif
