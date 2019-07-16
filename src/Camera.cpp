#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
: yaw(-90.f), pitch(0.f), lastX(W / 2.f), lastY(H / 2.f),
fov(glm::radians(80.0f)), ratio((float)W / (float)H), near(0.1f), far((float)DIST), inventory(0)
{
	cameraPos   = pos;
	cameraFront = glm::normalize(front);
	cameraUp    = glm::normalize(up);
	
	create_projection();
}

void			Camera::create_projection(void)
{
	float tang = (float)tan(fov * 0.5);
	nh = near * tang;
	nw = nh * ratio;
	fh = far  * tang;
	fw = fh * ratio;
	projection = glm::perspective(fov, ratio, near, far);
}


void			Camera::update(GLFWwindow* window, float deltaTime, std::vector<Chunk> &list)
{
	inputKey(window, deltaTime, list);
	inputMouse(window);

	view		= glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	vP			= projection * view;

	updateFrustum();
}

glm::vec3		Camera::getPosOnGrid(void)
{
	int maxS = SIZE / CS;
	int maxH = DEEP / CH;

	int x, y, z;

	x = ((int)cameraPos.x - (int)cameraPos.x % CS) / CS;
	y = ((int)cameraPos.y - (int)cameraPos.y % CH) / CH;
	z = ((int)cameraPos.z - (int)cameraPos.z % CS) / CS;
	return glm::vec3(x, y, z);
}

void			Camera::updateFrustum()
{
		glm::vec3 X, Y;

		glm::mat3 transform = vP;
		nc = cameraPos + cameraFront * near;
		fc = cameraPos + cameraFront * far;
		
		Y = glm::normalize(glm::row(transform, 1));
		X = glm::normalize(glm::row(transform, 0));
		
		ntl = nc + Y * nh - X * nw;
		ntr = nc + Y * nh + X * nw;
		nbl = nc - Y * nh - X * nw;
		nbr = nc - Y * nh + X * nw;

		ftl = fc + Y * fh - X * fw;
		ftr = fc + Y * fh + X * fw;
		fbl = fc - Y * fh - X * fw;
		fbr = fc - Y * fh + X * fw;

		setFrustumPlane();
}

void			Camera::setFrustumPlane(void)
{
	glm::vec3	p0, p1, p2;

	p0 = nbl; p1 = fbl; p2 = ftl;
	fPlaneNormal[LEFT] = glm::normalize(glm::cross(p1 - p0, p2 - p1));
	fPlaneOffset[LEFT] = glm::dot(fPlaneNormal[LEFT], p0);
	
	p0 = ntl; p1 = ftl; p2 = ftr;
	fPlaneNormal[TOP] = glm::normalize(glm::cross(p1 - p0, p2 - p1));
	fPlaneOffset[TOP] = glm::dot(fPlaneNormal[TOP], p0);
	
	p0 = ntr; p1 = ftr; p2 = fbr;
	fPlaneNormal[RIGHT] = glm::normalize(glm::cross(p1 - p0, p2 - p1));
	fPlaneOffset[RIGHT] = glm::dot(fPlaneNormal[RIGHT], p0);
	
	p0 = nbr; p1 = fbr; p2 = fbl;
	fPlaneNormal[BOTTOM] = glm::normalize(glm::cross(p1 - p0, p2 - p1));
	fPlaneOffset[BOTTOM] = glm::dot(fPlaneNormal[BOTTOM], p0);

	fPlaneNormal[NEAR] = cameraFront;
	fPlaneOffset[NEAR] = glm::dot(nc, cameraFront);
	
	fPlaneNormal[FAR] = -cameraFront;
	fPlaneOffset[FAR] = glm::dot(fc, -cameraFront);
}

std::string		Camera::cameraCompass(void)
{
	if ((getFront().x < 0.0f || getFront().x > 0.0f) && getFront().z > 0.5f)
		return ("NORTH");
	else if ((getFront().x < 0.0f || getFront().x > 0.0f) && getFront().z < -0.5f)
		return ("SOUTH");
	else if (getFront().x > 0.0f && (getFront().z > -0.5f && getFront().z < 0.5f))
		return ("WEST");
	else if (getFront().x < 0.0f && (getFront().z > -0.5f && getFront().z < 0.5f))
		return ("EAST");
	return ("404");
}

