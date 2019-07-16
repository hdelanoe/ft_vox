#include "ft_vox.hpp"
#include "Env.hpp"
#include "ChunkManager.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Skybox.hpp" 

const double maxFPS = 60.0;
const double maxPeriod = 1.0 / maxFPS;

int main(int argc, char *argv[])
{
	std::string mode = argc > 1 ? argv[1] : "normal";
    Env env;
    
    glm::vec3 player = glm::vec3(SIZE / 2, DEEP - 32, SIZE / 2);
    
    Camera camera(player);
    ChunkManager manager(camera.getPosOnGrid());

	Skybox skybox;
	Shader textShader("src/shader/textRender.vert", "src/shader/textRender.frag");
	textShader.initFreetype(textShader);


	float		clr[3] = {0.8f, 0.8, 0.8};


	while (!glfwWindowShouldClose(env.getGLFWwin()))
	{
		glm::vec4 color = camera.getPos().y > SEA + 1? glm::vec4(0.616f, 0.729f, 1.0f, 1.0f) : glm::vec4(0.1, 0.3, 0.4, 1.0f);
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


		float time = glfwGetTime();
		float deltaTime = time - env.getTime();
		camera.update(env.getGLFWwin(), deltaTime, manager.getList());
		
		if (deltaTime < 16.66f)
		{
    		manager.update(camera);
		}
		manager.render(camera);
		
  		glDepthFunc(GL_LEQUAL);
		if (camera.getPos().y > SEA + 1)
			skybox.render(camera);
		glDepthFunc(GL_LESS);

		if (camera.showDebug)
			textShader.Debbug_screen(camera, textShader, 1 / deltaTime, manager);	

		glfwSwapBuffers(env.getGLFWwin());
		glfwPollEvents();
		env.setTime(time);
	}

	glfwTerminate();
	return 0;
}