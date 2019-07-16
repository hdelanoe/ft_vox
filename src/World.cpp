#include "World.hpp"

const glm::vec3 NTOP = glm::vec3(0, 1, 0);
const glm::vec3 NBOTTOM = glm::vec3(0, -1, 0);
const glm::vec3 NLEFT = glm::vec3(-1, 0, 0);
const glm::vec3 NRIGHT = glm::vec3(1, 0, 0);
const glm::vec3 NFRONT = glm::vec3(0, 0, 1);
const glm::vec3 NBACK = glm::vec3(0, 0, -1);

World::World(void)
{
	bNoise.SetNoiseType(FastNoise::SimplexFractal);
	bNoise.SetFrequency(0.001);
	bNoise.SetSeed(42);
	e1Noise.SetNoiseType(FastNoise::SimplexFractal);
	e1Noise.SetFrequency(0.004);
	e1Noise.SetSeed(32);
	e2Noise.SetNoiseType(FastNoise::SimplexFractal);
	e2Noise.SetSeed(31);
	e2Noise.SetFrequency(0.003);
	e3Noise.SetNoiseType(FastNoise::SimplexFractal);
	e3Noise.SetSeed(33);
	e3Noise.SetFrequency(0.002);
	mNoise.SetNoiseType(FastNoise::SimplexFractal);
	mNoise.SetSeed(25);
	mNoise.SetFrequency(0.075);
	cNoise.SetNoiseType(FastNoise::PerlinFractal);
	cNoise.SetFrequency(0.01);
	tNoise.SetNoiseType(FastNoise::Simplex);
	tNoise.SetSeed(7);
	tNoise.SetFrequency(0.1);
	nNoise.SetNoiseType(FastNoise::SimplexFractal);
	nNoise.SetSeed(5);
	nNoise.SetFrequency(0.01);


}

double	World::ridgenoise(double nx, double ny) {
	double e0 = e1Noise.GetNoise(nx, ny);
	double e1 = 0.5 * e1Noise.GetNoise(2 * nx, 2 * ny) * e0;
	double e2 = 0.25 * e1Noise.GetNoise(4 * nx, 4 * ny) * (e0 + e1);
	double e = e0 + e1 + e2;
	return pow(e, 2.0f);
}

double	World::noise(double nx, double ny) {
	double e0 = e2Noise.GetNoise(nx, ny);
	double e1 = 0.6 * e2Noise.GetNoise(2 * nx, 2 * ny) * e0;
	double e2 = 0.33 * e2Noise.GetNoise(4 * nx, 4 * ny) * (e0 + e1);
	double e = e0 + e1 + e2;
	return pow(e, 2.0f);
}

double	World::bignoise(double nx, double ny) {
	double e0 = e3Noise.GetNoise(nx, ny);
	double e1 = 0.75 * e3Noise.GetNoise(2 * nx, 2 * ny) * e0;
	double e2 = 0.25 * e3Noise.GetNoise(4 * nx, 4 * ny) * (e0 + e1);
	double e = e0 + e1 + e2;
	return pow(e, 2.0f);
}

double	World::wormNoise(double nx, double ny, double nz) {
	double w0 = cNoise.GetCellular(nx, ny, nz);
	return w0;
}

double	World::moistureNoise(double nx, double ny) {
	double m0 = mNoise.GetNoise(nx, ny);
	double m1 = 0.75 * mNoise.GetNoise(2 * nx, 2 * ny) * m0;
	double m2 = 0.33 * mNoise.GetNoise(4 * nx, 4 * ny) * (m0 + m1);
	double m = m0 + m1 + m2;
	return pow(m, 2.0f);
}


void	World::getSurface(Block*** chunk, glm::vec3 pos, int x, int y, int z)
{
	int nx = x + pos.x, ny = y + pos.y, nz = z + pos.z;
	float b = bNoise.GetNoise(nx, nz) / 2 + 0.5f;
	float m = mNoise.GetNoise(nx, nz) / 2 + 0.5f;
	float e1 = ridgenoise(nx, nz);
	float e2 = noise(nx, nz);
	float e3 = bignoise(nx, nz) + 0.2f;
	float e;
	if (e2 < e1 < 0.005f)
		e = e2;
	else if (e3 - e1 < 0.005f)
		e = e3;
	else
		e = e1;
	float t = (tNoise.GetNoise(nx, nz) / 2 + 0.5f);
	float n = nNoise.GetNoise(nx,nz) / 2 + 0.5f;

	int h = e * float(SEA);
	int h2 = e * float(5);

	if (h < ny - SEA)
			chunk[x][z][y].setActive(false);
	if (e < 0.0075)
		chunk[x][z][y].setType(WATER);
	else if (e < 0.03) chunk[x][z][y].setType(SAND);
	else if (e < 0.06f)
	{
		if (m < 0.5) chunk[x][z][y].setType(SAND);
		else chunk[x][z][y].setType(DIRT);

	}
	else if (e < 0.2f)
	{
		if (m < 0.2) chunk[x][z][y].setType(COBBLE);
		else if (m < 0.6) chunk[x][z][y].setType(DIRT);
		else chunk[x][z][y].setType(GRASS);
	}
	else if (e < 0.4f)
	{
		if (m < 0.2) chunk[x][z][y].setType(COBBLE);
		else if (m < 0.4) chunk[x][z][y].setType(DIRT);
		else chunk[x][z][y].setType(GRASS);
	}
	else if (e < 0.6f)
	{
		if (m < 0.2) chunk[x][z][y].setType(COBBLE);
		else if (m < 0.3) chunk[x][z][y].setType(DIRT);
		else if (m < 0.6) chunk[x][z][y].setType(GRASS);
		else chunk[x][z][y].setType(SNOW);

	}
	else if (e < 0.8f)
	{
		if (m < 0.2) chunk[x][z][y].setType(COBBLE);
		else if (m < 0.3) chunk[x][z][y].setType(DIRT);
		else chunk[x][z][y].setType(SNOW);
	}
	else chunk[x][z][y].setType(ICE);

	if (e < 0.3f && t > 0.95f && chunk[x][z][y].isActive() && chunk[x][z][y].getType() != WATER)
		plantTree(chunk, x, y + 1, z);
	if (n < 0.25 && !chunk[x][z][y].isActive() && ny == 202)
	{
		chunk[x][z][y].setActive();
		chunk[x][z][y].setType(BLANK);	
	}
	
}

