#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "Shader.hpp"
#include "ft_vox.hpp"

class Skybox
{
	public:
	
		Skybox(void);
		~Skybox(void) {};

		void	render(Camera &camera);

	private:

		Shader			shader;
		unsigned int	texture;
		unsigned int	VAO;
		unsigned int	VBO;
};

#endif