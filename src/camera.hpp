#include "stdafx.hpp"
namespace bee
{
	class camera
	{
		public :
		static camera *CameraInMove; 
		void pixelMoved(double x , double y);
		camera();
		camera(glm::vec3 pos, glm::vec3 look, glm::vec3 axis);
		glm::mat4 getViewMat(){return m_viewmat;};
		void go(){m_pos = m_pos + (m_forward * glm::vec3(0.08)); m_viewmat = glm::lookAt(m_pos, m_look, m_axis);};
		void back(){m_pos = m_pos - (m_left * glm::vec3(0.08)); m_viewmat = glm::lookAt(m_pos, m_look, m_axis);};

		private :
		glm::vec3 m_pos, m_look, m_axis, m_forward, m_left;
		double m_phi, m_theta;
		glm::mat4 m_viewmat;
	};
}
