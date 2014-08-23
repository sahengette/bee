#include "camera.hpp"
bee::camera *bee::camera::CameraInMove = nullptr;
bee::camera::camera()
{
	m_pos = m_look  = glm::vec3(0);
	m_axis = glm::vec3(0,1,0);
	m_phi = m_theta = 0;
	m_viewmat = glm::lookAt(m_pos, m_look, m_axis);
	m_phi = m_theta = 0;
}

bee::camera::camera(glm::vec3 pos,glm::vec3 look,glm::vec3 axis)
{
	m_pos = pos ; m_look = look, m_axis = axis;
	m_viewmat = glm::lookAt(m_pos, m_look, m_axis);
	m_phi = m_theta = 0;
}

void bee::camera::pixelMoved(double x, double y)
{
	m_phi -= x*0.2;
	m_theta -= -y*0.2;

	if(m_phi>89)
		m_phi = 89;
	else if (m_phi <-89)
		m_phi = -89;
	float phit = cos(m_phi*M_PI/180);
	m_forward.x = phit*cos(m_theta*M_PI/180);
	m_forward.y = phit*sin(m_theta*M_PI/180);
	m_forward.z = sin(m_phi*M_PI/180);

	m_left = glm::cross(m_axis, m_forward);
	m_look = m_pos + m_forward;
	m_viewmat = glm::lookAt(m_pos, m_look, m_axis);
}