void	World::getUnder(Block*** chunk, glm::vec3 pos, int x, int y, int z)
{
	int nx = x + pos.x, ny = y + pos.y, nz = z + pos.z;
	float c = wormNoise(nx * 10, ny * 10, nz * 10);
	float e1 = ridgenoise(nx, nz);
	float e2 = noise(nx, nz);
	float e3 = bignoise(nx, nz) + 0.2f;
	float e;
	if (e2 < e1 < 0.005f)
		e = e2;
	else if (e3 - e1 < 0.005f)
		e = e3;
	else
		e = e1;

	if (ny == 0)
		chunk[x][z][y].setType(BEDROCK);
	else
		chunk[x][z][y].setType(STONE);
	if ((c < 0 && ny != 0) || (e < 0.01 && ny > 120))
		chunk[x][z][y].setActive(false);
	if (ny > 126 && e > 0.01)
		chunk[x][z][y].setActive();

}

void	World::plantTree(Block*** chunk, int x, int y, int z)
{
	srand((unsigned int)time(0));
	int length = rand()% 5 + 3;

	if (length >= CH)
		return;
	length = y + length >= CH ? 0 : length;

	for (int i = 0; i < length; i++)
		chunk[x][z][y + i].setType(TREE);
}

glm::vec2		World::checkType(int blockType, glm::vec3 normal)
{
	switch (blockType)
	{
		case GRASS:
		if (normal == NTOP)
			return (glm::vec2(0 * 0.0625f, 0 * 0.0625f));
		else if (normal == NBOTTOM)
			return (glm::vec2(2 * 0.0625f, 0 * 0.0625f));
		else if (normal == NLEFT || normal == NRIGHT || normal == NFRONT || normal == NBACK)
			return (glm::vec2(3 * 0.0625f, 0 * 0.0625f));

		case DIRT:
		return (glm::vec2(2 * 0.0625f, 0 * 0.0625f));
		
		case STONE:
		return (glm::vec2(1 * 0.0625f, 0 * 0.0625f));

		case SAND: 
		return (glm::vec2(2 * 0.0625f, 1 * 0.0625f));
		
		case WATER: 
		return (glm::vec2(15 * 0.0625f, 13 * 0.0625f));
		
		case COBBLE: 
		return (glm::vec2(0 * 0.0625f, 1 * 0.0625f));
		
		case GRAVEL: 
		return (glm::vec2(1 * 0.0625f, 3 * 0.0625f));
		
		case DIAMOND: 
		return (glm::vec2(2 * 0.0625f, 3 * 0.0625f));
		
		case COBBLESAND: 
		return (glm::vec2(0 * 0.0625f, 12 * 0.0625f));
		
		case COBBLEGRASS: 
		return (glm::vec2(4 * 0.0625f, 2 * 0.0625f));
		
		case DESERT: 
		return (glm::vec2(0 * 0.0625f, 11 * 0.0625f));
		
		case BEDROCK: 
		return (glm::vec2(1 * 0.0625f, 1 * 0.0625f));

		case BLANK: 
		return (glm::vec2(0 * 0.0625f, 4 * 0.0625f));
		
		case TREE: 
		if (normal == NTOP || normal == NBOTTOM)
			return (glm::vec2(5 * 0.0625f, 4 * 0.0625f));
		else if (normal == NLEFT || normal == NRIGHT || normal == NFRONT || normal == NBACK)
			return (glm::vec2(6 * 0.0625f, 4 * 0.0625f));
		
		case SNOW:
		if (normal == NTOP)
			return (glm::vec2(2 * 0.0625f, 4 * 0.0625f));
		else if (normal == NBOTTOM)
			return (glm::vec2(2 * 0.0625f, 0 * 0.0625f));
		else if (normal == NLEFT || normal == NRIGHT || normal == NFRONT || normal == NBACK)
			return (glm::vec2(4 * 0.0625f, 4 * 0.0625f));

		case ICE:
		return (glm::vec2(3 * 0.0625f, 4 * 0.0625f));
	}
	throw NoTypeException();
}

const char* World::NoTypeException::what() const throw() 	{ return ("No Type."); }


