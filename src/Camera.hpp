#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Chunk.hpp"
#include "ft_vox.hpp"

class Camera
{
	public:

		Camera(	glm::vec3 pos = glm::vec3(0.0f, 0.0f,  5.0f),
				glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3 up = glm::vec3(0.0f, 1.0f,  0.0f));
		~Camera(void) {};

		bool			showDebug = true;
		glm::vec3		getPos(void) 						{ return cameraPos; }
		glm::vec3		getCursor(void) 					{ return cursor; }
		int				getSpeed(void) 						{ return speed; }


		glm::vec3		getPosOnGrid(void);

		glm::vec3		getFront(void) 						{ return cameraFront; }
		glm::vec3		getUp(void)							{ return cameraUp; }

		glm::mat4		getProjection(void)					{ return projection; }
		glm::mat4		getView(void)						{ return view; }

		void			setFarPlane(float dist)				{ far = dist; create_projection(); }
		void			setCameraPos(glm::vec3 target) 		{ cameraPos = target; }
		void			setCameraFront(glm::vec3 target)	{ cameraFront = target; }
		void			setCameraUp(glm::vec3 target)		{ cameraUp = target; }
		void			setCursor(void);
		void			cursorize(Chunk* chunk);

		void			create_projection(void);

		void			update(GLFWwindow *window, float deltaTime, std::vector<Chunk> &list);
		void			updateFrustum(void);
		void			setFrustumPlane(void);

		void			inputKey(GLFWwindow *window, float deltaTime, std::vector<Chunk> &list);
		void			inputMouse(GLFWwindow *window);
		
		std::string		cameraCompass(void);
		void			printCameraPos(void);

		bool			isBlockOnPos(glm::vec3 p, std::vector<Chunk> &list);
		bool 			pointInFrustum(glm::vec3 &p);
		bool 			chunkInFrustum(glm::vec3 chunk);
		float			distanceFromPlane(glm::vec3 &p, int i);

		glm::vec3		getVertexMin(glm::vec3 &p, glm::vec3 &n);
		glm::vec3		getVertexMax(glm::vec3 &p, glm::vec3 &n);

	private:

		glm::vec3	cameraPos;
		glm::vec3	cursor;
		glm::vec3	cameraFront;
		glm::vec3	cameraUp;
		glm::vec3	cameraRight;

		glm::mat4	projection;
		glm::mat4	view;
		glm::mat4	vP;

		int			speed;
		bool		remove;
		bool		add;
		int			inventory;
		glm::vec3	fPlaneNormal[6];
		float		fPlaneOffset[6];

		float		fov, ratio, near, far;
		float 		nh, nw, fh, fw;


		float 		yaw;
		float 		pitch;
		float 		lastX;
		float 		lastY;

		glm::vec3 	nc, fc, ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

		enum { TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR };
};

#endif
