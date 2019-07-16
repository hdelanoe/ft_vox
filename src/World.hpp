#ifndef WORLD_HPP
#define WORLD_HPP

#include "Block.hpp"
#include "ft_vox.hpp"

class World
{
	public:

		World(void);
		~World(void) {};

		double		noise(double x, double y);
		double		ridgenoise(double x, double y);
		double		bignoise(double nx, double ny);
		double		moistureNoise(double x, double y);
		double		wormNoise(double x, double y, double nz);
		void		getBiome(Block*** chunk, glm::vec3 pos, int x, int y, int z);
		void		getSurface(Block*** chunk, glm::vec3 pos, int x, int y, int z);
		void		getUnder(Block*** chunk, glm::vec3 pos, int x, int y, int z);

		void		plantTree(Block*** chunk, int x, int y, int z);
		void		setType(Block* block, int nx, int ny, int nz, float** en, float** mn, int mode);
		glm::vec2	checkType(int blockType, glm::vec3 normal);

		class NoTypeException: public std::exception { virtual const char* what() const throw(); };



	private:

		FastNoise 	bNoise;
		FastNoise 	e1Noise;
		FastNoise 	e2Noise;
		FastNoise 	e3Noise;
		FastNoise 	mNoise;
		FastNoise 	cNoise;
		FastNoise 	tNoise;
		FastNoise 	nNoise;
};

#endif
