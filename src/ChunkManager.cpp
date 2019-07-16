#include "ChunkManager.hpp"

const int maxS = SIZE / CS;
const int maxH = DEEP / CH;

ChunkManager::ChunkManager(glm::vec3 head)
: id(0), cpf(1), shader(Shader("src/shader/model.vs", "src/shader/model.fs"))
{
	grid = new glm::vec4**[maxS];
	for(int x = 0; x < maxS; x++)
	{
        grid[x] = new glm::vec4*[maxH];

		for(int y = 0; y < maxH; y++)
		{
            grid[x][y] = new glm::vec4[maxS];

			for(int z = 0; z < maxS; z++)
			{
        	   grid[x][y][z] = glm::vec4((float)x , (float)y, (float)z, ++id);
			}
		}
	}
	loadingList.emplace_back(Chunk(head.x * CS, head.y * CH, head.z * CS, grid[(int)head.x][(int)head.y][(int)head.z].w));
	grid[(int)head.x][(int)head.y][(int)head.z].w = 0.f;

	texture = shader.loadTexture();
}

void	ChunkManager::update(Camera &camera)
{
	renderDistance = camera.getPos().y > SEA + 1 ? DIST : DIST / 4;
	camera.setFarPlane(renderDistance); 
	if (lastPos == camera.getPos())
	{
		double time = glfwGetTime();
		updateUnloading(camera);
	}
	updateLoading(getPosOnGrid(camera));
	updateSetupList(camera);
	updateRenderList(camera);
	lastPos = camera.getPos();
}

glm::vec3	ChunkManager::getPosOnGrid(Camera &camera)
{
	int x, y, z;

	x = ((int)camera.getPos().x - (int)camera.getPos().x % CS) / CS;
	y = ((int)camera.getPos().y - (int)camera.getPos().y % CH) / CH;
	z = ((int)camera.getPos().z - (int)camera.getPos().z % CS) / CS;
	return glm::vec3(x, y, z);
}

Block	ChunkManager::getBlockOnGrid(Camera &camera)
{
	if (camera.getPos().x > SIZE ||
		camera.getPos().x < 0 ||
		camera.getPos().y > DEEP ||
		camera.getPos().y < 0 ||
		camera.getPos().z > SIZE ||
		camera.getPos().z < 0)
		 throw (NoBlockAtPosException());
	int ox = (int)camera.getPos().x % CS,
		oy = (int)camera.getPos().y % CH,
		oz = (int)camera.getPos().z % CS;

	int x = ((int)camera.getPos().x - ox) / CS,
		y = ((int)camera.getPos().y - oy) / CH,
		z = ((int)camera.getPos().z - oz) / CS;

	try {

		for (int i = 0; i < setupList.size(); i++)
		{
			if (setupList[i].getPos().x / CS == x &&
				setupList[i].getPos().y / CH == y &&
				setupList[i].getPos().z / CS == z)
				return setupList[i].getBlock(ox, oy, oz);
		}
	} catch(std::exception & e) {throw(e); }
	throw (NoBlockAtPosException());
}

void	ChunkManager::updateLoading(glm::vec3 ptr)
{
	int chunkLoaded = 0;
	int gridS = renderDistance / CS;
	int gridH = SEA / CH;
	int dir = 1;
	int c = 0;
	int i = 0;
	
	while (i < gridS * 2)
	{
		bool alt = false; 
		for (int j = 0; j < 2 ; j++)
		{
			for (i = 0 ; i <= c ; i++)
			{
				if (alt)
					ptr.x += dir;
				else
					ptr.z += dir;
				for (int y = -gridH; y < gridH; y++)
				{
					int tmpx = ptr.x, tmpy = ptr.y + y, tmpz = ptr.z;
					if (!(tmpx >= 0 && tmpx < maxS &&
						tmpy >= 0 && tmpy < maxH &&
						tmpz >= 0 && tmpz < maxS))
						continue ;
					if (grid[tmpx][tmpy][tmpz].w != 0.f)
					{
							loadingList.emplace_back(Chunk(tmpx * CS, tmpy * CH, tmpz * CS, grid[tmpx][tmpy][tmpz].w));
							grid[tmpx][tmpy][tmpz].w = 0.f;
							if (++chunkLoaded >= cpf)
								return ;
					}
				}
			}
			alt = !alt;
		}
		dir *= -1;
		c++;

	}
}

void	ChunkManager::updateUnloading(Camera &camera)
{
	int chunkUnloaded = 0;
	for (int i = 0; i < setupList.size(); i++)
	{
		if (std::abs(glm::distance(camera.getPos(), setupList[i].getPos())) >= DIST * 1.5f)
		{
			grid[(int)setupList[i].getPos().x / CS][(int)setupList[i].getPos().y / CH][(int)setupList[i].getPos().z / CS].w = ++id;
			setupList[i].remove();
			iter_swap(setupList.begin() + i, setupList.end() -1);
			setupList.pop_back();
			if (++chunkUnloaded > cpf)
				return ;
		}
	}
}

void	ChunkManager::updateSetupList(Camera &camera)
{
	for (int i = 0; i < loadingList.size(); i++)
	{
			loadingList[i].setData();
			setupList.emplace_back(loadingList[i]);

	}
	loadingList.clear();
}


void	ChunkManager::updateRenderList(Camera &camera)
{
	renderList.clear();
	for (int i = 0; i < setupList.size(); i++)
	{
		if (camera.chunkInFrustum(setupList[i].getPos()))
		{
			if (!(camera.getPos().y > SEA + 1 && setupList[i].getPos().y < SEA))
			{
				if (std::abs(glm::distance(camera.getPos().x, setupList[i].getPos().x)) < renderDistance
					&& std::abs(glm::distance(camera.getPos().z, setupList[i].getPos().z)) < renderDistance)
				{
					camera.cursorize(&setupList[i]);
					renderList.emplace_back(setupList[i]);
				}
			}
		}
	}
}

void	ChunkManager::render(Camera &camera)
{
	shader.use();
	shader.displayModel(camera);
	for (int i = 0; i < renderList.size(); i++)
			renderList[i].render(texture);
}

const char* ChunkManager::NoBlockAtPosException::what() const throw() 	{ return ("No Block At Pos."); }