#include "Env.hpp"


Env::Env(void)
:time(glfwGetTime())
{
	try {
		initGlfwEnvironment();
		initGlfwWindow();
		initGladEnvironment();
	} catch (const std::exception& error) {
		std::cout << error.what() << std::endl;
	}

	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	glEnable(GL_MULTISAMPLE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Env::~Env(void) {}

void		Env::initGlfwEnvironment(void)
{
	if (!glfwInit())
		throw GlfwException();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
}

void		Env::initGlfwWindow(void)
{
	if (!(window = glfwCreateWindow(W, H, "ft_vox", NULL, NULL)))
		throw WindowException();
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

}

void		Env::initGladEnvironment(void)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw GladException();

}

GLFWwindow*	Env::getGLFWwin(void)
{
	return (window);
}

void		framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }


const char* Env::GlfwException::what() const throw() { return ("glfw initialization failed"); }
const char* Env::WindowException::what() const throw() { return ("window initialization failed"); }
const char* Env::GladException::what() const throw() { return ("glad initialization failed"); }