void			Camera::inputKey(GLFWwindow *window, float deltaTime, std::vector<Chunk> &list)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	glm::vec3 tmp = cameraPos;
	float cameraSpeed = speed * deltaTime; 
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		tmp = tmp + cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		tmp = tmp - cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		tmp = tmp - glm::cross(cameraFront, cameraUp * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		tmp = tmp + glm::cross(cameraFront, cameraUp * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		showDebug = true;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		showDebug = false;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 50;
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		speed = 20;

	else
		speed = 5;
	if (showDebug || !isBlockOnPos(tmp, list))
		cameraPos = tmp;
}

void			Camera::inputMouse(GLFWwindow *window)
{
	double mxpos, mypos;
	glfwGetCursorPos(window, &mxpos, &mypos);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		remove = true;
	else
		remove = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		add = true;
	else
		add = false;
	float xoffset = mxpos - lastX;
	float yoffset = lastY - mypos;
	lastX = mxpos;
	lastY = mypos;

	float sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	pitch = std::min(std::max(pitch, -89.0f), 89.0f);

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	setCursor();
}

void			Camera::setCursor(void)
{
	cursor = glm::vec3(cameraPos.x + cameraFront.x * 5, cameraPos.y + cameraFront.y * 5, cameraPos.z + cameraFront.z * 5);
}

void			Camera::cursorize(Chunk* chunk)
{
	if (chunk->getPos().x <= cursor.x && chunk->getPos().x + CS >= cursor.x
		&& chunk->getPos().y <= cursor.y && chunk->getPos().y + CH >= cursor.y
		&& chunk->getPos().z <= cursor.z && chunk->getPos().z + CS >= cursor.z)
	{
		for (int i = 0; i < CS; i++)
		{
			for (int j = 0; j < CH; j++)
			{
				for (int k = 0; k < CS; k++)
				{
					if (remove && chunk->getBlock(i, j, k).isActive()
						&& round(chunk->getPos().x + i) == round(cursor.x)
						&& round(chunk->getPos().y + j) == round(cursor.y)
						&& round(chunk->getPos().z + k) == round(cursor.z)
						&& chunk->getBlock(i, j, k).getType() != WATER)
					{
						inventory = chunk->getBlock(i, j, k).getType();
						chunk->getBlock(i, j, k).setActive(false);
						chunk->setData();

					}
					else if (add && !chunk->getBlock(i, j, k).isActive()
						&& round(chunk->getPos().x + i) == round(cursor.x)
						&& round(chunk->getPos().y + j) == round(cursor.y)
						&& round(chunk->getPos().z + k) == round(cursor.z))
					{
						chunk->getBlock(i, j, k).setActive();
						chunk->getBlock(i, j, k).setType(inventory);
						chunk->setData();
					}
				}
			}
		}
	}
}

bool	Camera::isBlockOnPos(glm::vec3 p, std::vector<Chunk> &list)
{
	int maxS = SIZE / CS;
	int maxH = DEEP / CH;

	int ox = (int)p.x % CS,
		oy = (int)p.y % CH,
		oz = (int)p.z % CS;

	int x = ((int)p.x - ox) / CS,
		y = ((int)p.y - oy) / CH,
		z = ((int)p.z - oz) / CS;

	try {

		for (int i = 0; i < list.size(); i++)
		{
			if (list[i].getBlock(ox, oy, oz).isActive() && list[i].getBlock(ox, oy, oz).getType() != WATER &&
				list[i].getPos().x / CS == x &&
				list[i].getPos().y / CH == y &&
				list[i].getPos().z / CS == z)
				return true;
		}
	} catch (std::exception e) {return false; }	
	return false;
}

void				Camera::printCameraPos(void)
{
	std::cout << "x = " << (int)cameraPos.x;
	std::cout << " || y = " << (int)cameraPos.y;
	std::cout << " || z = " << (int)cameraPos.z << std::endl;
}

bool 				Camera::pointInFrustum(glm::vec3 &p)
{
	for(int i = 0; i < 6; i++)
		if (distanceFromPlane(p, i) < 0)
			return(false);
	return(true);
}

bool 				Camera::chunkInFrustum(glm::vec3 chunk)
{
	chunk.x += CS / 2;
	chunk.y += CH / 2;
	chunk.z += CS / 2;
	for(int i = 0; i < 6; i++)
	{
		glm::vec3 vmax = getVertexMax(chunk, fPlaneNormal[i]);
		glm::vec3 vmin = getVertexMin(chunk, fPlaneNormal[i]);
		if (distanceFromPlane(vmax, i) < 0)
			return (false);
		else if (distanceFromPlane(vmin, i) < 0)
			continue;
	}
	return(true);

}

glm::vec3			Camera::getVertexMin(glm::vec3 &p, glm::vec3 &n)
{
	return glm::vec3(
		(n.x >= 0 ? p.x - CS / 2 : p.x + CS / 2),
        (n.y >= 0 ? p.y - CH / 2 : p.y + CH / 2),
        (n.z >= 0 ? p.z - CS / 2 : p.z + CS / 2)
	);
}

glm::vec3			Camera::getVertexMax(glm::vec3 &p, glm::vec3 &n)
{
	return glm::vec3(
		(n.x >= 0 ? p.x + CS / 2 : p.x - CS / 2),
        (n.y >= 0 ? p.y + CH / 2 : p.y - CH / 2),
        (n.z >= 0 ? p.z + CS / 2 : p.z - CS / 2)
	);
}

float				Camera::distanceFromPlane(glm::vec3 &p, int i)
{
	return glm::dot(fPlaneNormal[i], p) - fPlaneOffset[i];
}
