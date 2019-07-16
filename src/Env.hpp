#ifndef ENV_HPP
#define ENV_HPP

#include "ft_vox.hpp"

class Env
{
	public:

		Env(void);
		~Env(void);

		GLFWwindow*		getGLFWwin(void);
		float const		&getTime(void) const	{ return time; }
		void			setTime(float t) { time = t; }

	private:

		GLFWwindow*		window;
		int 			width;
		int 			height;
		
		float			time;
		
		void			initGlfwEnvironment(void);
		void			initGlfwWindow(void);
		void			initGladEnvironment(void);

		class 			GlfwException: public std::exception { virtual const char* what() const throw(); };
		class 			WindowException: public std::exception { virtual const char* what() const throw(); };
		class 			GladException: public std::exception { virtual const char* what() const throw(); };

};

void			framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif