#include "Chunk.hpp"

Chunk::Chunk(float posx, float posy, float posz, unsigned int id)
: id(id)
{
	pos = glm::vec3(posx, posy, posz);
	blocks = new Block**[CS];
	
	for(int x = 0; x < CS; x++)
	{
		blocks[x] = new Block*[CS];

		for(int z = 0; z < CS; z++)
		{
			blocks[x][z] = new Block[CH];
			for(int y = 0; y < CH; y++)
			{
				if (blocks[x][z][y].getType() == 0)
				{
					if (pos.y + y >= DEEP / 2)
						biome.getSurface(blocks, pos, x, y, z);
					else
						biome.getUnder(blocks, pos, x, y, z);
				}
			}
		}
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void	Chunk::remove(void)
{
	for (int x = 0; x < CS; x++)
		{
			for (int z = 0; z < CS; z++)
				delete [] blocks[x][z];
			delete [] blocks[x];
		}
		delete [] blocks;
}

void	Chunk::setData(void)
{
	data.clear();
	for (int x = 0; x < CS; x++)
		for (int y = 0; y < CH; y++)
			for (int z = 0; z < CS; z++)
				checkBlock(x, y, z);
	for (int x = 0; x < CS; x++)
		for (int y = 0; y < CH; y++)
			for (int z = 0; z < CS; z++)
				checkQuad(x, y, z);
	bind();
}

void	Chunk::render(unsigned int textureId)
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glDrawArrays(GL_TRIANGLES, 0, data.size() / 8);
	glBindVertexArray(0);
}

void	Chunk::bind(void)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void	Chunk::checkBlock(int x, int y, int z)
{
	if(blocks[x][z][y].isActive())
	{
		blocks[x][z][y].setLeft();
		if (x > 0 && blocks[x-1][z][y].getType() != WATER) blocks[x][z][y].setLeft(!blocks[x-1][z][y].isActive());
		blocks[x][z][y].setRight();
		if (x < CS - 1 && blocks[x+1][z][y].getType() != WATER) blocks[x][z][y].setRight(!blocks[x+1][z][y].isActive());
		blocks[x][z][y].setBottom();
		if (y > 0 && blocks[x][z][y-1].getType() != WATER) blocks[x][z][y].setBottom(!blocks[x][z][y-1].isActive());
		blocks[x][z][y].setTop();
		if (y < CH - 1 && blocks[x][z][y+1].getType() != WATER) blocks[x][z][y].setTop(!blocks[x][z][y+1].isActive());
		blocks[x][z][y].setBack();
		if (z > 0 && blocks[x][z-1][y].getType() != WATER) blocks[x][z][y].setBack(!blocks[x][z-1][y].isActive());
		blocks[x][z][y].setFront();
		if (z < CS - 1 && blocks[x][z+1][y].getType() != WATER) blocks[x][z][y].setFront(!blocks[x][z+1][y].isActive());
	}
}


void	Chunk::checkQuad(int x, int y, int z)
{
	std::vector<float> tmp;

	if(blocks[x][z][y].isActive())
	{
		if(blocks[x][z][y].getQuad(BACK) && blocks[x][z][y].getType() != WATER) 
		{
			int i = 1;
			for (i = 1; y + i < CH && blocks[x][z][y + i].getQuad(BACK) && blocks[x][z][y + i].getType() == blocks[x][z][y].getType(); i++)
				blocks[x][z][y + i].setBack(false);
			int j = 1;
			for (j = 1; x + j < CS && blocks[x + j][z][y].getQuad(BACK) && blocks[x + j][z][y].getType() == blocks[x][z][y].getType(); j++)
			{
				int k;
				for (k = 0; k < i && blocks[x + j][z][y + k].getQuad(BACK) && blocks[x + j][z][y + k].getType() == blocks[x][z][y].getType(); k++) {}
				if (k == i)
					for (k = 0; k < i; k++)
						blocks[x + j][z][y + k].setBack(false);
				else
					break ;
			}
			try { tmp = addBack(pos.x + x, pos.y + y, pos.x + x + j, pos.y + y + i, pos.z + z, blocks[x][z][y].getType());
				data.insert(data.end(), tmp.begin(), tmp.end());
			} catch (std::exception e) {return; }
		}

		if(blocks[x][z][y].getQuad(FRONT) && blocks[x][z][y].getType() != WATER)
		{
			int i = 1;
			for (i = 1; y + i < CH && blocks[x][z][y + i].getQuad(FRONT) && blocks[x][z][y + i].getType() == blocks[x][z][y].getType(); i++)
				blocks[x][z][y + i].setFront(false);
			int j = 1;
			for (j = 1; x + j < CS && blocks[x + j][z][y].getQuad(FRONT) && blocks[x + j][z][y].getType() == blocks[x][z][y].getType(); j++)
			{
				int k;
				for (k = 0; k < i && blocks[x + j][z][y + k].getQuad(FRONT) && blocks[x + j][z][y + k].getType() == blocks[x][z][y].getType(); k++) {}
				if (k == i)
					for (k = 0; k < i; k++)
						blocks[x + j][z][y + k].setFront(false);
				else
					break ;
			}
			try { tmp = addFront(pos.x + x, pos.y + y, pos.x + x + j, pos.y + y + i, pos.z + z, blocks[x][z][y].getType());
				data.insert(data.end(), tmp.begin(), tmp.end());
			} catch (std::exception e) {return; }
		}

		if(blocks[x][z][y].getQuad(LEFT) && blocks[x][z][y].getType() != WATER)
		{
			int i = 1;
			for (i = 1; y + i < CH && blocks[x][z][y + i].getQuad(LEFT) && blocks[x][z][y + i].getType() == blocks[x][z][y].getType(); i++)
				blocks[x][z][y + i].setLeft(false);
			int j = 1;
			for (j = 1; z + j < CS && blocks[x][z + j][y].getQuad(LEFT) && blocks[x][z + j][y].getType() == blocks[x][z][y].getType(); j++)
			{
				int k;
				for (k = 0; k < i && blocks[x][z + j][y + k].getQuad(LEFT) && blocks[x][z + j][y + k].getType() == blocks[x][z][y].getType(); k++) {}
				if (k == i)
					for (k = 0; k < i; k++)
						blocks[x][z + j][y + k].setLeft(false);
				else
					break ;
			}
			try { tmp = addLeft(pos.x + x, pos.y + y, pos.y + y + i, pos.z + z, pos.z + z + j, blocks[x][z][y].getType());
				data.insert(data.end(), tmp.begin(), tmp.end());
			} catch (std::exception e) {return; }
		}

		if(blocks[x][z][y].getQuad(RIGHT) && blocks[x][z][y].getType() != WATER)
		{
			int i = 1;
			for (i = 1; y + i < CH && blocks[x][z][y + i].getQuad(RIGHT) && blocks[x][z][y + i].getType() == blocks[x][z][y].getType(); i++)
				blocks[x][z][y + i].setRight(false);
			int j = 1;
			for (j = 1; z + j < CS && blocks[x][z + j][y].getQuad(RIGHT) && blocks[x][z + j][y].getType() == blocks[x][z][y].getType(); j++)
			{
				int k;
				for (k = 0; k < i && blocks[x][z + j][y + k].getQuad(RIGHT) && blocks[x][z + j][y + k].getType() == blocks[x][z][y].getType(); k++) {}
				if (k == i)
					for (k = 0; k < i; k++)
						blocks[x][z + j][y + k].setRight(false);
				else
					break ;
			}
			try { tmp = addRight(pos.x + x, pos.y + y, pos.y + y + i, pos.z + z, pos.z + z + j, blocks[x][z][y].getType());
				data.insert(data.end(), tmp.begin(), tmp.end());
			} catch (std::exception e) {return; }
		}

		if(blocks[x][z][y].getQuad(BOTTOM) && blocks[x][z][y].getType() != WATER)
		{
			int i = 1;
			for (i = 1; x + i < CS && blocks[x + i][z][y].getQuad(BOTTOM) && blocks[x + i][z][y].getType() == blocks[x][z][y].getType(); i++)
				blocks[x + i][z][y].setBottom(false);
			int j = 1;
			for (j = 1; z + j < CS && blocks[x][z + j][y].getQuad(BOTTOM) && blocks[x][z + j][y].getType() == blocks[x][z][y].getType(); j++)
			{
				int k;
				for (k = 0; k < i && blocks[x + k][z + j][y].getQuad(BOTTOM) && blocks[x + k][z + j][y].getType() == blocks[x][z][y].getType(); k++) {}
				if (k == i)
					for (k = 0; k < i; k++)
						blocks[x + k][z + j][y].setBottom(false);
				else
					break ;
			}
			try { tmp = addBottom(pos.x + x, pos.x + x + i, pos.y + y, pos.z + z, pos.z + z + j, blocks[x][z][y].getType());
				data.insert(data.end(), tmp.begin(), tmp.end());
			} catch (std::exception e) {return; }
		}

		if(blocks[x][z][y].getQuad(TOP))
		{
			int i = 1;
			for (i = 1; x + i < CS && blocks[x + i][z][y].getQuad(TOP) && blocks[x + i][z][y].getType() == blocks[x][z][y].getType(); i++)
				blocks[x + i][z][y].setTop(false);
			int j = 1;
			for (j = 1; z + j < CS && blocks[x][z + j][y].getQuad(TOP) && blocks[x][z + j][y].getType() == blocks[x][z][y].getType(); j++)
			{
				int k;
				for (k = 0; k < i && blocks[x + k][z + j][y].getQuad(TOP) && blocks[x + k][z + j][y].getType() == blocks[x][z][y].getType(); k++) {}
				if (k == i)
					for (k = 0; k < i; k++)
						blocks[x + k][z + j][y].setTop(false);
				else
					break ;
			}
			try { tmp = addTop(pos.x + x, pos.x + x + i, pos.y + y, pos.z + z, pos.z + z + j, blocks[x][z][y].getType());
				data.insert(data.end(), tmp.begin(), tmp.end());
			} catch (std::exception e) {return; }
		}
	}
	tmp.clear();
}

std::vector<float>	Chunk::addBack(float x1, float y1, float x2, float y2, float z, int t)
{
	glm::vec2 uv;
	try { uv = biome.checkType(t, glm::vec3(0.f, 0.f, -1.f)); } catch (std::exception e) {throw e; }
	std::vector<float> data = {
		x1,		y1,		z, uv.x, uv.y,		0.f, 0.f, -1.f,
		x2,		y2,		z, uv.x, uv.y,		0.f, 0.f, -1.f,
		x2,		y1,		z, uv.x, uv.y,		0.f, 0.f, -1.f,
		x2,		y2,		z, uv.x, uv.y,		0.f, 0.f, -1.f,
		x1,		y1,		z, uv.x, uv.y,		0.f, 0.f, -1.f,
		x1,		y2,		z, uv.x, uv.y,		0.f, 0.f, -1.f
	};
	return data;
}


std::vector<float>	Chunk::addFront(float x1, float y1, float x2, float y2, float z, int t)
{
	glm::vec2 uv;
	try { uv = biome.checkType(t, glm::vec3(0.f, 0.f, 1.f)); } catch (std::exception e) {throw e; }
	std::vector<float> data = {
		x1,		y1,		z + 1, uv.x, uv.y,	0.f, 0.f, 1.f,
		x2,		y1,		z + 1, uv.x, uv.y,	0.f, 0.f, 1.f,
		x2,		y2,		z + 1, uv.x, uv.y,	0.f, 0.f, 1.f,
		x2,		y2,		z + 1, uv.x, uv.y,	0.f, 0.f, 1.f,
		x1,		y2,		z + 1, uv.x, uv.y,	0.f, 0.f, 1.f,
		x1,		y1,		z + 1, uv.x, uv.y,	0.f, 0.f, 1.f
	};
	return data;
}


std::vector<float>	Chunk::addLeft(float x, float y1, float y2, float z1, float z2, int t)
{
	glm::vec2 uv;
	try { uv = biome.checkType(t, glm::vec3(-1.f, 0.f, 0.f)); } catch (std::exception e) {throw e; }

	std::vector<float> data = {
		x,		y2,		z2, uv.x, uv.y,		-1.f, 0.f, 0.f,
		x,		y2,		z1, uv.x, uv.y,		-1.f, 0.f, 0.f,
		x,		y1,		z1, uv.x, uv.y,		-1.f, 0.f, 0.f,
		x,		y1,		z1, uv.x, uv.y,		-1.f, 0.f, 0.f,
		x,		y1,		z2, uv.x, uv.y,		-1.f, 0.f, 0.f,
		x,		y2,		z2, uv.x, uv.y,		-1.f, 0.f, 0.f
	};
	return data;
}


std::vector<float>	Chunk::addRight(float x, float y1, float y2, float z1, float z2, int t)
{
	glm::vec2 uv;
	try { uv = biome.checkType(t, glm::vec3(1.f, 0.f, 0.f)); } catch (std::exception e) {throw e; }
	std::vector<float> data = {
		x + 1,	y2,		z2, uv.x, uv.y,		1.f, 0.f, 0.f,
		x + 1,	y1,		z1, uv.x, uv.y,		1.f, 0.f, 0.f,
		x + 1,	y2,		z1, uv.x, uv.y,		1.f, 0.f, 0.f,
		x + 1,	y1,		z1, uv.x, uv.y,		1.f, 0.f, 0.f,
		x + 1,	y2,		z2, uv.x, uv.y,		1.f, 0.f, 0.f,
		x + 1,	y1,		z2, uv.x, uv.y,		1.f, 0.f, 0.f
	};
	return data;
}

std::vector<float>	Chunk::addBottom(float x1, float x2, float y, float z1, float z2, int t)
{
	glm::vec2 uv;
	try { uv = biome.checkType(t, glm::vec3(0.f, -1.f, 0.f)); } catch (std::exception e) {throw e; }
	std::vector<float> data = {
		x1,		y,		z1, uv.x, uv.y, 	0.f, -1.f, 0.f,
		x2,		y,		z1, uv.x, uv.y, 	0.f, -1.f, 0.f,
		x2,		y,		z2, uv.x, uv.y, 	0.f, -1.f, 0.f,
		x2,		y,		z2, uv.x, uv.y, 	0.f, -1.f, 0.f,
		x1,		y,		z2, uv.x, uv.y, 	0.f, -1.f, 0.f,
		x1,		y,		z1, uv.x, uv.y, 	0.f, -1.f, 0.f
	};
	return data;
}

std::vector<float>	Chunk::addTop(float x1, float x2, float y, float z1, float z2, int t)
{
	glm::vec2 uv;
	y -= t == WATER ? 0.2f : 0.f;
	try { uv = biome.checkType(t, glm::vec3(0.f, 1.f, 0.f)); } catch (std::exception e) {throw e; }
	std::vector<float> data = {
		x1,		y + 1,	z1, uv.x, uv.y,		0.f, 1.f, 0.f,
		x2,		y + 1,	z2, uv.x, uv.y,		0.f, 1.f, 0.f,
		x2,		y + 1,	z1, uv.x, uv.y,		0.f, 1.f, 0.f,
		x2,		y + 1,	z2, uv.x, uv.y,		0.f, 1.f, 0.f,
		x1,		y + 1,	z1, uv.x, uv.y,		0.f, 1.f, 0.f,
		x1,		y + 1,	z2, uv.x, uv.y,		0.f, 1.f, 0.f
	};
	return data;
}

const char* Chunk::NoBlockAtPosException::what() const throw() 	{ return ("No Block At Pos."); }

